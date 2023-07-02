#pragma once
#include "Common.h"
#include "DescriptorSet.h"
#include "Model.h"
#include "Vulkan.h"
#include "UniformBuffer.h"

using namespace glm;

class GameObject
{
	public:
		GameObject(std::string _name, Vulkan* vulkan, Model* mesh, vec3 position, quat rotation, vec3 scale, DescriptorPool* pool);
		~GameObject();

		void Update(float deltaTime, uint currentFrame);
		void Draw(Vulkan* vulkan);

		void Move(vec3 targetPosition);
		void Rotate(vec3 targetRotation);
		void Scale(vec3 targetScale);

	private:
		void UpdateModelMatrix(const Transform& transform, uint frame);

		std::string name;
		Model* model;
		Transform transform;
		Transform defaultTransform;

		DescriptorSet** set;
		UniformBuffer* uniform;

};

