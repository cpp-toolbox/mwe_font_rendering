//
// Created by ccn on 24/02/24.
//

#include <iostream>
#include <map>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "text_rendering.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "shader_pipeline/shader_pipeline.hpp"

#include "graphics.hpp"

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void process_input(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


std::tuple<bool, GLFWwindow *>
initialize_glfw_and_return_window(const unsigned int screen_width, const unsigned int screen_height) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(screen_width, screen_height, "mwe font rendering", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return {false, NULL};
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    // if (gladLoadGL())
    if (!gladLoadGL((GLADloadfunc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return {false, NULL};
    }

    return {true, window};

}

OpenGLFontDrawingInformation configure_opengl(const unsigned int screen_width, const unsigned int screen_height) {

    // OpenGL state
    // ------------
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // compile and setup the shader_pipeline
    // ----------------------------
    ShaderPipeline shader_pipeline;
    shader_pipeline.load_in_shaders_from_file("../shaders/text.vert", "../shaders/text.frag");
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(screen_width), 0.0f, static_cast<float>(screen_height));
    glUniformMatrix4fv(glGetUniformLocation(shader_pipeline.shader_program_id, "projection"), 1, GL_FALSE,
                       glm::value_ptr(projection));

    // configure VAO/VBO for texture quads
    // -----------------------------------
    GLuint vao_name, vbo_name;
    glGenVertexArrays(1, &vao_name);
    glGenBuffers(1, &vbo_name);
    glBindVertexArray(vao_name);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_name);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

    // a generic vertex type that contains all information
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return {shader_pipeline, vao_name, vbo_name};

}

void
render(ShaderPipeline shader_pipeline, std::map<GLchar, CharacterDrawingData> char_to_drawing_data, GLuint vao_name,
       GLuint vbo_name) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    render_text(shader_pipeline, char_to_drawing_data, vao_name, vbo_name, "Yes, it's working", 25.0f, 25.0f, 1.0f,
                glm::vec3(0.5, 0.8f, 0.2f));
    render_text(shader_pipeline, char_to_drawing_data, vao_name, vbo_name, "Font Rendering :)", 540.0f, 570.0f, 0.5f,
                glm::vec3(0.3, 0.7f, 0.9f));
}

