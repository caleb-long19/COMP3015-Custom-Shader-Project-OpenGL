#ifndef __LOAD_SHADERS_H__
#define __LOAD_SHADERS_H__

// Library Header Files
#include "glad/glad.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>


class lShader
{
public:
	GLuint ID;
	lShader(const char* vertexFile, const char* fragmentFile);

	void Activate();
	void Delete();

};

#endif
