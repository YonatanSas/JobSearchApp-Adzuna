#pragma once
// Prevents multiple inclusions of this header file

#include "Job.h"
// Includes the Job header file, which contains the Job struct definition

#include <vector>
#include <string>

class FavoritesManager {
public:
    FavoritesManager();
    // Constructor for the FavoritesManager class

    void saveFavorites(const std::vector<Job>& favorites);
    // Function to save a vector of favorite jobs

    std::vector<Job> loadFavorites();
    // Function to load and return a vector of favorite jobs

private:
    const std::string filename = "favorites.json";
    // Constant string to store the filename for saving/loading favorites
};

