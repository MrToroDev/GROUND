#include "Ray.h"
#include "../GR_cross_definitions.h"
#include <glm/gtc/matrix_transform.hpp>

gr::Ray::Ray(glm::vec3 startPosition, glm::vec3 endPosition, const char* v, const char* f)
{
	start = startPosition;
	end = endPosition;
	shader = new Shader(v, f);
	float vertices[] = {
		startPosition.x, startPosition.y, startPosition.z,
		endPosition.x, endPosition.y, endPosition.z
	};
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	shader->setVertexAttrib("aPos", 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

gr::Ray::~Ray()
{
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	shader->Delete();
	delete shader;
}

void gr::Ray::SetViewProjection(glm::mat4 p, glm::mat4 v)
{
	_p = p;
	_v = v;
}

void gr::Ray::Draw(glm::vec3 Color)
{
	shader->use();
	shader->setMat4("projection", _p);
	shader->setMat4("view", _v);
	shader->setVec3("uColor", Color);

	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0);
}