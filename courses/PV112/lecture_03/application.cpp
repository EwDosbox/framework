// ###############################################################################
// Common Framework for Computer Graphics Courses at FI MUNI.
//
// Copyright (c) Visitlab (https://visitlab.fi.muni.cz)
// All rights reserved.
// ################################################################################

#include "application.hpp"
#include "data.hpp"

// ----------------------------------------------------------------------------
// Constructors & Destructors
// ----------------------------------------------------------------------------

Application::Application(int initial_width, int initial_height, std::vector<std::string> arguments) : PV112Application(initial_width, initial_height, arguments) {

    glCreateVertexArrays(1, &vao_empty);

    // Task 3.3: Create Vertex Buffer Objects (VBO). Copy vertex data found in data.hpp to VBOs.
    // Task 3.4-3.6: Create Vertex Array Objects (VAO) fetching data from VBOs.

    compile_shaders();
}

Application::~Application() {
    glDeleteVertexArrays(1, &vao_empty);

    // Task 3.3: Delete the Vertex Buffer Objects (VBOs).
    // Task 3.4-3.6: Delete Vertex Array Objects (VAOs).

    Application::delete_shaders();
}

// ----------------------------------------------------------------------------
// Methods
// ----------------------------------------------------------------------------

void Application::delete_shaders() {
    glDeleteProgram(program_triangle);
    glDeleteProgram(program_vao);
}

void Application::compile_shaders() {
    delete_shaders();

    program_triangle = create_program(lecture_shaders_path / "main.vert", lecture_shaders_path / "main.frag");
    program_vao = create_program(lecture_shaders_path / "main_vao.vert", lecture_shaders_path / "main_vao.frag");

    std::cout << "Shaders are reloaded." << std::endl;
}

void Application::update(float delta) {}

void Application::render() {
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);

    switch (ui_chosen_program) {
    case 0: {
        // Good ol' triangle.
        glUseProgram(program_triangle);
        glBindVertexArray(vao_empty);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        break;
    }
    case 1: {
        // Task 3.1: Draw a square.

        break;
    }
    case 2: {
        // Task 3.2: Draw a square using GL_TRIANGLE_STRIP.

        break;
    }
    case 3: {
        // Task 3.4-3.6: Draw all objects.

        break;
    }
    }
}

void Application::render_ui() {
    const float unit = ImGui::GetFontSize();

    ImGui::Begin("Configuration", nullptr, ImGuiWindowFlags_NoDecoration);
    ImGui::SetWindowSize(ImVec2(15 * unit, 20 * unit));
    ImGui::SetWindowPos(ImVec2(2 * unit, 2 * unit));
    ImGui::RadioButton("Triangle", &ui_chosen_program, 0);
    ImGui::RadioButton("Square", &ui_chosen_program, 1);
    ImGui::RadioButton("Square (triangle strip)", &ui_chosen_program, 2);
    ImGui::RadioButton("VAO-based objects", &ui_chosen_program, 3);
    ImGui::End();
}

// ----------------------------------------------------------------------------
// Input Events
// ----------------------------------------------------------------------------

void Application::on_resize(int width, int height) {
    // Calls the default implementation to set the class variables.
    PV112Application::on_resize(width, height);
}

void Application::on_mouse_move(double x, double y) {}

void Application::on_mouse_button(int button, int action, int mods) {}

void Application::on_key_pressed(int key, int scancode, int action, int mods) {
    // Calls default implementation that invokes compile_shaders when 'R key is hit.
    PV112Application::on_key_pressed(key, scancode, action, mods);
}
