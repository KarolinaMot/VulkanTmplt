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
		GameObject(std::string _name, Vulkan* vulkan, Mesh* mesh, vec3 position, quat rotation, vec3 scale, DescriptorPool* pool);
		~GameObject();

		void Update(float deltaTime, uint currentFrame);
		void Draw(Vulkan* vulkan);

		std::string GetName() { return name; }
		void SetName(std::string newName) { name = newName; }
		Transform* GetTransform() { return transform; }

	private:

		std::string name;
		Mesh* mesh;
		Transform* transform;
		DescriptorSet** set;
		GameObject* parent;
};

