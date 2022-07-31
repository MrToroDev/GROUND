#include "Basic3DGeometry.h"
#include "../GR_cross_definitions.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../graphics/Shapes.h"
#include "../system/DeviceInfo.h"

namespace gr {
    Basic3DGeometry::Basic3DGeometry(Basic3DGeometryShapes s, std::string Fshader, std::string Vshader)
    {
        this->shader = new gr::Shader(Vshader.c_str(), Fshader.c_str());
        types = s;
    }

    unsigned int Basic3DGeometry::GetBuffer() const
    {
        return this->VBO;
    }

    gr::Shader* Basic3DGeometry::GetShader() const
    {
        return shader;
    }

    void Basic3DGeometry::SetProjectionView(glm::mat4 p, glm::mat4 v)
    {
        this->proj = p;
        this->view = v;
    }

    void Basic3DGeometry::SetLightAttribute(glm::vec3 color, glm::vec3 pos, glm::vec3 camPos, glm::vec3 camFront)
    {
        lColor = color;
        lPos = pos;
        cPos = camPos;
        cFront = camFront;
    }

    void Basic3DGeometry::init()
    {
        if (!entity->hasComponent<TransformComponent>()) {
            entity->addComponent<TransformComponent>();
        }
        else { transform = &entity->getComponent<TransformComponent>(); }

        glGenVertexArrays(1, &this->VAO);
        glBindVertexArray(this->VAO);

        glGenBuffers(1, &this->VBO);
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

        switch (types)
        {
        case Basic3DGeometryShapes::CUBE:
            glBufferData(GL_ARRAY_BUFFER, sizeof(shapes3D::cube), shapes3D::cube, GL_STATIC_DRAW);
            break;
        case Basic3DGeometryShapes::PYRAMID:
            glBufferData(GL_ARRAY_BUFFER, sizeof(shapes3D::pyramid), shapes3D::pyramid, GL_STATIC_DRAW);
            break;
        }

        gr::Log(std::string("Buffer size allocated: ").append(std::to_string(gr::GetMemsizeBuffer(VBO))).append(" bytes").c_str());

        shader->setVertexAttrib("aPos", 3, GL_FLOAT, sizeof(gr::Vertex), (void*)offsetof(gr::Vertex, position));
        shader->setVertexAttrib("aColor", 3, GL_FLOAT, sizeof(gr::Vertex), (void*)offsetof(gr::Vertex, color));
        shader->setVertexAttrib("aNormal", 3, GL_FLOAT, sizeof(gr::Vertex), (void*)offsetof(gr::Vertex, normal));
        shader->setVertexAttrib("aTexcoord", 2, GL_FLOAT, sizeof(gr::Vertex), (void*)offsetof(gr::Vertex, texCoords));
    }

    void Basic3DGeometry::draw()
    {
        shader->use();
        shader->setMat4("projection", proj);
        shader->setMat4("view", view);
        shader->setVec3("lightColor", lColor);
        shader->setVec3("lightPos", lPos);
        shader->setVec3("viewPos", cPos);
        shader->setVec3("viewFront", cFront);

        glBindVertexArray(this->VAO);

        model = glm::mat4(1.0f);
        model = glm::scale(model, transform->size);
        model = glm::translate(model, transform->position);
        model = glm::rotate(model, transform->angle, transform->angleAxis);
        shader->setMat4("model", model);

        switch (types)
        {
        case Basic3DGeometryShapes::CUBE:
            glDrawArrays(GL_TRIANGLES, 0, 36);   
            break;
        case Basic3DGeometryShapes::PYRAMID:
            glDrawArrays(GL_TRIANGLES, 0, 18);
            break;
        }
    }

    void Basic3DGeometry::destroyGL()
    {
        glDeleteBuffers(1, &this->VBO);
        glDeleteVertexArrays(1, &this->VAO);
        shader->Delete();
    }
}