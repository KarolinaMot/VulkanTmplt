#pragma once
#include "../ImGUI/imgui.h"
#include "../ImGUI/imgui_internal.h"
#include "../ImGUI/imgui_impl_glfw.h"
#include "../ImGUI/imgui_impl_vulkan.h"
#include "../ImGUI/imgui_stdlib.h"
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <iostream>
#include <vulkan/vulkan.h>
#include <vulkan/vk_sdk_platform.h>
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include<glm/gtc/quaternion.hpp>
#include <glm/ext.hpp>
#include <vector>
#include <fstream>

typedef unsigned int uint;
typedef unsigned char uchar;

struct VPMatrix {
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
};

struct ModelMatrix {
	alignas(16) glm::mat4 model;
};


class Common
{
public:
	static glm::vec3 assimpToGlm(aiVector3D v) { return glm::vec3(v.x, v.y, v.z); }
	static glm::quat assimpToGlm(aiQuaternion quat) { return glm::quat(quat.w, quat.x, quat.y, quat.z); }
	static glm::vec3 GetWorldUp() { return glm::vec3(0.0f, 1.0f, 0.0f); }
	static glm::vec3 GetWorldForward() { return glm::vec3(0.f, 0.f, -1.f); }
    static std::vector<char> ReadShaderFile(const std::string& filename) {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            throw std::runtime_error("failed to open file!");
        }

        size_t fileSize = (size_t)file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();

        return buffer;
    };

};

namespace ProjectVariables {
	static glm::vec4 backgroundColor = glm::vec4(0.f, 0.f, 0.f, 1.f);
}

