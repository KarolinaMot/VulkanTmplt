#pragma once
#include "Common.h"
#include "DescriptorSet.h"
#include "UniformBuffer.h"
#include "Vulkan.h"

using namespace glm;

class Transform
{
public:
	Transform(Vulkan* vulkan, vec3 position, quat rotation, vec3 scale);
	~Transform() { delete uniform; }
	
	vec3 GetPosition() { return pos; }
	quat GetRotation() { return rotation; }
	vec3 GetScale() { return scale; }
	UniformBuffer* GetUniform() { return uniform; }

	void Move(vec3 targetPosition) {pos = targetPosition;};
	void Rotate(vec3 targetRotation) { rotation = quat(targetRotation);};
	void Rotate(quat targetRotation) {rotation = targetRotation;};
	void Scale(vec3 targetScale) {scale = targetScale;};

	void UpdateMatrix(uint frame);
private:
	vec3 pos;
	quat rotation;
	vec3 scale;

	UniformBuffer* uniform;

};

