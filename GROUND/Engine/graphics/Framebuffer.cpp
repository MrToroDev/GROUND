#include "Framebuffer.h"
#include "Shapes.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "../GR_cross_definitions.h"
#include <string>

namespace gr
{
    Framebuffer::Framebuffer(int width, int height, const char* vPath, const char* fPath)
    {
        glGenFramebuffers(1, &fbo);

        glGenTextures(1, &screen);
        glBindTexture(GL_TEXTURE_2D, screen);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screen, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
        
        unsigned int fbostatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (fbostatus != GL_FRAMEBUFFER_COMPLETE)
            gr::LogError((std::string("Framebuffer error: ").append(std::to_string(fbostatus))).c_str());

        shader = new Shader(vPath, fPath);

        vao.Bind();
        vbo = new base::VBO(sizeof(shapes2D::square), shapes2D::square);
        vbo->Bind();
        shader->setVertexAttrib("aPos", 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, position));
        shader->setVertexAttrib("aTexcoord", 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

        vbo->UnBind();
        vao.UnBind();
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
    Framebuffer::~Framebuffer()
    {
        vao.Delete();
        vbo->Delete();
        glDeleteFramebuffers(1, &fbo);
        glDeleteRenderbuffers(1, &rbo);
        glDeleteTextures(1, &screen);
        shader->Delete();
        delete shader;
        delete vbo;
    }
    
    void Framebuffer::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    }
    
    void Framebuffer::UnBind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
    void Framebuffer::DrawStorage()
    {
        shader->use();
        shader->setInt("uTexture", 0);
        glBindTexture(GL_TEXTURE_2D, screen);
        vao.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
        vao.UnBind();
    }
    
    Shader* Framebuffer::GetShader() const
    {
        return this->shader;
    }
    unsigned int Framebuffer::GetTexture() const
    {
        return screen;
    }
}