#include "../Headers/GameObject.h"

GameObject::GameObject(std::string _name, Vulkan* vulkan, Model* mesh, vec3 position, quat rotation, vec3 scale, DescriptorPool* pool)
{
	model = mesh;
	transform.pos = position;
	transform.rotation = rotation;
	transform.scale = scale;
	defaultTransform = transform;
	name = _name;


	uniform = new UniformBuffer(vulkan, 0, 1, VK_SHADER_STAGE_VERTEX_BIT, vulkan->GetMaxFramesInFlight(), sizeof(ModelMatrix));
	set = new DescriptorSet * [vulkan->GetMaxFramesInFlight()];
	for (int i = 0; i < vulkan->GetMaxFramesInFlight(); i++) {
		VkDescriptorBufferInfo bufferInfo = uniform->GetBufferInfo(i);
		VkDescriptorImageInfo textureInfo = model->GetDiffuseTex()->GetImageInfo(vulkan);
		set[i] = new DescriptorSet(vulkan, pool, vulkan->GetModelSetLayout());
		set[i]->AllocateSet();
		set[i]->WriteBuffer(0, &bufferInfo);
		set[i]->WriteImage(1, &textureInfo);
		set[i]->WriteSet();
	}

	UpdateModelMatrix(transform, vulkan->GetCurrentFrame());
}

GameObject::~GameObject()
{
	delete[] set;
	delete uniform;
}

void GameObject::Update(float deltaTime, uint currentFrame)
{
	static auto startTime = std::chrono::high_resolution_clock::now();
	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	transform.rotation = glm::angleAxis(deltaTime * glm::radians(90.f), Common::GetWorldUp()) * transform.rotation;

	UpdateModelMatrix(transform, currentFrame);

}

void GameObject::Draw(Vulkan* vulkan)
{
	set[vulkan->GetCurrentFrame()]->Bind(vulkan);
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

void GameObject::UpdateModelMatrix(const Transform& transform, uint frame)
{
	ModelMatrix matrix;
	matrix.model = glm::translate(glm::mat4(1.f), transform.pos) * glm::mat4(transform.rotation) * glm::scale(glm::mat4(1.0f), transform.scale);

	uniform->SetBufferData(frame, &matrix, sizeof(matrix));
}
