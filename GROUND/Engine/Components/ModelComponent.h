#pragma once

#include "../ECS.h"
#include "../graphics/Shader.h"
#include "TransformComponent.h"
#include "../system/Vertex.h"

namespace gr {
    class ModelComponent : public Component
    {
    private: // General...
        bool res;

    private: // Components
        TransformComponent* transform;

    private: // Texture, VAO, VBO, Matrix
        glm::mat4 projection, model, view;
        unsigned int NormalBuffer, UVBuffer, VertexBuffer, VAO, IndicesBuffer;
        std::vector< glm::vec3 > vertices;
        std::vector< glm::vec2 > uvs;
        std::vector< glm::vec3 > normals;
        std::vector< unsigned short > indices;
        std::vector< Texture > texture;

    private: // Lights attribute
        gr::Shader* shader;
        Light light;

    public:
        ModelComponent(const char* Modelpath, std::vector<Texture> textures,std::string Fshader, std::string Vshader);

        void SetProjectionView(glm::mat4 p, glm::mat4 v);
        void SetLight(Light light);

        gr::Shader* GetShader();
        unsigned int GetNormalBuffer();
        unsigned int GetUVBuffer();
        unsigned int GetVertexBuffer();

        void init() override;
        void draw() override;
        void destroyGL() override;
    };
}