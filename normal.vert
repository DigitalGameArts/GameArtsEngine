attribute vec3 vertex;
attribute vec3 normal;
attribute vec3 tangent;
attribute vec3 color;
attribute vec2 UV;

uniform vec3 fvLightPosition;
uniform vec3 fvEyePosition;


varying vec2 Texcoord;

varying vec3 Normal;
varying vec3 LightDirection;
varying vec3 ViewDirection;

attribute vec3  rm_Binormal;
attribute vec3  rm_Tangent;


void main(void)
{
   gl_Position = gl_ModelViewProjectionMatrix * vec4(vertex,1.0);
   
   Texcoord = UV;
   
   vec4 objectPosition = gl_ModelViewMatrix * vec4(vertex,1.0);
   
   vec3 nNormal = gl_NormalMatrix * normal;
   vec3 nBinormal = gl_NormalMatrix * rm_Binormal;
   vec3 nTangent = gl_NormalMatrix * tangent;
   
   
   vec3 nLightDirection = (gl_ModelViewMatrix * vec4(fvLightPosition, 1)).xyz - objectPosition.xyz;
  // vec3 nLightDirection = fvLightPosition - objectPosition.xyz;
   
   vec3 nViewDirection = fvEyePosition -  objectPosition.xyz;
  
   ViewDirection.x = dot(nTangent, nViewDirection);
   ViewDirection.y = dot(nBinormal, nViewDirection);
   ViewDirection.z = dot(nNormal, nViewDirection);
   
   
   LightDirection.x = dot(nTangent,nLightDirection);
   LightDirection.y = dot(nBinormal,nLightDirection);
   LightDirection.z = dot(nNormal,nLightDirection);
  
   Normal = gl_NormalMatrix * normal; 
}