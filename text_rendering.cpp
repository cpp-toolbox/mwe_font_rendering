//
// Created by ccn on 23/02/24.
//

#include <map>
#include <iostream>

#include <glad/gl.h>
#include "text_rendering.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

/**
 *
 * description:
 *  returns a tuple where the first element is whether or not we were
 *  able to successfully prepare the freetype font.
 *
 *  If we were able to successfully prepare the font, then the second
 *  element is a mapping of characters to their drawing data.
 *
 *  If we were not able to successfully prepare the font, then the
 *  second element is an empty map.
 */

std::tuple<bool, std::map<GLchar, CharacterDrawingData>> prepare_freetype_font() {

    std::map<GLchar, CharacterDrawingData> char_to_drawing_data;

    // FreeType
    // --------
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft)) {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return {false, char_to_drawing_data};
    }

    // find path to font
    std::string font_name = "../resources/fonts/Antonio-VariableFont_wght.ttf";
    if (font_name.empty()) {
        std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
        return {false, char_to_drawing_data};
    }

    // load font as face
    FT_Face face;
    if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return {false, char_to_drawing_data};
    } else {
        // set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, 48);

        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // load first 128 characters of ASCII set
        for (unsigned char c = 0; c < 128; c++) {
            // Load character glyph
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_RED,
                    face->glyph->bitmap.width,
                    face->glyph->bitmap.rows,
                    0,
                    GL_RED,
                    GL_UNSIGNED_BYTE,
                    face->glyph->bitmap.buffer
            );
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            CharacterDrawingData character = {
                    texture,
                    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    static_cast<unsigned int>(face->glyph->advance.x)
            };
            char_to_drawing_data.insert(std::pair<char, CharacterDrawingData>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    return {true, char_to_drawing_data};
}

void render_text(ShaderPipeline &shader, std::map<GLchar, CharacterDrawingData> char_to_drawing_data, GLuint vao_name,
                 GLuint vbo_name, std::string text, float x, float y, float scale, glm::vec3 color) {
    // activate corresponding render state
    // shader.use();
    glUniform3f(glGetUniformLocation(shader.shader_program_id, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(vao_name);

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {

        CharacterDrawingData ch = char_to_drawing_data[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
                // x position, y position, x texture coordinate, y texture coordinate
                {xpos,     ypos + h, 0.0f, 0.0f},
                {xpos,     ypos,     0.0f, 1.0f},
                {xpos + w, ypos,     1.0f, 1.0f},

                {xpos,     ypos + h, 0.0f, 0.0f},
                {xpos + w, ypos,     1.0f, 1.0f},
                {xpos + w, ypos + h, 1.0f, 0.0f}
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, vbo_name);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices),
                        vertices); // be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) *
             scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
