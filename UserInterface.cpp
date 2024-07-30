#include "UserInterface.h"
// Includes the header file for the UserInterface class, providing access to its declarations

#include <imgui.h>
// Includes the Dear ImGui library, which is used for creating the graphical user interface

#include <cstring>
// Includes the C-style string handling functions

#include <algorithm>
// Includes the C++ Standard Template Library algorithms

#include <windows.h>
// Includes the Windows API header, used for Windows-specific functionality

#include <shellapi.h>
// Includes the Windows Shell API, used for opening URLs in the default browser

#include <iomanip>
// Includes input/output manipulators for formatting output

#include <sstream>
// Includes the string stream class for string manipulation

#include <limits>
// Includes numeric limits, used for handling minimum and maximum values of data types

UserInterface::UserInterface() : showFavorites(false), selectedJob(-1), isSearching(false) {
    // Constructor for the UserInterface class
    // Initializes showFavorites to false (start in search mode)
    // Initializes selectedJob to -1 (no job selected)
    // Initializes isSearching to false (not currently searching)
    countries = { {"Please select a country"},
        {"Great Britain", "gb", "GBP"},
        {"Australia", "au", "AUD"},
        {"Belgium", "be", "EUR"},
        {"Brazil", "br", "BRL"},
        {"Canada", "ca", "CAD"},
        {"Switzerland", "ch", "CHF"},
        {"Germany", "de", "EUR"},
        {"Spain", "es", "EUR"},
        {"France", "fr", "EUR"},
        {"United Kingdom", "gb", "GBP"},
        {"India", "in", "INR"},
        {"Italy", "it", "EUR"},
        {"Mexico", "mx", "MXN"},
        {"Netherlands", "nl", "EUR"},
        {"New Zealand", "nz", "NZD"},
        {"Poland", "pl", "PLN"},
        {"Singapore", "sg", "SGD"},
        {"United States", "us", "USD"},
        {"South Africa", "za", "ZAR"}
    };
    // Initializes the countries vector with country information
    // Each entry contains the country name, country code, and currency symbol

    loadFavorites();
    // Calls the loadFavorites function to load any previously saved favorite jobs
}

void UserInterface::toggleFavorite(const Job& job) {
    // Function to toggle the favorite status of a job
    auto it = std::find_if(favorites.begin(), favorites.end(),
        [&job](const Job& fav) { return fav.url == job.url; });
    // Searches for the job in the favorites list using a lambda function
    // The lambda compares the URL of each favorite job with the URL of the given job

    if (it != favorites.end()) {
        favorites.erase(it);
        // If the job is found in favorites, remove it
    }
    else {
        favorites.push_back(job);
        // If the job is not in favorites, add it
    }

    saveFavorites();
    // Save the updated favorites list
}

void UserInterface::loadFavorites() {
    // Function to load favorite jobs from storage
    favorites = favoritesManager.loadFavorites();
    // Calls the loadFavorites function of the favoritesManager object
    // This loads the saved favorites and assigns them to the favorites vector
}

void UserInterface::saveFavorites() {
    // Function to save favorite jobs to storage
    favoritesManager.saveFavorites(favorites);
    // Calls the saveFavorites function of the favoritesManager object
    // This saves the current favorites vector to persistent storage
}

std::string formatSalary(double salary, const std::string& currency) {
    // Function to format a salary with thousands separators and currency symbol
    std::stringstream ss;
    ss << std::fixed << std::setprecision(0) << std::round(salary);
    std::string strSalary = ss.str();
    // Converts the salary to a string with no decimal places

    int length = static_cast<int>(strSalary.length());
    int insertPosition = length - 3;
    while (insertPosition > 0) {
        strSalary.insert(static_cast<size_t>(insertPosition), ",");
        insertPosition -= 3;
    }
    // Inserts thousands separators (commas) into the salary string

    return strSalary + " " + currency;
    // Returns the formatted salary string with the currency symbol appended
}

bool shouldDisplayJob(const Job& job, const std::string& salaryRange) {
    // Function to determine if a job should be displayed based on the selected salary range
    double minSalary = 0.0, maxSalary = (std::numeric_limits<double>::max)();
    // Initialize minimum and maximum salaries
    // The maximum is set to the highest possible double value

    if (salaryRange == "100000+") {
        minSalary = 100000.0;
        // If the range is "100000+", set the minimum salary to 100000
    }
    else {
        size_t dashPos = salaryRange.find('-');
        if (dashPos != std::string::npos) {
            minSalary = std::stod(salaryRange.substr(0, dashPos));
            maxSalary = std::stod(salaryRange.substr(dashPos + 1));
            // If the range contains a dash, parse the min and max salaries
        }
    }

    return job.salary >= minSalary && job.salary <= maxSalary;
    // Return true if the job's salary is within the specified range
}

