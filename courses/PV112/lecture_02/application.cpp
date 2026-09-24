// ################################################################################
// Common Framework for Computer Graphics Courses at FI MUNI.
//
// Copyright (c) Visitlab (https://visitlab.fi.muni.cz)
// All rights reserved.
// ################################################################################

#include "application.hpp"

// ----------------------------------------------------------------------------
// Constructors & Destructors
// ----------------------------------------------------------------------------

Application::Application(int initial_width, int initial_height, std::vector<std::string> arguments) : PV112Application(initial_width, initial_height, arguments) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Reserved for future lesson.
    glCreateVertexArrays(1, &vao);

    compile_shaders();
}

Application::~Application() {
    glDeleteVertexArrays(1, &vao);
    delete_shaders();
}

// ----------------------------------------------------------------------------
// Methods
// ----------------------------------------------------------------------------

void Application::delete_shaders() {
    // Task 2.2 Delete the program.
}

std::string load_file(std::filesystem::path path )
{
    std::ifstream file(path);

    std::stringstream string_stream;
    string_stream << file.rdbuf();

    return string_stream.str();

}

GLuint compile_shader(std::filesystem::path path, GLenum type)
{
    GLuint shader = glCreateShader(type);
    std::string source = load_file(path);
    //std::cout << source << std::endl;
    const char* source_char = source.data();
    glShaderSource(shader, 1, &source_char, nullptr);
    glCompileShader(shader);

    return shader;
}

void delete_shader(GLuint shader, GLuint program)
{
    glDetachShader(program, shader);
    glDeleteShader(shader);
}

void Application::compile_shaders() {
    delete_shaders();

    // Task 2.1: Compile vertex & fragment shaders.
    // Help: - There is a variable 'lecture_shaders_path' that contains path to the folder with shaders.
    //       - Use the helper function load_file from utilities to load the contents of a file into a string.
    //       - Method data() callable on std::string type can give you raw C99 pointer that OpenGL takes as an input.
    //         Example: const std::string vertex_shader_string = load_file(lecture_shaders_path / "main.vert");
    //                  const char* vertex_shader_source = vertex_shader_string.data();
    //       - Create a function so that you don't need to copy paste the code for both vertex and the fragment shaders.


    // Task 2.2: Link the shaders to create a program. Do not forget to delete shaders after linking.

    GLuint main_v_shader = compile_shader(lecture_shaders_path / "main.vert", GL_VERTEX_SHADER);
    GLuint main_f_shader = compile_shader(lecture_shaders_path / "main.frag", GL_FRAGMENT_SHADER);

    // Task 2.5: Link the second set of shaders.
    
    main_program = glCreateProgram();
    glAttachShader(main_program, main_v_shader);
    glAttachShader(main_program, main_f_shader);

    glLinkProgram(main_program);

    delete_shader(main_v_shader, main_program);
    delete_shader(main_f_shader, main_program);


    GLuint side_v_shader = compile_shader(lecture_shaders_path / "side.vert", GL_VERTEX_SHADER);
    GLuint side_f_shader = compile_shader(lecture_shaders_path / "side.frag", GL_FRAGMENT_SHADER);

    side_program = glCreateProgram();
    glAttachShader(side_program, side_v_shader);
    glAttachShader(side_program, side_f_shader);

    glLinkProgram(side_program);

    delete_shader(side_v_shader, side_program);
    delete_shader(side_f_shader, side_program);

    std::cout << "Shaders are reloaded." << std::endl;
}

void Application::update(float delta) {}

void Application::render() {
    // Task 2.4: Set the viewport.

    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(vao);


    // Task 2.3: Use the compiled program for drawing.
    // Task 2.5: Use glViewport to render two triangles side by side.
    // Task 2.5: Use the new program to draw the second triangle.
    // Task 2.6: Select the programs used for the left and right side based on the GUI state.

    if(ui_reverse_sides == 0)
    {
        l_program = main_program;
        r_program = side_program;
    }
    else
    {
        l_program = side_program;
        r_program = main_program;
    }

    glViewport(0, 0, width/2, height);
    glUseProgram(l_program);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glViewport(width/2, 0, width/2, height);
    glUseProgram(r_program);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

// ----------------------------------------------------------------------------
// Input Events
// ----------------------------------------------------------------------------

void Application::render_ui() {
    const float unit = ImGui::GetFontSize();

    ImGui::Begin("Configuration", nullptr, ImGuiWindowFlags_NoDecoration);
    ImGui::SetWindowSize(ImVec2(10 * unit, 20 * unit));
    ImGui::SetWindowPos(ImVec2(2 * unit, 2 * unit));
    ImGui::RadioButton("Default", &ui_reverse_sides, 0);
    ImGui::RadioButton("Switch sides", &ui_reverse_sides, 1);
    ImGui::End();
}

void Application::on_resize(int width, int height) {
    // Calls the default implementation to set the class variables.
    IApplication::on_resize(width, height);
}

void Application::on_mouse_move(double x, double y) {}

void Application::on_mouse_button(int button, int action, int mods) {}

void Application::on_key_pressed(int key, int scancode, int action, int mods) {
    // Calls the default implementation that invokes compile_shaders when 'R key is hit.
    PV112Application::on_key_pressed(key, scancode, action, mods);

    // Task 2.7: Switch the order of used programs for drawing on pressing keyboard key of your choice.
    //           Use for example if(action == GLFW_PRESS && key == GLFW_KEY_S){} to decide when the switch happens.
    //           For the interoperation of UI and Inputs use the same variable used in UI `ui_reverse_sides`
    
}
