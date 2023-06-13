#pragma once
#define GLFW_INCLUDE_VULKAN
#include <iostream>
//#define VK_USE_PLATFORM_XCB_KHR
#include <vulkan/vulkan.h>
#include <vulkan/vk_sdk_platform.h>
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext.hpp>

typedef unsigned int uint;
typedef unsigned char uchar;

class Common
{
public:
	static glm::vec3 assimpToGlm(aiVector3D v) { return glm::vec3(v.x, v.y, v.z); }
	static glm::quat assimpToGlm(aiQuaternion quat) { return glm::quat(quat.w, quat.x, quat.y, quat.z); }

};

