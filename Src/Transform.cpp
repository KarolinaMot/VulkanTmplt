#include "../Headers/Transform.h"

Transform::Transform(Vulkan* vulkan, VkDescriptorSetLayoutBinding binding, vec3 position, quat _rotation, vec3 _scale)
{
	pos = position;
	rotation = _rotation;
	scale = _scale;
	uniform = new UniformBuffer(vulkan, binding,  vulkan->GetMaxFramesInFlight(), sizeof(ModelMatrix));
	UpdateMatrix(vulkan->GetCurrentFrame());
}

void Transform::UpdateMatrix(uint frame)
{
	ModelMatrix matrix;
	matrix.model = glm::translate(glm::mat4(1.f), pos) * glm::mat4(rotation) * glm::scale(glm::mat4(1.0f), scale);

	uniform->SetBufferData(frame, &matrix, sizeof(matrix));
}
