uniform vec4 Diffuse;
uniform vec4 Ambient;
uniform vec4 Specular;
uniform float SpecularPower;


uniform sampler2D diffuseMap0;
uniform sampler2D normalMap1;
uniform sampler2D specularMap2;

varying vec2 Texcoord;


varying vec3 LightDirection;
varying vec3 ViewDirection;
varying vec3 Normal;


void main(void)
{
   vec3 nLightDirection = normalize(LightDirection);
   vec3 nNormal = normalize((texture2D(normalMap1,Texcoord).xyz * 2.0) - 1.0);
   
   float NDotL = dot(nNormal,nLightDirection);
   
   vec3 Reflection = normalize(((2.0 * nNormal) * NDotL) - nLightDirection);
   vec3 nViewDirection = normalize(ViewDirection);
   float RDotV = max(0.0,dot(Reflection, nViewDirection));
  
     
   vec4 diffuseColor = texture2D(diffuseMap0,Texcoord);

   vec4 DiffuseColor = diffuseColor * Diffuse * NDotL; 
   vec4 AmbientColor =  Ambient * diffuseColor;
  // vec4 SpecularColor = Specular * pow(RDotV, SpecularPower);
   vec4 SpecularColor = texture2D(specularMap2,Texcoord).xyza * pow(RDotV, SpecularPower);

   gl_FragColor = DiffuseColor + AmbientColor + SpecularColor;

   //gl_FragColor = diffuseColor;
   
}