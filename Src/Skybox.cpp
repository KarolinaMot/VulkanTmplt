#include "../Headers/Skybox.h"

Skybox::Skybox(Vulkan* vulkan, Camera* camera, DescriptorPool* pool, Model* model, std::string texturePath)
{
	texture = new CubemapTexture(vulkan, texturePath, vulkan->GetSkyboxSetLayout()->GetBinding(1));
	cube = model;
	buffer = new UniformBuffer(vulkan, vulkan->GetSkyboxSetLayout()->GetBinding(0), vulkan->GetMaxFramesInFlight(), sizeof(CameraVectors));
	cam = camera;

	set = new DescriptorSet * [vulkan->GetMaxFramesInFlight()];
	for (int i = 0; i < vulkan->GetMaxFramesInFlight(); i++) {
		VkDescriptorBufferInfo bufferInfo = buffer->GetBufferInfo(i);
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
	delete buffer;
	delete texture;
	delete[] set;
}

void Skybox::Update(float deltaTime, float currentFrame)
{
	//transform->UpdateMatrix(currentFrame);
	buffer->SetBufferData(currentFrame, &cam->GetVectors(), sizeof(cam->GetVectors()));

}

void Skybox::Draw(Vulkan* vulkan)
{
	set[vulkan->GetCurrentFrame()]->Bind(vulkan, vulkan->GetSkyboxPipelineLayout());
	cube->Draw(vulkan);
}
