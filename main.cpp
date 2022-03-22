// Include Libraries
#include <iostream>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


// GLM Libraies
#include <glm/glm.hpp> //includes GLM
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include <glm/ext/matrix_transform.hpp> // GLM: translate, rotate
#include <glm/ext/matrix_clip_space.hpp> // GLM: perspective and ortho 
#include <glm/gtc/type_ptr.hpp> // GLM: access to the value_ptr


// Sound Library - Used for background music
#include <irrklang/irrklang.h>
using namespace irrklang;


// ImGUI Library - Used for the GUI elements on screen
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


// Assimp Library - Supports model importing
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


// Header fILES
#include "shaderLoader.h"
#include"camera.h"

// Integers
const unsigned int windowWidth = 1280;
const unsigned int windowHeight = 720;


// Start the sound engine
ISoundEngine* soundEngine = createIrrKlangDevice();
ISoundEngine* backgoundMusic = createIrrKlangDevice();


#pragma region vertices and indices for a cube (Skybox Cube)
float customSkyboxVerts[] = 
{
	//Coords
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f

};

unsigned int customSkyboxIndi[] =
{
	1, 2, 6,
	6, 5, 1,

	0, 4, 7,
	7, 3, 0,

	4, 5, 6,
	6, 7, 4,

	0, 3, 2,
	2, 1, 0,

	0, 1, 5,
	5, 4, 0,

	3, 7, 6,
	6, 2, 3
};
#pragma endregion

int main(int argc, char* argv[])
{
	#pragma region Initilisation and Window Creation
	// Initialize GLFW & Tell GLFW what version of OpenGL we are using
	glfwInit();
	const char* glsl_version = "#version 440";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 	// < Tell GLFW we are using the CORE profile >

	// Create Window called Dynamic Shader Representation Tool - Resolution = 1280x720
	GLFWwindow* applicationWindow = glfwCreateWindow(windowWidth, windowHeight, "Dynamic Shader Representation Tool", NULL, NULL);

	// Check if window fails
	if (applicationWindow == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Introduce the window into the current context
	glfwMakeContextCurrent(applicationWindow);

	//Load GLAD
	gladLoadGL();

	// Culling - Supports performance

	// Specify the viewport of OpenGL in the Window
	glViewport(0, 0, windowWidth, windowHeight);
	#pragma endregion



	#pragma region Actiavte Shaders
	// Generates lShader object using shaders defualt.vert and default.frag
	lShader skyboxShader("skyboxShader.vert", "skyboxShader.frag");

	skyboxShader.Activate();
	glUniform1i(glGetUniformLocation(skyboxShader.ID, "customSkybox"), 0);

	glEnable(GL_DEPTH_TEST);
	#pragma endregion



	#pragma region Irrklang & ImGui Initilisation
	// Initialising ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(applicationWindow, true);
	ImGui_ImplOpenGL3_Init();



	// Loop background music in game and set the volume
	ISound* music = soundEngine->play2D("media/audio/Dreamer-by-Hazy.flac", true, false, true);
	music->setVolume(0.05f);
	#pragma endregion
	


	#pragma region Camera and Colours
	//Creates a camera object based on the screen size and provides distance
	Camera cam(windowWidth, windowHeight, glm::vec3(0.0f, 0.0f, -50.0f));

	ImVec4 backgroundColourValue = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	#pragma endregion



	#pragma region SkyBox
	unsigned int customSkyboxVAO, customSkyboxVBO, customSkyboxEBO;
	glGenVertexArrays(1, &customSkyboxVAO);
	glGenBuffers(1, &customSkyboxVBO);
	glGenBuffers(1, &customSkyboxEBO);

	glBindVertexArray(customSkyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, customSkyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(customSkyboxVerts), &customSkyboxVerts, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, customSkyboxEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(customSkyboxIndi), &customSkyboxIndi, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);




	std::string skyboxImageLocations[6] =
	{
		"media/skybox/Skybox_Right.jpg",
		"media/skybox/Skybox_Left.jpg",
		"media/skybox/Skybox_Top.jpg",
		"media/skybox/Skybox_Bottom.jpg",
		"media/skybox/Skybox_Front.jpg",
		"media/skybox/Skybox_Back.jpg"

	};


	unsigned int cubemapTex;
	glGenTextures(1, &cubemapTex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTex);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	// Clamp textures to prevent seems of the cube from being displayed
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


	for (unsigned int i = 0; i < 6; i++) 
	{
		int width, height, nrChannels;
		unsigned char* data = stbi_load(skyboxImageLocations[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			stbi_set_flip_vertically_on_load(false);

			glTexImage2D
			(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				GL_RGB,
				width,
				height,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				data
			);
			stbi_image_free(data);
			std::cout << "Success: Cubemap Creation Is Successful " << skyboxImageLocations[i] << std::endl;
		}
		else 
		{
			std::cout << "Error: Couldn't Load Texture: " << skyboxImageLocations[i] << std::endl;
			stbi_image_free(data);
		}
	}
	#pragma endregion



	#pragma region While Loop - Runs the entire application!
	// Project while loop
	while (!glfwWindowShouldClose(applicationWindow))
	{
		// Start the ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		// Display Lighting Object Locations and Lighting Colours
		ImGui::Begin("Dynamic Shader Representation Tool - Editor Window"); // Create Window - Title: Super Dodger

		ImGui::ColorEdit4("Background Colour", (float*)&backgroundColourValue);

		//Will be used to change Light Position and colour
		// ImGui::SliderFloat3("Position of Light", &ligthPos.x, ligthPos.y, ligthPos.z);
		// ImGui::ColorEdit3("Colour of Light", (float*)&lightCol);

		ImGui::Text("Average Framerate: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate); // Dispplay the framerate
		ImGui::End();

		// Specify the viewport of OpenGL in the Window
		ImGui::Render();
		int displayWidth, displayHeight;
		glfwGetFramebufferSize(applicationWindow, &displayWidth, &displayHeight);
		glViewport(0, 0, displayWidth, displayHeight);

		// Set window colour, clear/swap buffer
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cam.ManageDeltaTime();
		cam.userInputs(applicationWindow);
		cam.updateCamMatrix(45.0f, 0.1f, 100.0f);

		// Drawing the Custom Skybox
		glDepthFunc(GL_LEQUAL);

		skyboxShader.Activate();
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		view = glm::mat4(glm::mat3(glm::lookAt(cam.cameraPosition, cam.cameraPosition + cam.cameraOrientation, cam.Up)));
		projection = glm::perspective(glm::radians(45.0f), (float)displayWidth / displayHeight, 0.1f, 100.0f);

		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(customSkyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTex);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glDepthFunc(GL_LESS);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(applicationWindow);
		glfwPollEvents();
	}
	#pragma endregion



	#pragma region Delete objects, window, and GLFW before closing to support Memory Optimization
	// Cleanup

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();


	// Delete window before software shutdown
	glfwDestroyWindow(applicationWindow);


	// Terminate GLFW before software shutdown
	glfwTerminate();
	return 0;
	#pragma endregion

}
