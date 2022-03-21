// Include Project Header Files
#include"camera.h"
#include "windows.h";

float deltaTime = 0.0f;
float currentTime = 0.0f;
float lastFrame = 0.0f;

Camera::Camera(int wWidth, int wHeight, glm::vec3 camPosition)
{
	Camera::windowWidth = wWidth;
	Camera::windowHeight = wHeight;
	cameraPosition = camPosition;
}


void Camera::updateCamMatrix(float FOVdeg, float nearPlane, float farPlane)
{
	// Initializes matrices since otherwise they will be the null matrix
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	// Makes camera look in the right direction from the right cameraPosition
	//view positions / rotates our camera to look at a point in our scene, from another point
	view = glm::lookAt(cameraPosition, cameraPosition + cameraOrientation, Up);

	// Adds perspective to the scene
	projection = glm::perspective(glm::radians(FOVdeg), (float)windowWidth / windowHeight, nearPlane, farPlane);

	// Sets new camera matrix
	cameraMatrix = projection * view;
}



void Camera::ManageDeltaTime()
{
	currentTime = glfwGetTime();
	deltaTime = currentTime - lastFrame;
	lastFrame = currentTime;
}



void Camera::Matrix(lShader& lShader, const char* uniform)
{
	// Exports camera matrix
	glUniformMatrix4fv(glGetUniformLocation(lShader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}



void Camera::userInputs(GLFWwindow* window)
{
	//Move Right
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		cameraPosition += cameraSpeed * cameraOrientation;
		OutputDebugStringW(L"My output string.");
	}

	//Move Left
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		cameraPosition += cameraSpeed * -glm::normalize(glm::cross(cameraOrientation, Up));
	}

	//Move Down
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		cameraPosition += cameraSpeed * -cameraOrientation;
	}

	//Move Right
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		cameraPosition += cameraSpeed * glm::normalize(glm::cross(cameraOrientation, Up));
	}
}