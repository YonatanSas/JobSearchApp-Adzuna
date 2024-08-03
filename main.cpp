#include "UserInterface.h"
// Include the header file for the UserInterface class

#include <GLFW/glfw3.h>
// Include the GLFW library for creating windows and handling user input

#include <imgui.h>
// Include the Dear ImGui library for creating the graphical user interface

#include <backends/imgui_impl_opengl3.h>
// Include the ImGui implementation for OpenGL 3
#include <backends/imgui_impl_glfw.h>
// Include the ImGui implementation for GLFW

int main() {
    // The main function, entry point of the program

    if (!glfwInit())
        return 1;
    // Initialize GLFW. If initialization fails, return 1 to indicate an error

    GLFWwindow* window = glfwCreateWindow(1200, 600, "Job Search Application", NULL, NULL);
    // Create a GLFW window with dimensions 1200x600 and title "Job Search Application"
    if (window == NULL)
        return 1;
    // If window creation fails, return 1 to indicate an error

    glfwMakeContextCurrent(window);
    // Make the created window's OpenGL context current

    ImGui::CreateContext();
    // Create an ImGui context

    ImGuiIO& io = ImGui::GetIO();
    // Get ImGui's IO context

    // Define the extended range of characters
    static const ImWchar ranges[] = {
        0x0020, 0x00FF, // Basic Latin + Latin Supplement
        0x2000, 0x206F, // General Punctuation
        0x3000, 0x30FF, // CJK Symbols and Punctuations, Hiragana, Katakana
        0x31F0, 0x31FF, // Katakana Phonetic Extensions
        0xFF00, 0xFFEF, // Half-width characters
        0x4e00, 0x9FAF, // CJK Ideographs
        0x20A0, 0x20CF, // Currency Symbols
        0x0100, 0x024F, // Latin Extended-A and Latin Extended-B
        0x0370, 0x03FF, // Greek and Coptic
        0x0400, 0x04FF, // Cyrillic
        0x0500, 0x052F, // Cyrillic Supplement
        0,
    };

    // Configure font loading
    ImFontConfig config;
    config.MergeMode = false; // We're loading everything at once, so no merge
    config.PixelSnapH = true; // Improves text sharpness

    // Load the font with all required characters
    ImFont* mainFont = io.Fonts->AddFontFromFileTTF("calibri.ttf", 20.0f, &config, ranges);
    if (mainFont == nullptr) {
        // Handle font loading failure
        std::cerr << "Failed to load font!" << std::endl;
    }

    // Build the font atlas
    io.Fonts->Build();


    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    // Enable keyboard navigation in ImGui

    ImGuiStyle& style = ImGui::GetStyle();
    // Get the ImGui style object to customize colors

    // Set various style colors
    style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);  // öáò è÷ñè
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.9961f, 0.9529f, 0.8863f, 1.0f);//öáò çìåï øàùé
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.9961f, 0.9529f, 0.8863f, 1.0f);  // öáò ø÷ò çìåï ÷åôõ
    style.Colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);  // öáò âáåì
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.7451f, 0.7765f, 0.6275f, 1.0f); // öáò ø÷ò îñâøú
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.3765f, 0.4000f, 0.4627f, 1.0f);  // öáò ø÷ò îñâøú áîòáø òëáø
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);  // öáò ø÷ò îñâøú ôòéìä
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);  // öáò ø÷ò ëåúøú
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.9961f, 0.9529f, 0.8863f, 1.0f);  // öáò ø÷ò ëåúøú ôòéìä
    style.Colors[ImGuiCol_Button] = ImVec4(0.78f, 0.78f, 0.78f, 0.67f);  // öáò ëôúåø
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.3765f, 0.4000f, 0.4627f, 1.0f);   // öáò ëôúåø áîòáø òëáø
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.3765f, 0.4000f, 0.4627f, 1.0f);// öáò ëôúåø ôòéì



    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    // Initialize ImGui for use with GLFW and OpenGL

    UserInterface ui;
    // Create an instance of the UserInterface class

    while (!glfwWindowShouldClose(window)) {
        // Main application loop, continue until the window should close
        glfwPollEvents();
        // Process any pending events (keyboard, mouse, etc.)

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // Start a new ImGui frame

        ui.render();
        // Render the user interface

        ImGui::Render();
        // Finish the ImGui frame

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        // Render the ImGui draw data

        glfwSwapBuffers(window);
        // Swap the front and back buffers
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    // Clean up ImGui

    return 0;
    // Return 0 to indicate successful program execution
}



