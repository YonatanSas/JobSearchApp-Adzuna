#pragma once
// Prevents multiple inclusions of this header file

#include <string>

struct Job {
    // Defines the Job struct to represent a job listing
    std::string title;
    std::string company;
    std::string location;
    std::string description;
    double salary;
    std::string salaryCurrency;
    // Currency of the salary
    std::string contractType;
    // Type of contract for the job
    std::string category;
    // Category of the job
    std::string url;
    // URL for more details about the job

    Job() : salary(0.0), salaryCurrency("") {}
    // Default constructor: initializes salary to 0.0 and salaryCurrency to an empty string
};

