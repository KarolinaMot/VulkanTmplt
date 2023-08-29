#include "Skybox.h"

Skybox::Skybox(Renderer* vulkan, Camera* camera, DescriptorPool* pool, Model* model, string texturePath)
{
	auto skybox_layout = vulkan->GetSkyboxSetLayout();

	texture = new CubemapTexture(vulkan, texturePath, skybox_layout->GetBinding(1));
	cam = camera;
	cube = model;
	transform = new Transform(vulkan, skybox_layout->GetBinding(0), glm::vec3(0.0f, 0.f, 0.f), glm::quat(glm::vec3(0.f, 0.f, 0.f)), glm::vec3(1.f , 1.f, 1.f));

	cameraBuffer = new UniformBuffer(vulkan->GetDevice(), vulkan->GetCameraSetLayout()->GetBinding(0), vulkan->GetMaxFramesInFlight(), sizeof(VPMatrix));

	for (uint i = 0; i < vulkan->GetMaxFramesInFlight(); i++) {
		
		VkDescriptorBufferInfo bufferInfo = cameraBuffer->GetBufferInfo(i);
		VkDescriptorBufferInfo bufferInfo2 = transform->GetUniform()->GetBufferInfo(i);
		VkDescriptorImageInfo textureInfo = texture->GetImageInfo(vulkan);

		DescriptorSetBuilder skybox_set_builder(skybox_layout);

		skybox_set_builder
			.WriteBuffer(0, &bufferInfo)
			.WriteBuffer(1, &bufferInfo2)
			.WriteImage(2, &textureInfo);

		sets.emplace_back(skybox_set_builder.Build(vulkan->GetDevice(), vulkan->GetDescriptorPool()));

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
	delete transform;
	delete texture;
	delete cameraBuffer;
	
}

void Skybox::Update(float deltaTime, float currentFrame)
{
	VPMatrix matrix;
	matrix.proj = cam->GetVPM().proj;
	matrix.view = glm::mat4(glm::mat3(cam->GetVPM().view));
	cameraBuffer->SetBufferData(currentFrame, &matrix, sizeof(matrix));

	transform->UpdateMatrix(currentFrame);

}

void Skybox::Draw(Renderer* vulkan)
{
	//set[vulkan->GetCurrentFrame()]->Bind(vulkan, vulkan->GetBoxPipelineLayout());
	//cube->Draw(vulkan);

	sets[vulkan->GetCurrentFrame()]->Bind(vulkan->GetCommandBuffer(), vulkan->GetBoxPipelineLayout());
	//cameraSet[vulkan->GetCurrentFrame()]->Bind(vulkan, vulkan->GetBoxPipelineLayout());


	cube->Draw(vulkan);

}
