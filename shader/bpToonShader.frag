#version 460

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;


layout(binding=0) uniform sampler2D Tex1;


uniform struct LightInfo {
  // Light Position (In Eye Coordinates)
  vec4 Position;

  // Ambient Intensity
  vec3 La;       

  // Diffuse/Specular Light Intensity
  vec3 L;         
} Light;



uniform struct MaterialInfo {
  // Ambient reflectivity
  vec3 Ka;           

  // Diffuse reflectivity
  vec3 Kd;            
} Material;



uniform struct FogInfo {

  //Max distance of fog
  float MaxDist;

  //Min distance of fog
  float MinDist;

  //Fog Colour
  vec3 Color;

} Fog;


const int levels = 3;
const float scaleFactor = 1.0 / levels;


layout( location = 0 ) out vec4 FragColor;


vec3 bpToonShader( ) {

    vec3 texColor = texture(Tex1, TexCoord).rgb;

    vec3 n = normalize( Normal );
    vec3 s = normalize( Light.Position.xyz - Position );
    vec3 ambient = Light.La * texColor;

    float sDotN = max( dot( s, n ), 0.0 );
    vec3 diffuse = texColor * floor( sDotN * levels ) * scaleFactor;

    return ambient + Light.L * diffuse;
}

void main() {

   float dist = abs( Position.z );

   float fogFactor = (Fog.MaxDist - dist) /(Fog.MaxDist - Fog.MinDist); 

   fogFactor = clamp(fogFactor, 0.0, 1.0);

   vec3 shadeColor = bpToonShader();

   vec3 color = mix(Fog.Color, shadeColor, fogFactor);

   FragColor = vec4(color, 1.0);
}