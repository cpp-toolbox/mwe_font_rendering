//
// Created by ccn on 23/02/24.
//

#ifndef MWE_OPENGL_TEXT_TEXT_RENDERING_HPP
#define MWE_OPENGL_TEXT_TEXT_RENDERING_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader_pipeline/shader_pipeline.hpp"
#include <string>

/// Holds all state information relevant to a character as loaded using FreeType
struct CharacterDrawingData {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2 Size;      // Size of glyph
    glm::ivec2 Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};

std::tuple<bool, std::map<GLchar, CharacterDrawingData>> prepare_freetype_font();

void render_text(ShaderPipeline &shader, std::map<GLchar, CharacterDrawingData> char_to_drawing_data, GLuint vao_name,
                 GLuint vbo_name, std::string text, float x, float y, float scale, glm::vec3 color);


#endif //MWE_OPENGL_TEXT_TEXT_RENDERING_HPP
