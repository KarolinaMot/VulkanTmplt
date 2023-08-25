#pragma once
#include "Common.h"

struct Vertex {

	glm::vec3 pos = glm::vec3();
	glm::vec3 color = glm::vec3();
	glm::vec2 texCoord = glm::vec2();
	glm::vec3 norm = glm::vec3();
	glm::vec3 tangent = glm::vec3();
	glm::vec4 texMultisampling = glm::vec4();

	static VkVertexInputBindingDescription GetBindingDescription();
	static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();
	static VkVertexInputAttributeDescription GetVertexAttributeDescriptions();
};


