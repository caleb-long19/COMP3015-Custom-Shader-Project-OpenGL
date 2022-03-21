#version 460 core

out vec4 FragmentColour;

// Imports Current Position (default_shader.vert)
in vec3 currentPosition;

// Imports Normals (default_shader.vert)
in vec3 Normals;

// Imports Colour (default_shader.vert)
in vec3 colour;

// Imports Texture Coordinates (default_shader.vert)
in vec2 textureCoordinates;




// Retrieves the texture units from the Project.cpp file
uniform sampler2D diffuse0;
uniform sampler2D specular0;



// Retrieves the Lighting Colour from the main.cpp file
uniform vec4 lightingColour;

// Retrieves the Lighting Position from the main.cpp file
uniform vec3 lightingPosition;

// Retrieves the Camera Position from the main.cpp file
uniform vec3 cameraPosition;



// Creates Directional Lighting
vec4 directSceneLighting()
{
	// Ambient Lighting
	float ambientLight = 0.5f;

	// Diffuse Lighting
	vec3 normal = normalize(Normals);
	vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f));
	float diffuse = max(dot(normal, lightDirection), 1.0f);

	// Specular Lighting
	float specularLighting = 0.4f;

	vec3 viewDir = normalize(cameraPosition - currentPosition);
	vec3 reflectionDir = reflect(-lightDirection, normal);
	float specularStorage = pow(max(dot(viewDir, reflectionDir), 0.0f), 18);
	float specular = specularStorage * specularLighting;

	return (texture(diffuse0, textureCoordinates) * (diffuse + ambientLight) + texture(specular0, textureCoordinates).r * specular) * lightingColour;
}



// Creates Spotlight Lighting
vec4 spotlightSceneLighting()
{

}



// Creates Point Lighting
vec4 pointSceneLighting
{

}



void main()
{
	// Data returned from directLight() is stored in Fragment Colour
	FragmentColour = directSceneLighting();
}