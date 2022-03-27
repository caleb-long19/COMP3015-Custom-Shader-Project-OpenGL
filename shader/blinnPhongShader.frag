#version 460

// THIS CURRENTLY CONTAINS THE SPOTLIGHT BLINNPHONG MODEL - LAB 3

in vec3 Position;
in vec3 Normal;
// in vec2 TexCoord

// layout(binding=0) uniform sampler2D Tex1;

uniform struct SpotLightInformation {
    // Eye coordinate location
    vec3 Position;   

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


vec3 blinnPhongSpot(vec3 position, vec3 n) {  

  vec3 ambient = Spot.La * Material.Ka, diffuse = vec3(0), spec = vec3(0);

  vec3 s = normalize( Spot.Position - position );

  float cosAng = dot(-s, normalize(Spot.Direction));
  float angle = acos( cosAng );
  float spotScale = 0.0;

  if(angle >= 0.0 && angle < Spot.Cutoff ) {

    spotScale = pow( cosAng, Spot.Exponent );
    float sDotN = max( dot(s,n), 0.0 );
    diffuse = Material.Kd * sDotN;

    if( sDotN > 0.0 ) 
    {
      vec3 v = normalize(-position.xyz);
      vec3 h = normalize( v + s );
      spec = Material.Ks * pow( max( dot(h,n), 0.0 ), Material.Shininess );
    }

  }
  return ambient + spotScale * Spot.L * (diffuse + spec);
}

void main() {
  // TexCoord = VertexTexCoord;

  FragColor = vec4(blinnPhongSpot(Position, normalize(Normal)), 1);
}