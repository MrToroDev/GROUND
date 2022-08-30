#pragma once

#include <glm/glm.hpp>
#include "Shader.h"

namespace gr {
	class Ray
	{
	public:
		Ray(glm::vec3 startPosition, glm::vec3 endPosition, const char* v, const char* f);
		~Ray();

		void SetViewProjection(glm::mat4 p, glm::mat4 v);
		void Draw(glm::vec3 Color);

		glm::vec3 start, end;

	private:
		unsigned int VAO, VBO;
		Shader* shader;
		glm::mat4 _p, _v;
	};
}