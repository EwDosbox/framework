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

Application::Application(int initial_width, int initial_height, std::vector<std::string> arguments)
    : PV112Application(initial_width, initial_height, arguments) {

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_MULTISAMPLE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    compile_shaders();
}

Application::~Application() { Application::delete_shaders(); }

// ----------------------------------------------------------------------------
// Methods
// ----------------------------------------------------------------------------

void Application::delete_shaders() {
    glDeleteProgram(main_program);
}

void Application::compile_shaders() {
    delete_shaders();

    main_program = create_program(lecture_shaders_path / "main.vert", lecture_shaders_path / "main.frag");

    std::cout << "Shaders are reloaded." << std::endl;
}

void Application::update(float delta) {}

void Application::render() {
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // TIPS: Always use f after floating numbers, e.g 1.0f. This helps in avoiding glm template errors
    // Task 5.1: Setup the camera.
    //           1. Choose camera position (for example 1.5f, 2.0f, -8.0f is good to see all our objects) as glm::vec3.
    //           2. Create perspective matrix using glm::perspective(float field_of_view, float aspect_ratio, float near, float far).
    //              - glm works in radians by default, you can convert degrees to radians using glm::radians(degrees)
    //              - aspect_ratio is width of the window divided by height of the window
    //              - near and far planes should be: positive, near < far, near should be much smaller than 1.0f, e.g 0.01f
    //           3. Create view matrix using glm::lookAt(glm::vec3 eye_position, glm::vec3 look_at_position, glm::vec3 up_vector).
    //           4. Leave model matrix as identity for now.
    // Task 5.1: Upload MVP matrices to 'program' - use glm::value_ptr to get pointers for your matrices.

    // Task 5.2: Set uniform data for light and upload camera(eye) position to 'program'.
    // Task 5.2: Set uniform data for material. Remember: it's ambient, diffuse, specular, and additionally shininess for material.

    glUseProgram(main_program);

    // Task 5.1: Separate each object by moving them to different locations using glm::translate(glm::mat4 matrix_to_translate, glm::vec3).
    //           Play with other glm functions of the same form:
    //            - rotate (mat4x4 matrix_to_rotate, float angle, glm::vec3 axis_to_rotate_around)
    //            - translate (mat4x4 matrix_to_translate, glm::vec3 move_by_vector)
    cube.draw();
    sphere.draw();
    teapot.draw();
}

void Application::render_ui() { const float unit = ImGui::GetFontSize(); }

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

    if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_F:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        case GLFW_KEY_L:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
        case GLFW_KEY_Q:
            // Task 5.2: Map the change between directional and point light to a key on a keyboard.
            break;
        }
    }
}
