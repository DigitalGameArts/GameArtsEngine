varying vec3 outNormal; 
varying vec3 outTangent; 
varying vec3 outColor; 
varying vec3 outUV; 

uniform sampler2D texture0;

void main()
{
	gl_FragColor = vec4(outColor,1.0);
}