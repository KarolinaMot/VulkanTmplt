#pragma once
#include "Common.h"
#include "GLFW_Window.h"
#include "DescriptorSet.h"
#include <glm/gtx/rotate_vector.hpp>
#include "UniformBuffer.h"

struct CameraVectors {
	alignas(16) vec3 forwards;
	alignas(16) vec3 right;
	alignas(16) vec3 up;
};

class Camera
{
public:
	Camera(Renderer* vulkan, DescriptorPool* pool, glm::vec3 _pos, glm::vec2 _pans, float _fov, float _speed, float _sensitivity, uint _scrW, uint _scrH);
	~Camera();
	void Update(float deltaTime, int currentFrame, float scrW, float scrH);
	void ControlInputs(Inputs* inputs, float deltaTime);
	void Bind(Renderer* vulkan);
	void CalculateOrientation();
	glm::vec3 PickingDirection(glm::vec2 pos);
	CameraVectors& GetVectors() { return vectors; }
	VPMatrix GetVPM() { return matrix; }

	uint GetHeight() { return scrH; }
	uint GetWidth() { return scrW; }


	void UpdateMatrix(int currentFrame);
	float GetNearPlane() { return planes.x; }
	float GetFarPlane() { return planes.y; }
	glm::vec3 GetPosition() { return position; }

private:
	glm::vec3 position;
	glm::quat rotation = glm::quat(1, glm::vec3(0));
	glm::vec2 rotationAngles;
	glm::vec3 vel;

	CameraVectors vectors;

	//glm::vec3 up = Common::GetWorldUp();
	//glm::vec3 forward = Common::GetWorldForward();

	glm::vec2 planes;
	float fov;
	float sensitivity = 0.2f;
	float speed = 0.025f;
	uint scrW, scrH;

	glm::vec2 lastMousePos;

	VPMatrix matrix;
	DescriptorSet** cameraDescriptorSet;
	UniformBuffer* cameraBuffer;
};

