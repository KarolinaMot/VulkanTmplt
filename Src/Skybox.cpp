#include "../Headers/Skybox.h"

Skybox::Skybox(Vulkan* vulkan, DescriptorPool* pool, Model* model, std::string texturePath)
{
	transform = new Transform(vulkan, vulkan->GetSkyboxSetLayout()->GetBinding(0), glm::vec3(), quat(glm::vec3()), glm::vec3());
	texture = new CubemapTexture(vulkan, texturePath, vulkan->GetSkyboxSetLayout()->GetBinding(1));
	cube = model;

	set = new DescriptorSet * [vulkan->GetMaxFramesInFlight()];
	for (int i = 0; i < vulkan->GetMaxFramesInFlight(); i++) {
		VkDescriptorBufferInfo bufferInfo = transform->GetUniform()->GetBufferInfo(i);		
		VkDescriptorImageInfo imageInfo = texture->GetImageInfo(vulkan);
		set[i] = new DescriptorSet(vulkan, pool, vulkan->GetSkyboxSetLayout());
		set[i]->AllocateSet();
		set[i]->WriteBuffer(0, &bufferInfo);
		set[i]->WriteImage(1, &imageInfo);
		set[i]->WriteSet();
	}
}

Skybox::~Skybox()
{
	delete transform;
	delete texture;
	delete[] set;
}

void Skybox::Update(float deltaTime, float currentFrame)
{
	transform->UpdateMatrix(currentFrame);
}

void Skybox::Draw(Vulkan* vulkan)
{
	set[vulkan->GetCurrentFrame()]->Bind(vulkan, vulkan->GetSkyboxPipelineLayout());
	cube->Draw(vulkan);
}
