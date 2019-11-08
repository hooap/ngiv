#version 330 core
out vec3 gPosition;
out vec3 gNormal;
out vec4 gAlbedoSpec;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;

void main()
{    
    // store the fragment position vector in the first gbuffer texture
    gPosition = FragPos;
    // also store the per-fragment normals into the gbuffer
    gNormal = normalize(Normal);
    // and the diffuse per-fragment color
	
	gAlbedoSpec.rgb = texture(texture_diffuse, vec2(TexCoords.x, TexCoords.y)).rgb;	
	gAlbedoSpec.a = texture(texture_specular, TexCoords).r;
	
	
    // store specular intensity in gAlbedoSpec's alpha component
   
}
