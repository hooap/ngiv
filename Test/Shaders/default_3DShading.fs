#version 330 core
out vec4 FragColor;

in vec2 TexCoords;


struct Light {
    vec3 Position;
    vec3 Color;    
    float Linear;
    float Quadratic;
	float Radius;
};


const int NR_LIGHTS = 1;
uniform Light lights[NR_LIGHTS];

uniform vec3 viewPos;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;


vec3 CalcDirLight(vec3 normal, vec3 viewDir, vec3 diffuse, float specular);

void main()
{             
    // retrieve data from gbuffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Diffuse = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;
    
    // then calculate lighting as usual
    vec3 lighting  = Diffuse * 0.1; // hard-coded ambient component
    vec3 viewDir  = normalize(viewPos - FragPos);
    for(int i = 0; i < NR_LIGHTS; ++i)
    {
		float distance = length(lights[i].Position - FragPos);
		if(distance < lights[i].Radius){			
			// diffuse
			vec3 lightDir = normalize(lights[i].Position - FragPos);
			vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * lights[i].Color;
			// specular
			vec3 halfwayDir = normalize(lightDir + viewDir);  
			float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
			vec3 specular = lights[i].Color * spec * Specular;
			// attenuation
			
			float attenuation = 1.0 / (1.0 + lights[i].Linear * distance + lights[i].Quadratic * distance * distance);
		    diffuse *= attenuation;
		    specular *= attenuation;
			lighting += diffuse + specular;     
		}   
    }
	
	vec3 dirlight = CalcDirLight(Normal,viewDir,Diffuse,Specular);
	lighting += dirlight;
	

    FragColor = vec4(lighting, 1.0);
	//FragColor = vec4(Diffuse, 1.0);

	
}



vec3 CalcDirLight(vec3 normal, vec3 viewDir, vec3 diffuse, float specular)
{
	vec3 lightdir = vec3(-0.1, -0.2, -0.3);
    vec3 lightDir = normalize(-lightdir);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
    // combine results
	
	float lighdif = 1.0;
	float lighspec = 1.1;
	
    vec3 lldif  = vec3(lighdif)  * vec3(diff) * diffuse;
    vec3 llspec = vec3(lighspec) * spec * vec3(specular);

    return lldif + llspec;
}  
