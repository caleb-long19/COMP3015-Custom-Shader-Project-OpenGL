#version 440 core

layout (location = 0) in vec3 aPosition;

out vec3 textureCoordinates;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	vec4 position = projection * view * vec4(aPosition, 1.0f);
	gl_Position = vec4(position.x, position.y, position.w, position.w);
	textureCoordinates = aPosition;
}