#pragma once

//Vulkan and GLFW
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

//ASSIMP
#include <assimp/Importer.hpp>      // C++ importer interface

//ImGui
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"
#include "imgui_stdlib.h"

//Math
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/ext.hpp>

//STD
#include <array>
#include <vector>
#include <fstream>
#include <string>
#include <optional>
#include <set>
#include <algorithm>
#include <iostream>
#include <memory>
#include <optional>
#include <set>
#include <filesystem>

using namespace std;

typedef uint32_t uint;
typedef unsigned char uchar;

typedef filesystem::path FilePath;

struct VPMatrix {
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
	alignas(16) glm::vec4 pos;
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
	static glm::vec3 GetWorldRight() { return glm::vec3(1.0f, 0.0f, 0.0f); }
	static glm::vec3 GetWorldForward() { return glm::vec3(0.f, 0.f, -1.f); }

};

namespace ProjectVariables {
	static glm::vec4 backgroundColor = glm::vec4(0.f, 0.f, 0.f, 1.f);
}

static void CHECK_VK(VkResult err)
{
	if (err == 0) return;

	fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
	if (err < 0) abort();
}
