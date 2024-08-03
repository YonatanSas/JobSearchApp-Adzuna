#include "FavoritesManager.h"
// Includes the FavoritesManager header file

#include <fstream>
// Includes the file stream classes for file I/O operations

#include <json.hpp>
// Includes the JSON library for parsing and creating JSON data

FavoritesManager::FavoritesManager() {}
// Constructor implementation

void FavoritesManager::saveFavorites(const std::vector<Job>& favorites) {
    // Function to save favorite jobs to a JSON file
    nlohmann::json j;
    // Create a JSON object

    for (const auto& job : favorites) {
        // Iterate through each job in the favorites vector
        j.push_back({
            {"title", job.title},
            {"company", job.company},
            {"location", job.location},
            {"description", job.description},
            {"salary", job.salary},
            {"salaryCurrency", job.salaryCurrency},
            {"contractType", job.contractType},
            {"category", job.category},
            {"url", job.url}
            });
        // Add each job's details as a JSON object to the array
    }

    std::ofstream file(filename);
    // Open the file for writing

    file << j.dump(4);  // Pretty print with 4 spaces
    // Write the JSON data to the file with formatting
}

std::vector<Job> FavoritesManager::loadFavorites() {
    // Function to load favorite jobs from a JSON file
    std::vector<Job> favorites;
    // Vector to store loaded favorite jobs

    std::ifstream file(filename);
    // Open the file for reading
    if (file.peek() == std::ifstream::traits_type::eof()) {
        // If the favorites file is empty, init with empty vector
        return std::vector<Job>();
    }

    if (file.is_open()) {
        // Check if the file was successfully opened
        nlohmann::json j;
        // Create a JSON object

        file >> j;
        // Read the file contents into the JSON object

        for (const auto& jobJson : j) {
            // Iterate through each job in the JSON array
            Job job;
            // Create a new Job object

            job.title = jobJson["title"];
            job.company = jobJson["company"];
            job.location = jobJson["location"];
            job.description = jobJson["description"];
            job.salary = jobJson["salary"];
            job.salaryCurrency = jobJson["salaryCurrency"];
            job.contractType = jobJson["contractType"];
            job.category = jobJson["category"];
            job.url = jobJson["url"];
            // Populate the Job object with data from the JSON

            favorites.push_back(job);
            // Add the Job object to the favorites vector
        }
    }

    return favorites;
    // Return the vector of loaded favorite jobs
}


