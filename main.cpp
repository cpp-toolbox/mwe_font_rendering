#include <iostream>
#include <map>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "graphics.hpp"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main() {

    TextRenderer text_renderer;

    auto [able_to_initialize, window] = initialize_glfw_and_return_window(SCR_WIDTH, SCR_HEIGHT);
    if (!able_to_initialize) return -1;

    std::string font_path = "../resources/fonts/SimSun.ttf";
    auto [able_to_prepare_font, char_to_drawing_data] = text_renderer.prepare_freetype_font(font_path);
    if (!able_to_prepare_font) return -1;

    // IMPORTANT TO DO, else crash
    text_renderer.configure_opengl_for_text_rendering(SCR_WIDTH, SCR_HEIGHT);

    while (!glfwWindowShouldClose(window))
    {
        process_input(window);

        render( char_to_drawing_data, text_renderer);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

