#version 330 core

in vec2 fragUV;


uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;


out vec4 color;



void main(){
	
    
    color = texture(texture_diffuse, fragUV);
	color.a = texture(texture_specular,fragUV).r;

 
}