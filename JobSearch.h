#pragma once
// Prevents multiple inclusions of this header file

#include <vector>
// Includes the vector container from the C++ Standard Template Library

#include <string>
// Includes the string class from the C++ Standard Template Library

#include <thread>
// Includes the thread class for multi-threading support

#include <mutex>
// Includes the mutex class for thread synchronization

#include <condition_variable>
// Includes the condition_variable class for thread synchronization

#include <atomic>
// Includes the atomic class for thread-safe operations

#include "Job.h"
// Includes the Job header file, which likely contains the Job struct definition

#include "httplib.h"
// Includes the httplib header file for HTTP client functionality

#include "json.hpp"
// Includes the JSON library for parsing JSON data

class JobSearch {
    // Defines the JobSearch class
public:
    JobSearch();
    // Constructor for the JobSearch class

    ~JobSearch();
    // Destructor for the JobSearch class

    void searchJobs(const std::string& query, const std::string& country,
        const std::string& salaryRange, int resultsPerPage, const std::string& currencySymbol);
    // Function to initiate a job search with given parameters

    std::vector<Job> getResults() const;
    // Function to retrieve the search results

    void startSearchThread();
    // Function to start the search thread

    void stopSearchThread();
    // Function to stop the search thread

private:
    std::vector<Job> results;
    // Vector to store the search results

    std::thread searchThread;
    // Thread object for performing searches asynchronously

    mutable std::mutex resultsMutex;
    // Mutex for thread-safe access to the results vector

    std::atomic<bool> stopThread;
    // Atomic flag to signal the search thread to stop

    std::condition_variable cv;
    // Condition variable for thread synchronization

    bool searchRequested;
    // Flag to indicate if a search has been requested

    std::string currentQuery;
    // Current search query

    std::string currentCountry;
    // Current country for the search

    std::string currentSalaryRange;
    // Current salary range for the search

    std::string currentContractType;
    // Current contract type for the search

    int currentResultsPerPage;
    // Current number of results per page

    std::string currentCurrencySymbol;
    // Current currency symbol for the search

    void searchThreadFunction();
    // Function that runs in the search thread

    void performSearch();
    // Function to perform the actual search operation
};

