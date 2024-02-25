//
// Created by ccn on 24/02/24.
//

#ifndef MWE_OPENGL_TEXT_GRAPHICS_HPP
#define MWE_OPENGL_TEXT_GRAPHICS_HPP

struct OpenGLFontDrawingInformation {
    ShaderPipeline shader_pipeline;
    GLuint vao_name;
    GLuint vbo_name;
};

void process_input(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

OpenGLFontDrawingInformation configure_opengl(const unsigned int screen_height, const unsigned int screen_width);
std::tuple<bool, GLFWwindow*> initialize_glfw_and_return_window(const unsigned int screen_height, const unsigned int screen_width);
void render(ShaderPipeline shader_pipeline, std::map<GLchar, CharacterDrawingData> char_to_drawing_data, GLuint vao_name, GLuint vbo_name);

#endif //MWE_OPENGL_TEXT_GRAPHICS_HPP
