#include "ModelComponent.h"
#include "../graphics/GraphicLoader.h"
#include "../GR_cross_definitions.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../system/DeviceInfo.h"

namespace gr {
    ModelComponent::ModelComponent(const char* Modelpath, const char* Texturepath, std::string Fshader, std::string Vshader, unsigned int Tcount)
    {
        res = gr::loadOBJ(Modelpath, this->vertices, this->uvs, this->normals);
        this->shader = new gr::Shader(Vshader.c_str(), Fshader.c_str());
        this->TextureCount = Tcount;
        this->TexturePath = Texturepath;
    }

    void ModelComponent::SetProjectionView(glm::mat4 p, glm::mat4 v)
    {
        this->projection = p;
        this->view = v;
    }

    void ModelComponent::SetLightAttribute(glm::vec3 color, glm::vec3 pos, glm::vec3 camPos, glm::vec3 camFront)
    {
        this->lColor = color;
        this->lPos = pos;
        this->cPos = camPos;
        this->cFront = camFront;
    }

    gr::Shader* ModelComponent::GetShader()
    {
        return this->shader;
    }

    unsigned int ModelComponent::GetNormalBuffer()
    {
        return this->NormalBuffer;
    }

    unsigned int ModelComponent::GetUVBuffer()
    {
        return this->UVBuffer;
    }

    unsigned int ModelComponent::GetVertexBuffer()
    {
        return this->VertexBuffer;
    }

    void ModelComponent::init()
    {
        if (!entity->hasComponent<TransformComponent>()) {
            entity->addComponent<TransformComponent>();
        }
        else { transform = &entity->getComponent<TransformComponent>(); }

        if (!res) {
            gr::LogError("ERROR! cannot load the 3D model! No source file on directory!");
            return;
        }
        else
        {
            TextureID = gr::LoadTexture2D(TexturePath.c_str());

            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);

            gr::Log("-- Creating model component --");

            glGenBuffers(1, &VertexBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
             
            gr::Log(std::string("Vertex Buffer size allocated: ").append(std::to_string(gr::GetMemsizeBuffer(VertexBuffer))).append(" bytes").c_str());

            glGenBuffers(1, &UVBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, UVBuffer);
            glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

            gr::Log(std::string("UV Buffer size allocated: ").append(std::to_string(gr::GetMemsizeBuffer(UVBuffer))).append(" bytes").c_str());

            glGenBuffers(1, &NormalBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, NormalBuffer);
            glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

            gr::Log(std::string("Normal Buffer size allocated: ").append(std::to_string(gr::GetMemsizeBuffer(NormalBuffer))).append(" bytes").c_str());

            glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
            shader->setVertexAttrib("aPos", 3, GL_FLOAT, 0, (void*)0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glBindBuffer(GL_ARRAY_BUFFER, UVBuffer);
            shader->setVertexAttrib("aUV", 3, GL_FLOAT, 0, (void*)0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glBindBuffer(GL_ARRAY_BUFFER, NormalBuffer);
            shader->setVertexAttrib("aNormal", 3, GL_FLOAT, 0, (void*)0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            gr::Log("-- Init function ended --");
        }
    }

    void ModelComponent::draw()
    {
        shader->use();

        model = glm::mat4(1.0f);
        model = glm::translate(model, transform->position);
        model = glm::rotate(model, transform->angle, transform->angleAxis);
        model = glm::scale(model, transform->size);

        shader->setMat4("model", model);
        shader->setMat4("projection", projection);
        shader->setMat4("view", view);

        shader->setVec3("lightPos", lPos);
        shader->setVec3("lightColor", lColor);
        shader->setVec3("viewPos", cPos);
        shader->setVec3("viewFront", cFront);

        glActiveTexture(TextureCount);
        glBindTexture(GL_TEXTURE_2D, TextureID);
        shader->setInt("uTexture", 0);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        glBindVertexArray(0);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void ModelComponent::destroyGL()
    {
        glDeleteBuffers(1, &NormalBuffer);
        glDeleteBuffers(1, &UVBuffer);
        glDeleteBuffers(1, &VertexBuffer);
        glDeleteVertexArrays(1, &VAO);
        glDeleteTextures(1, &TextureID);
        shader->Delete();

        delete shader;
    }
}