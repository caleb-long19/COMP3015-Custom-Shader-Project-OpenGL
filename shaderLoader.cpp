// Include Project Header Files
#include "shaderLoader.h"

// Read shader file, output data as a string
std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

// Constructor that build the lShader Program from 2 different shaders
lShader::lShader(const char* vertexFile, const char* fragmentFile)
{
	// Read vertexFile and fragmentFile and store the strings
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);


	// Convert the lShader source strings into character arrays
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();


	// Create Vertex lShader Object and get its reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);


	// Attach Vertex lShader source to the Vertex lShader Object & Compile the Vertex lShader into machine code
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);



	// Create Fragment lShader Object and get its reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Attach Fragment lShader source to the Fragment lShader Object & Compile the Fragment lShader into machine code
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);




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
