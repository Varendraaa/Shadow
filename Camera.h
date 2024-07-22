#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//Camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices
class Camera
{
public:
	//Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);
	Camera(glm::vec3 position);

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();

	void ProcessKeyboard(bool* keys, float deltaTime);
	void ProcessMouseMovement(float xOffset, float yOffset);
	void SetProjection(float fov, float aspectRatio, float nearPlane, float farPlane);

protected:
	//Camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	//Euler Angles
	float Yaw;
	float Pitch;

	//Camera Properties
	float MouseSensitivity;
	float MovementSpeed;
	
	//float zoom;
	float fov;
	float aspectRatio;
	float nearPlane;
	float farPlane;

	//Internal Method to update camera vectors
	void UpdateCameraVectors();
};
#endif