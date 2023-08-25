#pragma once
#include "Common.h"

struct Vertex {

	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 texCoord;
	glm::vec3 norm;
	glm::vec3 tangent;

	static VkVertexInputBindingDescription GetBindingDescription();
	static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();
	static VkVertexInputAttributeDescription GetVertexAttributeDescriptions();
};


