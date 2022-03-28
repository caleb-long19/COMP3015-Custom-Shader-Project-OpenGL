#version 460

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;


layout(binding=0) uniform sampler2D Tex1;


uniform struct LightInfo {
  vec4 Position;  // Light position in eye coords.
  vec3 La;        // Ambient light intesity
  vec3 L;         // Diffuse and specular light intensity
} Light;



uniform struct MaterialInfo {
  vec3 Ka;            // Ambient reflectivity
  vec3 Kd;            // Diffuse reflectivity
  vec3 Ks;            // Specular reflectivity
  float Shininess;    // Specular shininess factor
} Material;



uniform struct FogInfo {
  float MaxDist;
  float MinDist;
  vec3 Color;
} Fog;



layout( location = 0 ) out vec4 FragColor;



vec3 phongModel(vec3 position, vec3 n) {

  vec3 texColor = texture(Tex1, TexCoord).rgb;

  vec3 ambient = Light.La * texColor;

  vec3 s;

  if( Light.Position.w == 0.0 )
    s = normalize( Light.Position.xyz );
  else
    s = normalize( Light.Position.xyz - position );

  float sDotN = max( dot(s,n), 0.0 );

  vec3 diffuse = texColor * sDotN;
  vec3 spec = vec3(0.0);

  if( sDotN > 0.0 ) {
    vec3 v = normalize(-position.xyz); 
    vec3 r = reflect( -s, n ); 
    spec = Material.Ks * pow( max( dot(r,v), 0.0 ), Material.Shininess );
  }

  return ambient + Light.L * (diffuse + spec);
}

void main()
{

   float dist = abs( Position.z );

   float fogFactor = (Fog.MaxDist - dist) /(Fog.MaxDist - Fog.MinDist); 

   fogFactor = clamp(fogFactor, 0.0, 1.0);

   vec3 shadeColor = phongModel(Position, normalize(Normal));

   vec3 color = mix(Fog.Color, shadeColor, fogFactor);

   FragColor = vec4(color, 1.0);
}