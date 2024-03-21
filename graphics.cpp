#include <iostream>
#include <map>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

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


void render(std::map<GLchar, CharacterDrawingData> char_to_drawing_data, TextRenderer text_renderer) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    text_renderer.render_text(char_to_drawing_data,  "HEALTH: 100", 25.0f, 25.0f, 1.0f,
                glm::vec3(0.5, 0.8f, 0.2f));
    text_renderer.render_text(char_to_drawing_data,  "this is a test JJJjjjj", 540.0f, 570.0f, 0.5f,
                glm::vec3(0.3, 0.7f, 0.9f));
}

