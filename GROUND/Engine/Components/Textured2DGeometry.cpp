#include "Textured2DGeometry.h"
#include "../GR_cross_definitions.h"
#include "../graphics/Shapes.h"
#include "../graphics/GraphicLoader.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../system/DeviceInfo.h"

namespace gr
{
    Textured2DGeometry::Textured2DGeometry(const char* file, const char* v, const char* f, Basic2DGeometryShapes shape)
        : _file(file), _shapes(shape)
    {
        this->_shader = new gr::Shader(v, f);
    }
    
    unsigned int Textured2DGeometry::GetBuffer()
    {
        return _vbo;
    }
    
    Shader* Textured2DGeometry::GetShader()
    {
        return _shader;
    }
    
    void Textured2DGeometry::init()
    {
        if (!entity->hasComponent<TransformComponent>())
            this->_transform = &entity->addComponent<TransformComponent>();
        else
            this->_transform = &entity->getComponent<TransformComponent>();

        _texture = gr::LoadTexture2D(this->_file);

        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);
        glGenBuffers(1, &_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        if (_shapes == Basic2DGeometryShapes::SQUARE) {
            glBufferData(GL_ARRAY_BUFFER, sizeof(gr::shapes2D::square), shapes2D::square, GL_STATIC_DRAW);
        } else {
            glBufferData(GL_ARRAY_BUFFER, sizeof(gr::shapes2D::triangle), shapes2D::triangle, GL_STATIC_DRAW);
        }

        gr::Log(std::string("Buffer size allocated: ").append(std::to_string(gr::GetMemsizeBuffer(_vbo))).append(" bytes").c_str());

        _shader->setVertexAttrib("aPos", 3, GL_FLOAT, sizeof(gr::Vertex), (void*)offsetof(gr::Vertex, position));
        _shader->setVertexAttrib("aColor", 3, GL_FLOAT, sizeof(gr::Vertex), (void*)offsetof(gr::Vertex, color));
        _shader->setVertexAttrib("aNormal", 3, GL_FLOAT, sizeof(gr::Vertex), (void*)offsetof(gr::Vertex, normal));
        _shader->setVertexAttrib("aTexcoord", 2, GL_FLOAT, sizeof(gr::Vertex), (void*)offsetof(gr::Vertex, texCoords));
    }
    
    void Textured2DGeometry::draw()
    {
        _shader->use();
        _m = glm::mat4(1.0);
        _m = glm::translate(_m, _transform->position);
        _m = glm::scale(_m, _transform->size);
        _m = glm::rotate(_m, _transform->angle, _transform->angleAxis);
        _shader->setMat4("model", _m);
        _shader->setInt("uTexture0", 0);
        glBindTexture(GL_TEXTURE_2D, _texture);
        glBindVertexArray(_vao);
        if (_shapes == Basic2DGeometryShapes::SQUARE) {
            glDrawArrays(GL_TRIANGLES, 0, 6);
        } else {
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    
    void Textured2DGeometry::destroyGL()
    {
        glDeleteVertexArrays(1, &_vao);
        glDeleteBuffers(1, &_vbo);
        glDeleteTextures(1, &_texture);
        _shader->Delete();
        delete _shader;
    }
}