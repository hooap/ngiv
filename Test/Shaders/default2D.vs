#version 330 core

in vec2 vertexPos;
in vec2 vertexUV;

out vec2 fragUV;

uniform mat4 projection;

void main(){

	gl_Position.xy = (projection * vec4(vertexPos, 0.0, 1.0)).xy; 
	gl_Position.z = 0;
	gl_Position.w = 1;
	
    fragUV = vertexUV;
 }
