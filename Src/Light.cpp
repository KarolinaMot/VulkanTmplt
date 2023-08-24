#include "Light.h"

Light::Light(Renderer* vulkan, string name, DescriptorPool* pool, Mesh* mesh, vec3 position, vec4 color, vec3 direction)
	: GameObject(name, vulkan, mesh, position, glm::quat(), glm::vec3(0.5f), pool) 
{
	
	info.pos = position;
	info.direction = direction;
	info.color = color;
	info.type = (int)LightType::DIRECTIONAL_LIGHT;

	buffer = new UniformBuffer(vulkan->GetDevice(), vulkan->GetLightSetLayout()->GetBinding(0), vulkan->GetMaxFramesInFlight(), sizeof(LightInfo));

	sets.resize(vulkan->GetMaxFramesInFlight());
	for (int i = 0; i < vulkan->GetMaxFramesInFlight(); i++) {
		VkDescriptorBufferInfo bufferInfo = buffer->GetBufferInfo(i);
		sets[i] = new DescriptorSet(vulkan, pool, vulkan->GetLightSetLayout());
		sets[i]->AllocateSet();
		sets[i]->WriteBuffer(0, &bufferInfo);
		sets[i]->WriteSet();
	}

}

Light::Light(Renderer* vulkan, string name, DescriptorPool* pool, Mesh* mesh, vec3 position, vec4 color, vec3 direction, float distance)
	: GameObject(name, vulkan, mesh, position, glm::quat(), glm::vec3(0.5f), pool)
{
	info.pos = position;
	info.direction = direction;
	info.color = color;
	info.type = (int)LightType::POINT_LIGHT;
	info.radius = distance;

	buffer = new UniformBuffer(vulkan->GetDevice(), vulkan->GetLightSetLayout()->GetBinding(0), vulkan->GetMaxFramesInFlight(), sizeof(LightInfo));

	sets.resize(vulkan->GetMaxFramesInFlight());

	for (int i = 0; i < vulkan->GetMaxFramesInFlight(); i++) {
		VkDescriptorBufferInfo bufferInfo = buffer->GetBufferInfo(i);
		sets[i] = new DescriptorSet(vulkan, pool, vulkan->GetLightSetLayout());
		sets[i]->AllocateSet();
		sets[i]->WriteBuffer(0, &bufferInfo);
		sets[i]->WriteSet();
	}

}

Light::~Light()
{
	delete buffer;

	for (auto&& set : sets) {
		delete set;
	}
}

void Light::Update(float deltaTime, uint currentFrame)
{
	info.pos = transform->GetPosition();
	buffer->SetBufferData(currentFrame, &info, sizeof(info));
	transform->UpdateMatrix(currentFrame);
}

void Light::Bind(Renderer* vulkan)
{
	sets[vulkan->GetCurrentFrame()]->Bind(vulkan, vulkan->GetViewportPipelineLayout());
}

void Light::GUIDetails()
{
	vec3 position = transform->GetPosition();
	quat rotation = transform->GetRotation();
	vec3 rotationVec = glm::degrees(glm::eulerAngles(rotation));
	vec3 scale = transform->GetScale();

	if (rotationVec.x == -0.f) rotationVec.x *= -1;
	if (rotationVec.y == -0.f) rotationVec.y *= -1;
	if (rotationVec.z == -0.f) rotationVec.z *= -1;
	if (rotationVec.x == -180.f) rotationVec.x = 180.f;
	if (rotationVec.y == -180.f) rotationVec.y = 180.f;
	if (rotationVec.z == -180.f) rotationVec.z = 180.f;

	ImGui::Begin("Details");

	ImGui::SameLine();
	ImGui::InputText("Name##1", &name);
	ImGui::Separator();
	ImGui::Text("Transform");
	ImGui::DragFloat("Position x", &position.x, 0.5f, -FLT_MAX, +FLT_MAX);
	ImGui::DragFloat("Position Y", &position.y, 0.5f, -FLT_MAX, +FLT_MAX);
	ImGui::DragFloat("Position Z", &position.z, 0.5f, -FLT_MAX, +FLT_MAX);

	ImGui::DragFloat("Rotation x", &rotationVec.x, 0.5f, -360, 360);
	ImGui::DragFloat("Rotation Y", &rotationVec.y, 0.5f, -360, 360);
	ImGui::DragFloat("Rotation Z", &rotationVec.z, 0.5f, -360, 360);

	ImGui::DragFloat("Scale x", &scale.x, 0.01f, -FLT_MAX, +FLT_MAX);
	ImGui::DragFloat("Scale Y", &scale.y, 0.01f, -FLT_MAX, +FLT_MAX);
	ImGui::DragFloat("Scale Z", &scale.z, 0.01f, -FLT_MAX, +FLT_MAX);

	ImGui::Separator();
	ImGui::Text("Light parameters");
	ImGui::Text("Color");
	ImGui::DragFloat("R", &info.color.x, 0.001f, 0, 1);
	ImGui::DragFloat("G", &info.color.y, 0.001f, 0, 1);
	ImGui::DragFloat("B", &info.color.z, 0.001f, 0, 1);

	ImGui::Text("Direction");
	ImGui::DragFloat("Direction X", &info.direction.x, 0.01f, -FLT_MAX, +FLT_MAX);
	ImGui::DragFloat("Direction Y", &info.direction.y, 0.01f, -FLT_MAX, +FLT_MAX);
	ImGui::DragFloat("Direction Z", &info.direction.z, 0.01f, -FLT_MAX, +FLT_MAX);

	ImGui::End();

	if (rotationVec.x == -0.f) rotationVec.x *= -1;
	if (rotationVec.y == -0.f) rotationVec.y *= -1;
	if (rotationVec.z == -0.f) rotationVec.z *= -1;
	if (rotationVec.x == -180.f) rotationVec.x = 180.f;
	if (rotationVec.y == -180.f) rotationVec.y = 180.f;
	if (rotationVec.z == -180.f) rotationVec.z = 180.f;

	transform->Move(position);
	transform->Rotate(quat(glm::radians(rotationVec)));
	transform->Scale(scale);
	info.direction = glm::normalize(info.direction);
}

