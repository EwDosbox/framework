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
    #pragma region Diamond
    GLuint diamond_position_vbo;
    glCreateBuffers(1, &diamond_position_vbo);
    glNamedBufferStorage(diamond_position_vbo, 36 * sizeof(float), diamond_positions, 0);

    GLuint diamond_color_vbo;
    glCreateBuffers(1, &diamond_color_vbo);
    glNamedBufferStorage(diamond_color_vbo, 36 * sizeof(float), diamond_colors, 0);

    glCreateVertexArrays(1, &diamond_vao);
    glVertexArrayVertexBuffer(diamond_vao, 0, diamond_position_vbo, 0, 3 * sizeof(float));
    glVertexArrayVertexBuffer(diamond_vao, 1, diamond_color_vbo, 0, 3 * sizeof(float));

    glEnableVertexArrayAttrib(diamond_vao, 0);
    glEnableVertexArrayAttrib(diamond_vao, 1);

    glVertexArrayAttribFormat(diamond_vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribFormat(diamond_vao, 1, 3, GL_FLOAT, GL_FALSE, 0);

    glVertexArrayAttribBinding(diamond_vao, 0, 0);
    glVertexArrayAttribBinding(diamond_vao, 1, 1);

    #pragma endregion 
    #pragma region Square

    GLuint square_vbo;
    glCreateBuffers(1, &square_vbo);
    glNamedBufferStorage(square_vbo, 36 * sizeof(float), square_data, 0);

    glCreateVertexArrays(1, &square_vao);
    glVertexArrayVertexBuffer(square_vao, 0, square_vbo, 0, 3 * sizeof(float)); //position in main_vao.vert
    glVertexArrayVertexBuffer(square_vao, 1, square_vbo, 18 * sizeof(float), 3 * sizeof(float)); //color in main_vao.vert

    glEnableVertexArrayAttrib(square_vao, 0);
    glEnableVertexArrayAttrib(square_vao, 1);

    glVertexArrayAttribFormat(square_vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribFormat(square_vao, 1, 3, GL_FLOAT, GL_FALSE, 0);

    glVertexArrayAttribBinding(square_vao, 0, 0);
    glVertexArrayAttribBinding(square_vao, 1, 1);

    #pragma endregion
    #pragma region Triangle

    GLuint triangle_vbo;
    glCreateBuffers(1, &triangle_vbo);
    glNamedBufferStorage(triangle_vbo, 3 * sizeof(Vertex), triangle_vertices, 0);

    glCreateVertexArrays(1, &triangle_vao);
    glVertexArrayVertexBuffer(triangle_vao, 0, triangle_vbo, 0, sizeof(Vertex)); //position in main_vao.vert
    glVertexArrayVertexBuffer(triangle_vao, 1, triangle_vbo, 0, sizeof(Vertex)); //color in main_vao.vert

    glEnableVertexArrayAttrib(triangle_vao, 0);
    glEnableVertexArrayAttrib(triangle_vao, 1);

    glVertexArrayAttribFormat(triangle_vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribFormat(triangle_vao, 1, 3, GL_UNSIGNED_BYTE, GL_TRUE, 3 * sizeof(float));
    
    glVertexArrayAttribBinding(triangle_vao, 0, 0);
    glVertexArrayAttribBinding(triangle_vao, 1, 1);

    #pragma endregion
    #pragma region Indexed

    GLuint indexed_indices_vbo;
    glCreateBuffers(1, &indexed_indices_vbo);
    glNamedBufferStorage(indexed_indices_vbo, 12 * sizeof(GL_UNSIGNED_BYTE), diamond_indices, 0);

    GLuint indexed_positions_vbo;
    glCreateBuffers(1, &indexed_positions_vbo);
    glNamedBufferStorage(indexed_positions_vbo, 15 * sizeof(float), diamond_positions_indexed, 0);

    GLuint indexed_colors_vbo;
    glCreateBuffers(1, &indexed_colors_vbo);
    glNamedBufferStorage(indexed_colors_vbo, 15 * sizeof(float), diamond_colors_indexed, 0);

    glCreateVertexArrays(1, &indexed_vao);
    glVertexArrayVertexBuffer(indexed_vao, 0, indexed_positions_vbo, 0, 3 * sizeof(float)); //position in main_vao.vert
    glVertexArrayVertexBuffer(indexed_vao, 1, indexed_colors_vbo, 0, 3 * sizeof(float)); //color in main_vao.vert

    glEnableVertexArrayAttrib(indexed_vao, 0);
    glEnableVertexArrayAttrib(indexed_vao, 1);

    glVertexArrayAttribFormat(indexed_vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribFormat(indexed_vao, 1, 3, GL_FLOAT, GL_FALSE, 0);
    
    glVertexArrayAttribBinding(indexed_vao, 0, 0);
    glVertexArrayAttribBinding(indexed_vao, 1, 1);

    glVertexArrayElementBuffer(indexed_vao, indexed_indices_vbo);

    #pragma endregion

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

    program_square = create_program(lecture_shaders_path / "main_square.vert", lecture_shaders_path / "main_square.frag");
    program_strip = create_program(lecture_shaders_path / "main_strip.vert", lecture_shaders_path / "main_strip.frag");

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
        glUseProgram(program_square);
        glBindVertexArray(vao_empty);
        /*
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawArrays(GL_TRIANGLES, 3, 3);
        */
       glDrawArrays(GL_TRIANGLES, 0, 6);

        break;
    }
    case 2: {
        // Task 3.2: Draw a square using GL_TRIANGLE_STRIP.
        glUseProgram(program_strip);
        glBindVertexArray(vao_empty);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        break;
    }
    case 3: {
        // Task 3.4-3.6: Draw all objects.
        glBindVertexArray(diamond_vao);
        glUseProgram(program_vao);

        glDrawArrays(GL_TRIANGLES, 0, 12);

        glBindVertexArray(square_vao);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        glBindVertexArray(triangle_vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(indexed_vao);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);
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
