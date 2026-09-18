// ################################################################################
// Common Framework for Computer Graphics Courses at FI MUNI.
// 
// Copyright (c) Visitlab (https://visitlab.fi.muni.cz)
// All rights reserved.
// ################################################################################

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <glad/glad.h>

std::string load_file(std::filesystem::path file_path) {
    file_path.make_preferred();
    std::ifstream infile{file_path};

    if (!infile.is_open()) {
        std::cerr << "File " + file_path.generic_string() + " not found.";
    }

    return {std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>()};
}

GLuint create_shader(std::filesystem::path file_path, GLenum shader_type) {
    const std::string shader_string = load_file(file_path);
    const char* shader_source = shader_string.data();

    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &shader_source, nullptr);
    glCompileShader(shader);

    return shader;
}

GLuint create_program(std::filesystem::path vertex_path, std::filesystem::path fragment_path) {
    GLuint vertex_shader = create_shader(vertex_path, GL_VERTEX_SHADER);
    GLuint fragment_shader = create_shader(fragment_path, GL_FRAGMENT_SHADER);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    // link and get errors
    int link_status;
    glGetProgramiv(program, GL_LINK_STATUS, &link_status);
    if (GL_FALSE == link_status) {
        std::cout << "Failed to link program with shaders: ";
        std::cout << vertex_path << ", "  << fragment_path;
        std::cout << std::endl;

        int log_len = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_len);
        const std::unique_ptr<char[]> log(new char[log_len]);
        glGetProgramInfoLog(program, log_len, nullptr, log.get());
        std::cout << log.get() << std::endl;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    glDetachShader(program, vertex_shader);
    glDetachShader(program, fragment_shader);

    return program;
}
