#version 330 core

in vec3 vertexPos;
in vec2 vertexUV;

out vec2 fragUV;

uniform mat4 projection;

void main(){

	gl_Position = projection * vec4(vertexPos.xy, 0.0, 1.0); 
	gl_Position.z = vertexPos.z;
	
    fragUV = vertexUV;
 }
