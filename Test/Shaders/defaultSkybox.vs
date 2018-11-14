#version 330 core

in vec3 aPos;

out vec3 fragPos;  

uniform mat4 view;
uniform mat4 projection;


void main(){
	fragPos = aPos;
	vec4 pos = projection * view * vec4(aPos, 1.0);	
	gl_Position = pos.xyww;	
 }
