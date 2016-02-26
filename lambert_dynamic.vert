attribute vec3 vertex;
attribute vec3 normal;
attribute vec3 tangent;
attribute vec3 color;
attribute vec2 UV;


varying vec3 outNormal; 
varying vec3 outTangent; 
varying vec3 outColor; 
varying vec2 outUV; 


varying vec2 Texcoord;
varying vec3 LightDirection;
uniform vec3 fvLightPosition;


void main(void)
{
    gl_Position = gl_ModelViewProjectionMatrix * vec4(vertex,1.0);
    Texcoord = gl_MultiTexCoord0.xy;
    vec4 objectPosition = gl_ModelViewMatrix * vec4(vertex,1.0);
   
    LightDirection = (gl_ModelViewMatrix * vec4(fvLightPosition, 1)).xyz - objectPosition.xyz;  

    outNormal = gl_NormalMatrix * normal;
	outTangent= gl_NormalMatrix * tangent;
	outColor = color;
	outUV=UV;
}