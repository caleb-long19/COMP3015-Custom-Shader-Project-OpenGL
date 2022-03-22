#version 440 core

out vec4 fragmentColour;

in vec3 textureCoordinates;

uniform samplerCube customSkybox;

void main()
{
	fragmentColour = texture(customSkybox, textureCoordinates);
}