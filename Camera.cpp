#include "Camera.h"
#include <GLFW/glfw3.h>

//Default Camera Parameters. Allows for creation of a camera with preset params and easy adjustment later
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

//Constructor
// Constructor with position
Camera::Camera(glm::vec3 position) :
    Position(position),
    WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
    Front(glm::vec3(0.0f, 0.0f, -1.0f)),
    Yaw(YAW),
    Pitch(PITCH),    
    MovementSpeed(SPEED),
    MouseSensitivity(SENSITIVITY)
   /* aspectRatio(4.0f / 3.0f),
    nearPlane(0.1f),
    farPlane(100.0f) */
{
    UpdateCameraVectors();
}

//Returns the view matrix calculated using Euler Angges and the LookAt Matrix
glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(Position, Position + Front, Up);
}

//Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM
void Camera::ProcessKeyboard(bool* keys, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;			//double check this

    if (keys[GLFW_KEY_W])
        Position += glm::normalize(glm::vec3(Front.x, 0.0f, Front.z)) * velocity;
    if (keys[GLFW_KEY_S])
        Position -= glm::normalize(glm::vec3(Front.x, 0.0f, Front.z)) * velocity;
    if (keys[GLFW_KEY_A])
        Position -= glm::normalize(glm::vec3(Right.x, 0.0f, Right.z)) * velocity;
    if (keys[GLFW_KEY_D])
        Position += glm::normalize(glm::vec3(Right.x, 0.0f, Right.z)) * velocity;
}

//Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::ProcessMouseMovement(float xOffset, float yOffset)
{
    xOffset = xOffset * MouseSensitivity;
    yOffset = yOffset * MouseSensitivity;

    Yaw = Yaw + xOffset;
    Pitch = Pitch + yOffset;

    Pitch = glm::clamp(Pitch, -89.0f, 89.0f);

    UpdateCameraVectors();
    GetViewMatrix();
}

void Camera::SetProjection(float fov, float aspectRatio, float nearPlane, float farPlane)
{
    this->fov = fov;
    this->aspectRatio = aspectRatio;
    this->nearPlane = nearPlane;
    this->farPlane = farPlane;

}

glm::mat4 Camera::GetProjectionMatrix()
{
    return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

void Camera::UpdateCameraVectors()
{
    //Calculate new front vector
    glm::vec3 frontVec;
    frontVec.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    frontVec.y = sin(glm::radians(Pitch));
    frontVec.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

    //Normalise values and update the front vector
    Front = glm::normalize(frontVec);

    //Recalculate the right and up vectors
    Right = glm::cross(Front, WorldUp);
    Up = glm::cross(Right, Front);
}