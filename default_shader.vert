#version 460 core

// Position Coordinates
layout (location = 0) in vec3 aPosition;

// Normals
layout (location = 1) in vec3 aNormals;

// Colours
layout (location = 2) in vec3 aColour;

// Texture Coordinates
layout (location = 3) in vec2 aTexture;



// Outputs Position
out vec3 currentPosition;

// Outputs normal
out vec3 Normals;

// Outputs colour
out vec3 colour;

// Outputs texture coordinates
out vec2 textureCoordinates;




// Imports the camera matrix
uniform mat4 cameraMatrix;
uniform mat4 model;

// Imports the model matrix 
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;




void main()
{
	//Calculates the current position
	currentPosition = vec3(model * translation * -rotation * scale * vec4(aPosition, 1.0f));

	//Assigns normal to Vertex Data
	Normals = aNormals;

	//Assigns colour to Vertex Data
	colour = aColour;

	//Assigns texture coordinates to Vertex Data
	textureCoordinates = mat2(0.0, -1.0, 1.0, 0.0) * aTexture;

	gl_Position = cameraMatrix * vec4(currentPosition, 1.0);
}