#include "../Headers/GameObject.h"

GameObject::GameObject(Vulkan* vulkan, Model* mesh, vec3 position, quat rotation, vec3 scale)
{
	model = mesh;
	transform.pos = position;
	//transform.rotation = rotation;
	transform.scale = scale;



}

GameObject::~GameObject()
{

}

void GameObject::Update(Vulkan* vulkan)
{
}

void GameObject::Draw(Vulkan* vulkan)
{

}

void GameObject::Move(vec3 targetPosition)
{
	transform.pos = targetPosition;

}

void GameObject::Rotate(vec3 targetRotation)
{
	//transform.rotation = quat(targetRotation);
}

void GameObject::Scale(vec3 targetScale)
{
	transform.scale = targetScale;
}

void GameObject::UpdateDescriptor()
{

}

void GameObject::UpdateMatrix()
{
}
