#include "../Headers/Camera.h"

Camera::Camera(Vulkan* vulkan, DescriptorPool* pool, glm::vec3 _pos, glm::vec2 _pans, float _fov, float _speed, float _sensitivity, uint _scrW, uint _scrH)
{

	cameraBuffer = new UniformBuffer(vulkan, vulkan->GetCameraSetLayout()->GetBinding(0), vulkan->GetMaxFramesInFlight(), sizeof(VPMatrix));
	cameraDescriptorSet = new DescriptorSet * [vulkan->GetMaxFramesInFlight()];
	for (int i = 0; i < vulkan->GetMaxFramesInFlight(); i++) {
		VkDescriptorBufferInfo info = cameraBuffer->GetBufferInfo(i);
		cameraDescriptorSet[i] = new DescriptorSet(vulkan, pool, vulkan->GetCameraSetLayout());
		cameraDescriptorSet[i]->AllocateSet();
		cameraDescriptorSet[i]->WriteBuffer(cameraBuffer->GetLayoutBinding().binding, &info);
		cameraDescriptorSet[i]->WriteSet();
	}


	position = _pos;
	planes = _pans;
	speed = _speed;
	sensitivity = _sensitivity;
	fov = glm::radians(_fov);
	scrW = _scrW;
	scrH = _scrH;
	float  aspect = (float)scrW / (float)scrH;

	CalculateOrientation();
	UpdateMatrix(vulkan->GetCurrentFrame());

	//matrix.proj = glm::perspective(fov, aspect, planes.x, planes.y);
	//matrix.proj[1][1] *= -1;
	//matrix.view = glm::lookAt(position, position + CalculateOrientation(), up);
}

Camera::~Camera()
{
	delete cameraBuffer;
	delete[] cameraDescriptorSet;
}

void Camera::Update(float deltaTime, int currentFrame, float _scrW, float _scrH)
{
	position += vel * deltaTime;
	rotation = glm::angleAxis(rotationAngles.x, Common::GetWorldUp());
	rotation = glm::angleAxis(rotationAngles.y, cross(Common::GetWorldUp(), vectors.forwards)) * rotation;
	scrW = _scrW;
	scrH = _scrH;
	UpdateMatrix(currentFrame);

}

void Camera::ControlInputs(Inputs* inputs, float deltaTime)
{
	if (glm::length(lastMousePos - inputs->mousePos) > 0.1 && inputs->rightMousePress)
	{
		glm::vec2 rotAmount = lastMousePos - inputs->mousePos;
		rotationAngles += glm::vec2(rotAmount.x, -rotAmount.y) * sensitivity * deltaTime;
		rotationAngles.y = glm::clamp(rotationAngles.y, -glm::pi<float>() * 0.48f, glm::pi<float>() * 0.48f);
	}
	lastMousePos = inputs->mousePos;
	CalculateOrientation();

	vel = glm::vec3(0.f);
	//glm::vec3 forward = { orientation.x,  orientation.y,  orientation.z};

	if (inputs->w)		vel += speed * vectors.forwards;
	if (inputs->s)		vel += speed * -vectors.forwards;

	if (inputs->a)		vel += speed * -normalize(cross(vectors.forwards, Common::GetWorldUp()));
	if (inputs->d)		vel += speed * normalize(cross(vectors.forwards, Common::GetWorldUp()));

	if (inputs->e)		vel += speed * Common::GetWorldUp();
	if (inputs->q)		vel += speed * -Common::GetWorldUp();
}

void Camera::Bind(Vulkan* vulkan)
{
	cameraDescriptorSet[vulkan->GetCurrentFrame()]->Bind(vulkan, vulkan->GetViewportPipelineLayout());
}

void Camera::CalculateOrientation()
{
	vectors.forwards = glm::mat3_cast(rotation) * Common::GetWorldForward();
	vectors.right = glm::mat3_cast(rotation) * Common::GetWorldRight();
	vectors.up = glm::mat3_cast(rotation) * Common::GetWorldUp();
}

glm::vec3 Camera::PickingDirection(glm::vec2 pos)
{
	// these positions must be in range [-1, 1] (!!!), not [0, width] and [0, height]
	float mouseX = pos.x / ((float)scrW * 0.5f) - 1.0f;
	float mouseY = pos.y / ((float)scrH * 0.5f) - 1.0f;

	glm::mat4 invVP = glm::inverse(matrix.proj);
	glm::vec4 screenPos = glm::vec4(mouseX, mouseY, 1.0f, 1.0f);
	glm::vec4 worldPos = invVP * screenPos;


	glm::vec3 dir = glm::mat3_cast(rotation) * glm::vec3(-worldPos);

	return glm::normalize(dir);
}

void Camera::UpdateMatrix(int currentFrame)
{
	float  aspect = (float)scrW / (float)scrH;

	matrix.proj = glm::perspective(fov, aspect, planes.x, planes.y);
	matrix.proj[1][1] *= -1;
	matrix.view = glm::lookAt(position, position + vectors.forwards, Common::GetWorldUp());
	matrix.pos = vec4(position, 0.f);
	cameraBuffer->SetBufferData(currentFrame, &matrix, sizeof(matrix));
}
