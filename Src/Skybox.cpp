#include "../Headers/Skybox.h"

Skybox::Skybox(Vulkan* vulkan, Camera* camera, DescriptorPool* pool, Model* model, std::string texturePath)
{
	texture = new CubemapTexture(vulkan, texturePath, vulkan->GetSkyboxSetLayout()->GetBinding(1));
	cam = camera;
	cube = model;
	transform = new Transform(vulkan, vulkan->GetSkyboxSetLayout()->GetBinding(0), glm::vec3(0.0f, 0.f, 0.f), glm::quat(glm::vec3(0.f, 0.f, 0.f)), glm::vec3(1.f , 1.f, 1.f));

	cameraBuffer = new UniformBuffer(vulkan, vulkan->GetCameraSetLayout()->GetBinding(0), vulkan->GetMaxFramesInFlight(), sizeof(VPMatrix));

	set = new DescriptorSet * [vulkan->GetMaxFramesInFlight()];
	for (int i = 0; i < vulkan->GetMaxFramesInFlight(); i++) {
		VkDescriptorBufferInfo bufferInfo = cameraBuffer->GetBufferInfo(i);
		VkDescriptorBufferInfo bufferInfo2 = transform->GetUniform()->GetBufferInfo(i);
		VkDescriptorImageInfo textureInfo = texture->GetImageInfo(vulkan);
		set[i] = new DescriptorSet(vulkan, pool, vulkan->GetSkyboxSetLayout());
		set[i]->AllocateSet();
		set[i]->WriteBuffer(0, &bufferInfo);
		set[i]->WriteBuffer(1, &bufferInfo2);
		set[i]->WriteImage(2, &textureInfo);
		set[i]->WriteSet();
	}



	VPMatrix matrix;
	matrix.proj = cam->GetVPM().proj;
	matrix.view = cam->GetVPM().view;
	cameraBuffer->SetBufferData(vulkan->GetCurrentFrame(), &matrix, sizeof(matrix));



	//for (int i = 0; i < vulkan->GetMaxFramesInFlight(); i++) {
	//	VkDescriptorBufferInfo bufferInfo = buffer->GetBufferInfo(i);
	//	VkDescriptorImageInfo imageInfo = texture->GetImageInfo(vulkan);
	//	set[i] = new DescriptorSet(vulkan, pool, vulkan->GetSkyboxSetLayout());
	//	set[i]->AllocateSet();
	//	set[i]->WriteBuffer(0, &bufferInfo);
	//	set[i]->WriteImage(1, &imageInfo);
	//	set[i]->WriteSet();
	//}
}

Skybox::~Skybox()
{
	//delete buffer;
	delete transform;
	delete texture;
	delete[] set;
}

void Skybox::Update(float deltaTime, float currentFrame)
{
	VPMatrix matrix;
	matrix.proj = cam->GetVPM().proj;
	matrix.view = glm::mat4(glm::mat3(cam->GetVPM().view));
	cameraBuffer->SetBufferData(currentFrame, &matrix, sizeof(matrix));

	transform->UpdateMatrix(currentFrame);

}

void Skybox::Draw(Vulkan* vulkan)
{
	//set[vulkan->GetCurrentFrame()]->Bind(vulkan, vulkan->GetBoxPipelineLayout());
	//cube->Draw(vulkan);

	set[vulkan->GetCurrentFrame()]->Bind(vulkan, vulkan->GetBoxPipelineLayout());
	//cameraSet[vulkan->GetCurrentFrame()]->Bind(vulkan, vulkan->GetBoxPipelineLayout());


	cube->Draw(vulkan);

}
