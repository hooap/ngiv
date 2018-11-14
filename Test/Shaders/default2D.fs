#version 330 core

in vec2 fragUV;

uniform sampler2D sampler_texture;

out vec4 color;

void main(){
	
    
    color = texture(sampler_texture, fragUV);

 
}