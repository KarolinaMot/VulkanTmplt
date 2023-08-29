#pragma once
#include "Common.h"
#include "GameObject.h"
#include "Renderer.h"
#include "DescriptorSet.h"
#include "UniformBuffer.h"

struct LightInfo {
	alignas(16) vec3 pos;
	alignas(16) vec3 direction;
	alignas(16) vec4 color;
	alignas(16) float radius;
	alignas(16) int type;
};

enum class LightType {
	DIRECTIONAL_LIGHT = 0,
	POINT_LIGHT = 1
};

class Light : public GameObject
{
public:

	Light(
		Renderer* vulkan,
		string name,
		DescriptorPool* pool,
		Mesh* mesh,
		vec3 position,
		vec4 color,
		vec3 direction
	);

	Light(Renderer* vulkan,
		string name,
		DescriptorPool* pool,
		Mesh* mesh,
		vec3 position,
		vec4 color,
		vec3 direction,
		float distance
	);

	virtual ~Light();

	void Update(float deltaTime, uint currentFrame);

	void Bind(Renderer* vulkan);

	void GUIDetails();

	glm::vec3 GetColor() { return info.color; }
	glm::vec3 GetDirection() { return info.direction; }

private:

	vector<unique_ptr<DescriptorSet>> sets;

	UniformBuffer* buffer;
	LightInfo info;

};


