#version 460

// THIS CURRENTLY CONTAINS THE SPOTLIGHT BLINNPHONG MODEL - LAB 3

in vec3 LightDir;
in vec2 TexCoord;
in vec3 ViewDir;

layout(binding=0) uniform sampler2D ColorTex;
layout(binding=1) uniform sampler2D NormalMapTex;


uniform struct SpotLightInformation {
    // Eye coordinate location
    vec4 Position;   

    // Diffuse & Specular Intensity
    vec3 L; 

    //Ambience Intensity
    vec3 La;

    //Spoitlight direction (Eye coordinates)
    vec3 Direction;
    
    // Angular Attenuation
    float Exponent;

    // Angle Cutoff
    float Cutoff;
} Spot;


uniform struct MaterialInformation {
  // Ambient reflectivity
  vec3 Ka;

  // Diffuse reflectivity
  vec3 Kd;

  // Specular reflectivity
  vec3 Ks;

  // Specular shininess factor
  float Shininess;

} Material;


layout( location = 0 ) out vec4 FragColor;


vec3 blinnPhongSpot(vec3 n) {  

  vec3 diffuse;
  vec3 spec;

  vec3 texColor = texture(ColorTex, TexCoord).rgb;
    
  vec3 ambient = Spot.La * texColor;

  vec3 s = normalize(LightDir);

  float cosAng = dot(-s, normalize(Spot.Direction));
  float angle = acos(cosAng);
  float spotScale = 0.0;

  if(angle >= 0.0 && angle < Spot.Cutoff) {

    spotScale = pow(cosAng, Spot.Exponent);

    float sDotN = max( dot(s,n), 0.0);

    diffuse = texColor * sDotN;

    spec = vec3(0.0);
    if( sDotN > 0.0 ) 
    {
      vec3 v = normalize(ViewDir);
      vec3 h = normalize(v + s);

      spec = Material.Ks * pow( max(dot(h,n), 0.0 ), Material.Shininess);
    }

  }
  return ambient + spotScale * Spot.L * (diffuse + spec);
}

void main() 
{
  vec3 norm = texture(NormalMapTex, TexCoord).xyz;
  norm.xy = 2.0 * norm.xy - 1.0;

  FragColor = vec4(blinnPhongSpot(norm), 1.0);
}