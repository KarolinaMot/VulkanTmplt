#pragma once
#include "Common.h"
#include "DescriptorSet.h"
#include "Model.h"
#include "Vulkan.h"
#include "Transform.h"

using namespace glm;

class GameObject
{
	public:
		GameObject(std::string _name, Vulkan* vulkan, Model* mesh, vec3 position, quat rotation, vec3 scale, DescriptorPool* pool);
		~GameObject();

		void Update(float deltaTime, uint currentFrame);
		void Draw(Vulkan* vulkan);

		std::string GetName() { return name; }

	private:

		std::string name;
		Model* model;
		Transform* transform;
		DescriptorSet** set;




};

