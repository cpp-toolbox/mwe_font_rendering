#include <iostream>
#include <map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "font_rendering/text_renderer.hpp"
#include "window/window.hpp"

unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;

void render(std::map<GLchar, CharacterDrawingData> char_to_drawing_data, TextRenderer text_renderer) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    text_renderer.render_text(char_to_drawing_data,  "HEALTH: 100", 25.0f, 25.0f, 1.0f,
                              glm::vec3(0.5, 0.8f, 0.2f));
    text_renderer.render_text(char_to_drawing_data,  "this is a test JJJjjjj", 540.0f, 570.0f, 0.5f,
                              glm::vec3(0.3, 0.7f, 0.9f));
}


int main() {

    TextRenderer text_renderer;
    LiveInputState live_input_state;
    GLFWwindow *window = initialize_glfw_glad_and_return_window(&SCR_WIDTH, &SCR_HEIGHT, "mwe text rendering", false, false, false, &live_input_state);

    std::string font_path = "../../src/resources/fonts/SimSun.ttf";
    auto [able_to_prepare_font, char_to_drawing_data] = text_renderer.prepare_freetype_font(font_path);
    if (!able_to_prepare_font) return -1;

    // IMPORTANT TO DO, else crash
    text_renderer.configure_opengl_for_text_rendering(SCR_WIDTH, SCR_HEIGHT);

    while (!glfwWindowShouldClose(window))
    {

        render( char_to_drawing_data, text_renderer);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

