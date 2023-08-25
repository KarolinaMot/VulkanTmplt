#pragma once
#include "Common.h"
#include "DescriptorSet.h"
#include "Model.h"
#include "Renderer.h"
#include "Transform.h"

using namespace glm;

class GameObject
{
	public:

		GameObject(string _name, Renderer* vulkan, Mesh* mesh, vec3 position, quat rotation, vec3 scale, DescriptorPool* pool);
		virtual ~GameObject();

		virtual void Update(float deltaTime, uint currentFrame);
		void Draw(Renderer* vulkan);
		virtual void GUIDetails();

		string GetName() { return name; }
		void SetName(string newName) { name = newName; }
		Transform* GetTransform() { return transform; }

	protected:

		string name;
		Mesh* mesh;
		Transform* transform;
		vector<DescriptorSet*> sets;
		GameObject* parent;
};

