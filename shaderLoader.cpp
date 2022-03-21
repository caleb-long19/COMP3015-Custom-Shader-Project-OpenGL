// Include Project Header Files
#include "shaderLoader.h"


// Constructor that build the lShader Program from 2 different shaders
lShader::lShader(const char* vertexFile, const char* fragmentFile)
{
	// Create Vertex lShader Object and get its reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Create Fragment lShader Object and get its reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Create lShader Object, retrieve reference & Attach Vertex/Fragment Shaders to lShader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);

	// Linl the shaders to the lShade & Delete the Vertex/Fragment lShader objects
	glLinkProgram(ID);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}


// Activate lShader
void lShader::Activate()
{
	glUseProgram(ID);
}


// Delete lShader
void lShader::Delete()
{
	glDeleteProgram(ID);
}
