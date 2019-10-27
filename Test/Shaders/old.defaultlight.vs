#version 330 core

in vec3 vertexPos;
in vec3 vertexNormal;
in vec2 vertexUV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragNormal;
out vec3 fragPos;  
out vec2 fragUV;

void main() {
    
    gl_Position = projection * view * model * vec4(vertexPos, 1.0);
	fragNormal = mat3(transpose(inverse(model))) * vertexNormal;  

	fragPos = vec3(model * vec4(vertexPos, 1.0));
	fragUV = vertexUV;
	
	}