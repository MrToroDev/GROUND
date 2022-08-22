#include "ModelComponent.h"
#include "../graphics/GraphicLoader.h"
#include "../GR_cross_definitions.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../system/Memory.h"

namespace gr {
    ModelComponent::ModelComponent(const char* Modelpath, std::vector<Texture> textures, std::string Fshader, std::string Vshader)
    {
        // Deprecated!
        //res = gr::loadOBJ(Modelpath, this->vertices, this->uvs, this->normals);
        
        res = gr::loadAssimp(Modelpath, indices, vertices, uvs, normals);
        this->shader = new gr::Shader(Vshader.c_str(), Fshader.c_str());
        this->texture = textures;
    }

    void ModelComponent::SetProjectionView(glm::mat4 p, glm::mat4 v)
    {
        this->projection = p;
        this->view = v;
    }

    void ModelComponent::SetLight(Light light)
    {
        this->light = light;
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
            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);

            gr::Log("-- Creating model component --");

            glGenBuffers(1, &VertexBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
             
            gr::Log(std::string("Vertex Buffer size allocated: ").append(std::to_string(gr::Memory::GetBufferSize(VertexBuffer))).append(" bytes").c_str());

            glGenBuffers(1, &UVBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, UVBuffer);
            glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

            gr::Log(std::string("UV Buffer size allocated: ").append(std::to_string(gr::Memory::GetBufferSize(UVBuffer))).append(" bytes").c_str());

            glGenBuffers(1, &NormalBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, NormalBuffer);
            glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

            gr::Log(std::string("Normal Buffer size allocated: ").append(std::to_string(gr::Memory::GetBufferSize(NormalBuffer))).append(" bytes").c_str());

            glGenBuffers(1, &IndicesBuffer);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndicesBuffer);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

            glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
            shader->setVertexAttrib("aPos", 3, GL_FLOAT, 0, (void*)0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glBindBuffer(GL_ARRAY_BUFFER, UVBuffer);
            shader->setVertexAttrib("aUV", 2, GL_FLOAT, 0, (void*)0);
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

        shader->setVec3("lightColor", light.color);
        shader->setVec3("lightPos", light.position);
        shader->setVec3("viewPos", light.cameraPos);
        shader->setVec3("viewFront", light.front);

        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;

        for (unsigned int i = 0; i < texture.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            std::string number;
            std::string name = texture[i].type;

            if (name  == "diffuse") {
                number = std::to_string(diffuseNr++);
            }
            else if (name == "specular") {
                number = std::to_string(specularNr++);
            }
            else if (name == "normal") {
                number = std::to_string(normalNr++);
            }

            shader->setInt("texture_" + name + number, i);

            glBindTexture(GL_TEXTURE_2D, texture[i].ID);
        }
        glActiveTexture(GL_TEXTURE0);
        
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndicesBuffer);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, 0);
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void ModelComponent::destroyGL()
    {
        glDeleteBuffers(1, &NormalBuffer);
        glDeleteBuffers(1, &UVBuffer);
        glDeleteBuffers(1, &VertexBuffer);
        glDeleteBuffers(1, &IndicesBuffer);
        glDeleteVertexArrays(1, &VAO);
        for (auto e : texture) {
            glDeleteTextures(1, &e.ID);
        }
        shader->Delete();

        delete shader;
    }
}