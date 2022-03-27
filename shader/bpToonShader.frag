#version 460

// THIS CURRENTLY CONTAINS THE SPOTLIGHT BLINNPHONG MODEL - LAB 3

in vec3 Position;
in vec3 Normal;

uniform struct LightInfo {
  vec4 Position;  // Light position in eye coords.
  vec3 La;        // Ambient light intesity
  vec3 L;         // Diffuse and specular light intensity
} Light;

uniform struct MaterialInfo {
  vec3 Ka;            // Ambient reflectivity
  vec3 Kd;            // Diffuse reflectivity
} Material;

const int levels = 3;
const float scaleFactor = 1.0 / levels;

layout( location = 0 ) out vec4 FragColor;

vec3 bpToonShader( ) {
    vec3 n = normalize( Normal );
    vec3 s = normalize( Light.Position.xyz - Position );
    vec3 ambient = Light.La * Material.Ka;
    float sDotN = max( dot( s, n ), 0.0 );
    vec3 diffuse = Material.Kd * floor( sDotN * levels ) * scaleFactor;

    return ambient + Light.L * diffuse;
}

void main() {
    FragColor = vec4(bpToonShader(), 1.0);
}