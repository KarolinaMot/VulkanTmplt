#pragma once
#include "Common.h"
#include "DescriptorSet.h"
#include "Model.h"
#include "Vulkan.h"
#include "UniformBuffer.h"

using namespace glm;
struct Transform {
	vec3 pos;
	quat rotation;
	vec3 scale;
};
class GameObject
{
	public:
		GameObject(Vulkan* vulkan, Model* mesh, vec3 position, quat rotation, vec3 scale);
		~GameObject();

		void Update(Vulkan* vulkan);
		void Draw(Vulkan* vulkan);

		void Move(vec3 targetPosition);
		void Rotate(vec3 targetRotation);
		void Scale(vec3 targetScale);

	private:
		void UpdateDescriptor();
		void UpdateMatrix();

		Model* model;
		Transform transform;

};

