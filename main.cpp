// ImGUI Library - Used for the GUI elements on screen
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// Include Libraries
#include <iostream>
#include <glad/glad.h>
#include <glfw/glfw3.h>
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


int main(int argc, char* argv[])
{
	#pragma region Initilisation and Window Creation
	// Initialize GLFW & Tell GLFW what version of OpenGL we are using
	glfwInit();
	const char* glsl_version = "#version 450";
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
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Specify the viewport of OpenGL in the Window
	glViewport(0, 0, windowWidth, windowHeight);

	glEnable(GL_DEPTH_TEST);
	#pragma endregion


	// Generates lShader object using shaders defualt.vert and default.frag
	lShader defaultShader("default_shader.vert", "default_shader.frag");

	glm::vec4 lightCol = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f);
	glm::vec3 ligthPos = glm::vec3(2.0f, 2.0f, 1.0f);
	glm::mat4 lightModel = glm::mat4(0.3f);

	lightModel = glm::translate(lightModel, ligthPos);

	glUniform4f(glGetUniformLocation(defaultShader.ID, "lightingColour"), lightCol.x, lightCol.y, lightCol.z, lightCol.w);
	glUniform3f(glGetUniformLocation(defaultShader.ID, "lightingPosition"), ligthPos.x, ligthPos.y, ligthPos.z);


	defaultShader.Activate();

	#pragma region Irrklang & ImGui Initilisation
	// Initialising ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(applicationWindow, true);
	ImGui_ImplOpenGL3_Init("Version 450");



	// Loop background music in game and set the volume
	ISound* music = soundEngine->play2D("media/audio/Dreamer-by-Hazy.flac", true, false, true);
	music->setVolume(0.05f);
	#pragma endregion



	//Creates a camera object based on the screen size and provides distance
	Camera cam(windowWidth, windowHeight, glm::vec3(0.0f, 0.0f, 0.0f));



	// Project while loop
	while (!glfwWindowShouldClose(applicationWindow))
	{
		// Start the ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		// Display Lighting Object Locations and Lighting Colours
		{
			ImGui::Begin("Dynamic Shader Representation Tool - Editor Window"); // Create Window - Title: Super Dodger

			ImGui::Text("Average Framerate: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate); // Dispplay the framerate
			ImGui::End();
		}


		cam.ManageDeltaTime();
		cam.userInputs(applicationWindow);


		// Specify the viewport of OpenGL in the Window
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(applicationWindow, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);

		// Set window colour, clear/swap buffer
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		glfwSwapBuffers(applicationWindow);
		glfwPollEvents();
	}




	#pragma region Delete objects, window, and GLFW before closing to support Memory Optimization
	// Cleanup
	defaultShader.Delete();

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
