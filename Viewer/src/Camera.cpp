#include "Camera.h"

Camera::Camera()
{
	GetOrtho();
    SetCameraLookAt();
}

Camera::~Camera()
{
	
}


glm::vec3 Camera::getCameraPos() const
{
	return camerePos;
}

void Camera::SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up)
{
	camerePos = eye;
	view_transformation = glm::lookAt(eye, at, up);
}
const void Camera::GetOrtho(float left, float right, float down, float up, float neard, float fard)
{
	projection_transformation = glm::ortho(left, right, down, up, neard, fard);
}

const void Camera::Prespective(float fov, float aspect, float neard, float fard)
{
	projection_transformation = glm::perspective(fov, aspect, neard, fard);
}

const glm::mat4x4& Camera::GetProjectionTransformation() const 
{
	return  projection_transformation;
}

const glm::mat4x4& Camera::GetViewTransformation() const
{
	return view_transformation;
}