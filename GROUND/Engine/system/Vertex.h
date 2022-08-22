#pragma once

#include <glm/glm.hpp>
#include "Color.h"

namespace gr
{
	// basic vertex structure
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 color;
		glm::vec2 texCoords;
		glm::vec3 normal;
	};

	struct Texture {
		Texture(unsigned int id, std::string t) : ID(id), type(t) {}

		unsigned int ID;
		std::string type;
	};

	struct Light {
		glm::vec3 color;
		glm::vec3 position;
		glm::vec3 front;
		glm::vec3 cameraPos;
	};
}