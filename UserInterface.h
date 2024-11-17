#pragma once
// Prevents multiple inclusions of this header file
#include "JobSearch.h"
// Includes the JobSearch header file - contains the JobSearch class definition
#include "FavoritesManager.h"
// Includes the FavoritesManager header file - contains the FavoritesManager class definition
#include <vector>
#include <string>

class UserInterface {
public:
    UserInterface();
    // Constructor for the UserInterface class

    void render();
    // Function to render the user interface

private:
    struct CountryInfo {
        std::string name;
        std::string code;
        // Country code
        std::string currencySymbol;
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

    void toggleFavorite(const Job& job);
    // Function to toggle a job's favorite status

};