void UserInterface::updateSearchResults() {
    // Function to update the search results
    if (isSearching) {
        // If a search is in progress
        auto now = std::chrono::steady_clock::now();
        // Get the current time
        if (std::chrono::duration_cast<std::chrono::seconds>(now - lastSearchTime).count() >= 1) {
            // If at least 1 second has passed since the last update
            currentSearchResults = jobSearch.getResults();
            // Get the latest results from the jobSearch object
            if (!currentSearchResults.empty()) {
                isSearching = false;
                // If results are received, set isSearching to false
            }
            lastSearchTime = now;
            // Update the last search time
        }
    }
}

void UserInterface::render() {
    // Function to render the user interface
    updateSearchResults();
    // Update search results before rendering

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    // Set the position and size of the next ImGui window to cover the entire screen

    ImGui::Begin("Job Search", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    // Begin a new ImGui window with no decorations, movement, or resizing allowed

    static const char* salaryRanges[] = { "Any", "0-20000", "20000-40000", "40000-60000", "60000-80000", "80000-100000", "100000+" };
    static int salaryIndex = 0;
    // Define an array of salary ranges and an index to keep track of the selected range

    if (!showFavorites) {
        // If not showing favorites (i.e., in search mode)
        static char searchBuffer[256] = "";
        ImGui::InputText("Job Title", searchBuffer, IM_ARRAYSIZE(searchBuffer));
        // Create an input text field for the job title search

        static int countryIndex = 0;
        if (ImGui::BeginCombo("Country", countries[countryIndex].name.c_str())) {
            // Begin a combo box (dropdown) for country selection
            for (int i = 0; i < static_cast<int>(countries.size()); i++) {
                const bool isSelected = (countryIndex == i);
                if (ImGui::Selectable(countries[i].name.c_str(), isSelected)) {
                    countryIndex = i;
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            // For each country, create a selectable item in the dropdown
            ImGui::EndCombo();
        }

        ImGui::Combo("Salary Range", &salaryIndex, salaryRanges, IM_ARRAYSIZE(salaryRanges));
        // Create a combo box for salary range selection

        static int resultsPerPage = 20;
        ImGui::SliderInt("Results Per Page", &resultsPerPage, 1, 50);
        // Create a slider to select the number of results per page

        if (ImGui::Button("Search")) {
            // If the Search button is clicked
            if (strlen(searchBuffer) > 0 && countryIndex != 0) {
                // If a job title is entered and a country is selected
                std::string salaryRange = salaryRanges[salaryIndex];
                jobSearch.searchJobs(searchBuffer, countries[countryIndex].code, salaryRange, resultsPerPage, countries[countryIndex].currencySymbol);
                // Initiate a job search with the specified parameters
                isSearching = true;
                lastSearchTime = std::chrono::steady_clock::now();
                selectedJob = -1;  // Reset selected job
                currentSearchResults.clear();  // Clear previous results
            }
            else {
                // If search criteria are invalid, show error popups
                if (strlen(searchBuffer) == 0) {
                    ImGui::OpenPopup("Search Error");
                }
                else if (countryIndex == 0) {
                    ImGui::OpenPopup("Country Error");
                }
            }
        }

        ImGui::SameLine();
    }

    if (ImGui::Button(showFavorites ? "Back to Search" : "Favorites")) {
        showFavorites = !showFavorites;
        selectedJob = -1;  // Reset selected job
    }
    // Create a button to toggle between search results and favorites

    // Render error popups
    if (ImGui::BeginPopupModal("Country Error", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Please select a country before searching.");
        if (ImGui::Button("OK")) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    if (ImGui::BeginPopupModal("Search Error", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Please enter a job title to search.");
        if (ImGui::Button("OK")) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    ImGui::Separator();
    // Add a separator line

    ImGui::Columns(2, "JobColumns", true);
    // Create two columns for the layout

    if (showFavorites) {
        // If showing favorites
        ImGui::Text("Favorites");

        static std::vector<char> selectedFavorites(favorites.size(), false);
        if (selectedFavorites.size() != favorites.size()) {
            selectedFavorites.resize(favorites.size(), false);
        }
        // Create and maintain a vector to keep track of selected favorites

        if (ImGui::Button("Delete Selected")) {
            // If the Delete Selected button is clicked
            for (int i = static_cast<int>(favorites.size()) - 1; i >= 0; --i) {
                if (selectedFavorites[i]) {
                    favorites.erase(favorites.begin() + i);
                    selectedFavorites.erase(selectedFavorites.begin() + i);
                }
            }
            // Remove selected favorites
            saveFavorites();
            // Save the updated favorites list
            if (selectedJob >= static_cast<int>(favorites.size())) {
                selectedJob = -1;
            }
            // Reset selected job if it's no longer valid
        }

        ImGui::BeginChild("FavoritesList", ImVec2(0, 0), true);
        for (int i = 0; i < static_cast<int>(favorites.size()); i++) {
            const auto& job = favorites[i];
            bool isSelected = selectedFavorites[i] != 0;
            if (ImGui::Checkbox(("##fav" + std::to_string(i)).c_str(), &isSelected)) {
                selectedFavorites[i] = isSelected ? 1 : 0;
            }
            ImGui::SameLine();
            if (ImGui::Selectable((job.title + "##fav" + std::to_string(i)).c_str(), selectedJob == i)) {
                selectedJob = i;
            }
        }
        // Render the list of favorite jobs with checkboxes and make them selectable
        ImGui::EndChild();
    }
    else {
        // If showing search results
        updateSearchResults();

        if (isSearching) {
            ImGui::Text("Searching...");
        }
        else {
            ImGui::Text("Displaying %d jobs", static_cast<int>(currentSearchResults.size()));
        }

        ImGui::BeginChild("JobList", ImVec2(0, 0), true);
        for (int i = 0; i < static_cast<int>(currentSearchResults.size()); i++) {
            const auto& job = currentSearchResults[i];
            if (shouldDisplayJob(job, salaryRanges[salaryIndex])) {
                bool isFavorite = std::find_if(favorites.begin(), favorites.end(),
                    [&job](const Job& fav) { return fav.url == job.url; }) != favorites.end();
                if (ImGui::Checkbox(("##" + std::to_string(i)).c_str(), &isFavorite)) {
                    toggleFavorite(job);
                }
                ImGui::SameLine();
                if (ImGui::Selectable((job.title + "##" + std::to_string(i)).c_str(), selectedJob == i)) {
                    selectedJob = i;
                }
            }
        }
        // Render the list of search results, allowing toggling of favorites and selection of jobs
        ImGui::EndChild();
    }

    ImGui::NextColumn();
    // Move to the next column

    ImGui::Text("Selected job:");
    ImGui::BeginChild("JobDetails", ImVec2(0, 0), true);

    const Job* selectedJobPtr = nullptr;
    if (showFavorites && selectedJob >= 0 && selectedJob < static_cast<int>(favorites.size())) {
        selectedJobPtr = &favorites[selectedJob];
    }
    else if (!showFavorites && selectedJob >= 0 && selectedJob < static_cast<int>(currentSearchResults.size())) {
        selectedJobPtr = &currentSearchResults[selectedJob];
    }
    // Determine which job is currently selected

    if (selectedJobPtr) {
        // If a job is selected, display its details
        const auto& job = *selectedJobPtr;
        ImGui::Text("Name of The Position: %s", job.title.c_str());
        ImGui::Text("Company: %s", job.company.c_str());
        ImGui::Text("Location: %s", job.location.c_str());
        if (job.salary == 0.0)
            ImGui::Text("Salary: Not Specified");
        else
            ImGui::Text("Salary: %s", formatSalary(job.salary, job.salaryCurrency).c_str());
        ImGui::Text("Category: %s", job.category.c_str());
        ImGui::Text("Contract Type: %s", job.contractType.c_str());

        ImGui::Separator();

        ImGui::TextWrapped("Description:");
        ImGui::TextWrapped("%s", job.description.c_str());

        if (ImGui::Button("Apply for this job")) {
            ShellExecuteA(0, "open", job.url.c_str(), 0, 0, SW_SHOW);
        }
        // Add a button to open the job's URL in the default web browser when clicked
    }
    else {
        ImGui::Text("No job selected");
        // Display a message if no job is selected
    }

    ImGui::EndChild();
    // End the child window for job details

    ImGui::Columns(1);
    // Reset the column layout to a single column

    ImGui::End();
    // End the main ImGui window
}

void UserInterface::handleInput() {
    // This method is currently empty, but you can add any additional input handling here if needed
}

bool UserInterface::containsIgnoreCase(const std::string& str, const std::string& substr) {
    // Function to check if a string contains a substring, ignoring case
    auto it = std::search(
        str.begin(), str.end(),
        substr.begin(), substr.end(),
        [](char ch1, char ch2) { return std::toupper(ch1) == std::toupper(ch2); }
    );
    // Use the std::search algorithm with a lambda function to perform case-insensitive comparison
    return (it != str.end());
    // Return true if the substring is found, false otherwise
}