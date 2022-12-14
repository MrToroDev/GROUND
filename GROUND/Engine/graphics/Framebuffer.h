#pragma once

#include "Shader.h"
#include "base/VBO.h"
#include "base/VAO.h"

namespace gr
{
    /**
     * @brief OpenGL Framebuffer class
     * 
     */
    class Framebuffer
    {
    public:
        Framebuffer(int width, int height, const char* vPath, const char* fPath);
        ~Framebuffer();

        void Bind();
        void UnBind();

        // Draw the framebuffer
        void DrawStorage();

        Shader* GetShader() const;
        unsigned int GetTexture() const;

    private:
        unsigned int fbo, rbo, screen;
        Shader* shader;
        base::VAO vao;
        base::VBO* vbo;
    };
}