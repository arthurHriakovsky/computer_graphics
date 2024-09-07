#pragma once
#define _USE_MATH_DEFINES
#include "Utils.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Camera
{
public:
	Camera();
	virtual ~Camera();
	 glm::vec3 getCameraPos() const;
	void SetCameraLookAt(const glm::vec3& eye = { 0,0,1 }, const glm::vec3& at = { 0,0,0 }, const glm::vec3& up = { 0,1,0 });
	const void Prespective(float fov, float aspect, float neardis, float fardis);
	const glm::mat4x4& GetProjectionTransformation() const;
	const glm::mat4x4& GetViewTransformation() const;
	const void GetOrtho(float left = -10.f, float right = 10.f, float down = -10.f, float up = 10.f, float near = 0.1f, float far = 10.f);
private:
	glm::vec3 camerePos;
	glm::mat4x4 view_transformation;
	glm::mat4x4 projection_transformation;
	
};
