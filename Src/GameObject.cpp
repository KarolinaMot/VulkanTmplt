#include "../Headers/GameObject.h"

GameObject::GameObject(std::string _name, Vulkan* vulkan, Model* mesh, vec3 position, quat rotation, vec3 scale, DescriptorPool* pool)
{
	model = mesh;
	name = _name;
	transform = new Transform(vulkan, vulkan->GetModelSetLayout()->GetBinding(0), position, rotation, scale);

	set = new DescriptorSet * [vulkan->GetMaxFramesInFlight()];
	for (int i = 0; i < vulkan->GetMaxFramesInFlight(); i++) {
		VkDescriptorBufferInfo bufferInfo = transform->GetUniform()->GetBufferInfo(i);
		VkDescriptorImageInfo textureInfo = model->GetDiffuseTex()->GetImageInfo(vulkan);
		set[i] = new DescriptorSet(vulkan, pool, vulkan->GetModelSetLayout());
		set[i]->AllocateSet();
		set[i]->WriteBuffer(0, &bufferInfo);
		set[i]->WriteImage(1, &textureInfo);
		set[i]->WriteSet();
	}
}

GameObject::~GameObject()
{
	delete[] set;
	delete transform;
}

void GameObject::Update(float deltaTime, uint currentFrame)
{
	static auto startTime = std::chrono::high_resolution_clock::now();
	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	transform->Rotate(glm::angleAxis(deltaTime * glm::radians(90.f), Common::GetWorldUp()) * transform->GetRotation());

	transform->UpdateMatrix(currentFrame);

}

void GameObject::Draw(Vulkan* vulkan)
{
	set[vulkan->GetCurrentFrame()]->Bind(vulkan, vulkan->GetViewportPipelineLayout());
	model->Draw(vulkan);
}
