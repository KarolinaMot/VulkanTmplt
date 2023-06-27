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

void GameObject::Update(uint currentFrame)
{
	static auto startTime = std::chrono::high_resolution_clock::now();
	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
	//mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//rotation = glm::rotate(rotation, time * glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//transform.rotation = angleAxis(time * glm::radians(90.0f), Common::GetWorldUp()) * defaultTransform.rotation;
	transform.rotation = glm::angleAxis(glm::radians(90.f), Common::GetWorldUp()) * transform.rotation;

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
