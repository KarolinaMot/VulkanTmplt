#include "../Headers/GameObject.h"

GameObject::GameObject(std::string _name, Renderer* vulkan, Mesh* _mesh, vec3 position, quat rotation, vec3 scale, DescriptorPool* pool)
{
	mesh = _mesh;
	name = _name;
	transform = new Transform(vulkan, vulkan->GetModelSetLayout()->GetBinding(0), position, rotation, scale);

    sets.resize(vulkan->GetMaxFramesInFlight());
	for (int i = 0; i < vulkan->GetMaxFramesInFlight(); i++) {

		VkDescriptorBufferInfo bufferInfo = transform->GetUniform()->GetBufferInfo(i);
		VkDescriptorImageInfo textureInfo = mesh->GetMaterial()->diffuse->GetImageInfo(vulkan);
		VkDescriptorImageInfo textureInfo2 = mesh->GetMaterial()->specular->GetImageInfo(vulkan);
		VkDescriptorImageInfo textureInfo3 = mesh->GetMaterial()->normal->GetImageInfo(vulkan);
		sets[i] = new DescriptorSet(vulkan, pool, vulkan->GetModelSetLayout());
		sets[i]->AllocateSet();
		sets[i]->WriteBuffer(0, &bufferInfo);
		sets[i]->WriteImage(1, &textureInfo);
		sets[i]->WriteImage(2, &textureInfo2);
		sets[i]->WriteImage(3, &textureInfo3);
		sets[i]->WriteSet();

	}
}

GameObject::~GameObject()
{
    for (auto&& set : sets) {
        delete set;
    }

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

void GameObject::Draw(Renderer* vulkan)
{
	sets[vulkan->GetCurrentFrame()]->Bind(vulkan, vulkan->GetViewportPipelineLayout());
	mesh->Draw(vulkan);
}

void GameObject::GUIDetails()
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

    //if (Light* light = dynamic_cast<Light*>(objects[selectedObject])) {

    //    vec3 color = light->GetColor();
    //    vec3 direction = light->GetDirection();

    //    ImGui::Separator();
    //    ImGui::Text("Light parameters");
    //    ImGui::Text("Color");

    //    ImGui::DragFloat("Rotation x", &rotationVec.x, 0.5f, -360, 360);
    //    ImGui::DragFloat("Rotation Y", &rotationVec.y, 0.5f, -360, 360);
    //    ImGui::DragFloat("Rotation Z", &rotationVec.z, 0.5f, -360, 360);

    //}

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
}
