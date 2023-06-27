#include "../Headers/GameObject.h"

GameObject::GameObject(Vulkan* vulkan, Model* mesh, vec3 position, quat rotation, vec3 scale)
{
	model = mesh;
	transform.pos = position;
	transform.rotation = rotation;
	transform.scale = scale;
	defaultTransform = transform;
	model->UpdateModelMatrix(transform, vulkan->GetCurrentFrame());
}

GameObject::~GameObject()
{

}

void GameObject::Update(float deltaTime, uint currentFrame)
{
	static auto startTime = std::chrono::high_resolution_clock::now();
	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	transform.rotation = glm::angleAxis(deltaTime * glm::radians(90.f), Common::GetWorldUp()) * transform.rotation;

	model->UpdateModelMatrix(transform, currentFrame);

}

void GameObject::Draw(Vulkan* vulkan)
{
	model->Draw(vulkan);
}

void GameObject::Move(vec3 targetPosition)
{
	transform.pos = targetPosition;

}

void GameObject::Rotate(vec3 targetRotation)
{
	transform.rotation = quat(targetRotation);
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
