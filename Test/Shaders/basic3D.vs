#version 330 core


in vec3 aPos;
in vec3 aNormal;
in vec2 aTexCoords;



out vec3 fragPos;  
out vec2 fragUV;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main(){
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	
	fragUV = aTexCoords;
 }
