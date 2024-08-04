#define CPPHTTPLIB_OPENSSL_SUPPORT
// Define this macro to enable OpenSSL support in the httplib library

#include "JobSearch.h"
// Include the header file for the JobSearch class

#include <sstream>
// Include the string stream library for string manipulation

JobSearch::JobSearch() : stopThread(false), searchRequested(false) {
    // Constructor for the JobSearch class
    // Initialize stopThread and searchRequested to false

    updateApiCredentials(app_id, api_key, "config.ini");

    startSearchThread();
    // Start the search thread when the object is created
}

JobSearch::~JobSearch() {
    // Destructor for the JobSearch class
    stopSearchThread();
    // Stop the search thread when the object is destroyed
}

void JobSearch::searchJobs(const std::string& query, const std::string& country,
    const std::string& salaryRange,
    int resultsPerPage, const std::string& currencySymbol) {
    // Function to initiate a job search with the given parameters

    std::unique_lock<std::mutex> lock(resultsMutex);
    // Lock the mutex to ensure thread-safe access to shared data

    results.clear();  // Clear previous results
    // Clear the previous search results

    currentQuery = query;
    currentCountry = country;
    currentSalaryRange = salaryRange;
    currentResultsPerPage = resultsPerPage;
    currentCurrencySymbol = currencySymbol;
    // Store the search parameters

    searchRequested = true;
    // Set the flag to indicate a search has been requested

    cv.notify_one();
    // Notify the search thread that a new search has been requested
}

std::vector<Job> JobSearch::getResults() const {
    // Function to retrieve the current search results
    std::lock_guard<std::mutex> lock(resultsMutex);
    // Lock the mutex to ensure thread-safe access to the results

    return results;
    // Return a copy of the results vector
}

void JobSearch::startSearchThread() {
    // Function to start the search thread
    searchThread = std::thread(&JobSearch::searchThreadFunction, this);
    // Create and start a new thread, running the searchThreadFunction
}

void JobSearch::stopSearchThread() {
    // Function to stop the search thread
    {
        std::unique_lock<std::mutex> lock(resultsMutex);
        // Lock the mutex

        stopThread = true;
        // Set the flag to stop the thread
    }

    cv.notify_one();
    // Notify the search thread to check its stop condition
    if (searchThread.joinable()) {
        searchThread.join();
        // Wait for the search thread to finish if it's joinable
    }
}

void JobSearch::searchThreadFunction() {
    // The main function that runs in the search thread
    while (!stopThread) {
        // Continue running until stopThread is set to true
        std::unique_lock<std::mutex> lock(resultsMutex);
        // Lock the mutex

        cv.wait(lock, [this] { return searchRequested || stopThread; });
        // Wait for a search request or a stop signal

        if (stopThread) break;
        // If stop is requested, exit the loop

        if (searchRequested) {

            lock.unlock();
            // Unlock the mutex before performing the search

            performSearch();
            // Perform the actual search

            searchRequested = false;
            // Reset the search request flag
        }
    }
}

bool JobSearch::getSearchRequested() {
    return searchRequested;
}

void JobSearch::performSearch() {
    // Function to perform the actual job search

    std::string encodedQuery = httplib::detail::encode_url(currentQuery);
    // URL-encode the search query

    std::stringstream ss;
    ss << "/v1/api/jobs/" << currentCountry << "/search/1?app_id=" << app_id
        << "&app_key=" << api_key
        << "&results_per_page=" << currentResultsPerPage
        << "&what=" << encodedQuery;
    // Construct the API request URL

    if (currentSalaryRange != "Any") {
        size_t dashPos = currentSalaryRange.find('-');
        if (dashPos != std::string::npos) {
            ss << "&salary_min=" << currentSalaryRange.substr(0, dashPos)
                << "&salary_max=" << currentSalaryRange.substr(dashPos + 1);
        }
        else if (currentSalaryRange == "100000+") {
            ss << "&salary_min=100000";
        }
    }
    // Add salary range parameters to the URL if specified

    std::string path = ss.str();
    std::cout << "Full URL: https://api.adzuna.com" << path << std::endl;
    // Log the full URL for debugging purposes

    httplib::SSLClient cli("api.adzuna.com");
    cli.set_connection_timeout(10);
    // Create an SSL client with a 10-second timeout

    httplib::Headers headers = {
        { "Accept", "application/json" }
    };
    // Set the request headers

    auto res = cli.Get(path.c_str(), headers);
    // Send the GET request to the API

    std::vector<Job> newResults;
    // Vector to store the new search results

    if (res && res->status == 200) {
        // If the request was successful (HTTP status 200)
            auto json = nlohmann::json::parse(res->body);
            // Parse the JSON response

            for (const auto& result : json["results"]) {
                // Iterate through each job in the results
                Job job;
                job.title = result.value("title", "Not specified");
                job.company = result.value("company", nlohmann::json::object()).value("display_name", "Not specified");
                job.location = result.value("location", nlohmann::json::object()).value("display_name", "Not specified");
                job.description = result.value("description", "No description available");
                job.salary = result.value("salary_min", 0.0);
                job.salaryCurrency = currentCurrencySymbol;
                job.category = result.value("category", nlohmann::json::object()).value("label", "Not specified");
                job.contractType = result.value("contract_type", "Not specified");
                job.url = result.value("redirect_url", "Not specified");
                // Populate a Job object with data from the JSON

                newResults.push_back(job);
                // Add the job to the new results vector
            }

            std::cout << "Found " << newResults.size() << " jobs." << std::endl;
            // Log the number of jobs found
    }
    else {
        std::cerr << "API request failed. Status: " << (res ? std::to_string(res->status) : "No response") << std::endl;
        if (res) {
            std::cerr << "Response body: " << res->body << std::endl;
        }
        // Log API request failures
    }

    {
        std::lock_guard<std::mutex> lock(resultsMutex);
        results = std::move(newResults);
        // Update the results vector with the new search results
        // Use std::move for efficiency, transferring ownership of newResults to results
    }
}

void JobSearch::updateApiCredentials(std::string& app_id, std::string& app_key, const std::string& configPath) {
    std::ifstream configFile(configPath);
    if (!configFile.is_open()) {
        throw std::runtime_error("Unable to open config file: " + configPath);
    }

    std::string line;
    bool foundId = false, foundKey = false;

    while (std::getline(configFile, line)) {
        if (line.find("app_id=") != std::string::npos) {
            app_id = line.substr(line.find("=") + 1);
            foundId = true;
        }
        else if (line.find("api_key=") != std::string::npos) {
            app_key = line.substr(line.find("=") + 1);
            foundKey = true;
        }

        if (foundId && foundKey) break;
    }

    if (!foundId || !foundKey) {
        throw std::runtime_error("API credentials not found in config file");
    }
}
