#pragma once
// Prevents multiple inclusions of this header file

#include "JobSearch.h"
// Includes the JobSearch header file, which likely contains the JobSearch class definition

#include "FavoritesManager.h"
// Includes the FavoritesManager header file, which likely contains the FavoritesManager class definition

#include <vector>
// Includes the vector container from the C++ Standard Template Library

#include <string>
// Includes the string class from the C++ Standard Template Library

#include <chrono>
// Includes the chrono library for time-related functions

class UserInterface {
    // Defines the UserInterface class
public:
    UserInterface();
    // Constructor for the UserInterface class

    void render();
    // Function to render the user interface

private:
    struct CountryInfo {
        std::string name;
        // Name of the country
        std::string code;
        // Country code
        std::string currencySymbol;
        // Currency symbol of the country
    };

    std::vector<CountryInfo> countries;
    // Vector to store information about countries

    JobSearch jobSearch;
    // Instance of the JobSearch class to perform job searches

    std::vector<Job> currentSearchResults;
    // Vector to store the current search results

    FavoritesManager favoritesManager;
    // Instance of the FavoritesManager class to manage favorite jobs

    bool showFavorites;
    // Flag to indicate whether to show favorites or search results

    std::vector<Job> favorites;
    // Vector to store favorite jobs

    int selectedJob;
    // Index of the currently selected job

    bool isSearching;
    // Flag to indicate if a search is in progress

    std::chrono::steady_clock::time_point lastSearchTime;
    // Time point of the last search operation

    static bool containsIgnoreCase(const std::string& str, const std::string& substr);
    // Static function to check if a string contains a substring, ignoring case

    void toggleFavorite(const Job& job);
    // Function to toggle a job's favorite status

    void loadFavorites();
    // Function to load favorite jobs

    void saveFavorites();
    // Function to save favorite jobs

    void updateSearchResults();
    // Function to update search results
};


