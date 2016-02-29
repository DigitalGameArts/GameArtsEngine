uniform vec4 Diffuse;
uniform vec4 Ambient;
uniform sampler2D cloudTexture;
uniform sampler2D diffuseMap0;


varying vec2 Texcoord;

varying vec3 outNormal; 
varying vec3 outTangent; 
varying vec3 outColor; 




varying vec3 LightDirection;


void main(void)
{
   float facingRatio = dot(normalize(outNormal),normalize(LightDirection));
   
   vec4 diffuseColor = texture2D(diffuseMap0,Texcoord);
   vec4 lightColor = diffuseColor * Diffuse * facingRatio ; 
   
   //gl_FragColor = lightColor + Ambient * diffuseColor;
   //gl_FragColor = lightColor + Ambient * vec4(outColor,1.0);
   
   gl_FragColor =  lightColor +  Ambient * vec4(outColor,1.0) ; 

}


