#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

// Include Library Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

//Include Project File Headers
#include "shaderLoader.h"




class Camera
{
public:
	// Camera Vectors
	glm::vec3 cameraPosition;
	glm::vec3 cameraOrientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 cameraMatrix = glm::mat4(1.0f);


	// Width/Height of the window
	int windowWidth;
	int windowHeight;

	float cameraSpeed = 40.0;

	// Camera constructor to set up initial values
	Camera(int winWidth, int winHeight, glm::vec3 camPosition);


	// Updates the camera matrix to the Vertex lShader
	void updateCamMatrix(float FOVdeg, float nearPlane, float farPlane);


	void ManageDeltaTime();


	// Exports camera matrix to a lShader
	void Matrix(lShader& lShader, const char* uniform);


	// Manages the userInputs from the user
	void userInputs(GLFWwindow* window);
};
#endif
