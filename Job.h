#pragma once
// Prevents multiple inclusions of this header file

#include <string>
// Includes the string class from the C++ Standard Template Library

struct Job {
    // Defines the Job struct to represent a job listing
    std::string title;
    // Title of the job

    std::string company;
    // Name of the company offering the job

    std::string location;
    // Location of the job

    std::string description;
    // Description of the job

    double salary;
    // Salary for the job (as a floating-point number)

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

