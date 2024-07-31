# Job Search Application

## Overview
This application is a comprehensive job search tool that leverages the Adzuna API to provide users with up-to-date job listings.
It features a graphical user interface built with ImGui, allowing users to search for jobs, view details, and manage favorites.

## Preview:
https://github.com/user-attachments/assets/be5bbd16-bdc1-46bf-8e0f-e9069076370c

## Features
- **Job Search**: Search for jobs by title, location, and salary range.
- **Real-time Results**: Fetch current job listings from the Adzuna API.
- **Detailed Job View**: View comprehensive details of each job listing.
- **Favorites Management**: Save and manage favorite job listings.
- **User-friendly Interface**: Easy-to-use graphical interface built with ImGui.

## Technologies Used
- C++
- ImGui for GUI
- GLFW for window management
- OpenGL for rendering
- nlohmann::json for JSON parsing
- httplib for API requests

## Project Structure
  - Source files
  - `main.cpp`: Entry point of the application
  - `UserInterface.cpp`: Handles the GUI logic
  - `JobSearch.cpp`: Manages job search functionality
  - `FavoritesManager.cpp`: Handles saving and loading of favorite jobs
  - Header files
  - External libraries such as:
      - `ImGui/`: ImGui library files
      - `GLFW/`: GLFW library files
      - `httplib/`: HTTP client library
         etc...
  - Application assets (e.g., fonts)

## Setup and Installation

### Prerequisites
- Visual Studio 2019 or later
- C++17 compatible compiler

### Installation Steps
1. Download the project as a ZIP file from GitHub.
2. Extract the ZIP file to your desired location.
3. Open Visual Studio.
4. Click on "File" > "Open" > "Project/Solution".
5. Navigate to the extracted folder and select the `C++ FinalProject.sln` file.
6. Once the project is loaded, ensure that the solution configuration is set to "Debug" and the solution platform is set to "x64" (you can check this in the toolbar).
7. Click on "Build" > "Build Solution" to compile the project.
8. After successful compilation, click on "run" or press F5 to run the application.

## Usage
1. Upon launching the application, you'll see the main interface.
2. Enter a job title in the search box.
3. Select a country from the dropdown menu.
4. Optionally, select a salary range.
5. Click the "Search" button to fetch job listings.
6. Browse through the results in the left panel (You can also navigate with your keyboard).
7. Click on a job to view its details in the right panel.
8. Use the checkbox next to each job to add or remove it from your favorites.
9. Switch between search results and favorites using the toggle button.

## Acknowledgments
- Adzuna API for providing job data
- ImGui creators for the GUI framework
- GLFW team for the windowing library
