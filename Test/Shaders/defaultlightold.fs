#version 330 core

struct Material {
    sampler2D texture_diffuse1;
	sampler2D texture_specular1;
    float shininess;
}; 


struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

struct SpotLight{
	vec3 position;
	vec3 front;
	
	float cutOff;
	float outerCutOff;
	
	float constant;
	float linear;
	float quadratic;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};


in vec3 fragNormal;
in vec3 fragPos;
in vec2 fragUV;



uniform DirLight dirLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;
uniform bool spotlighton;



uniform vec3 viewPos;
uniform Material material;

out vec4 color;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
	

	
	vec3 normal = normalize(fragNormal);
    vec3 viewDir = normalize(viewPos - fragPos);
	vec3 resultdir = CalcDirLight(dirLight, normal, viewDir);
	vec3 resultpoint = CalcPointLight(pointLight, normal, fragPos, viewDir);    
	
	vec3 resultspot;
	if(spotlighton){
	resultspot = CalcSpotLight(spotLight, normal, fragPos, viewDir);
	} else {
		resultspot = vec3(0);
	}
	    
	//color = vec4(resultdir + resultpoint + resultspot,texture(material.texture_diffuse1, fragUV).a);
	color = vec4(resultdir + resultpoint + resultspot, 1);
	
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
	
	vec3 tex_dif = vec3(texture(material.texture_diffuse1, fragUV));
	vec3 tex_spec = vec3(texture(material.texture_specular1, fragUV));
		
    vec3 ambient  = light.ambient  * tex_dif;
    vec3 diffuse  = light.diffuse  * diff * tex_dif;
    vec3 specular = light.specular * spec * tex_spec;
    return ambient + diffuse + specular;
}  

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // combine results
	
	vec3 tex_dif = vec3(texture(material.texture_diffuse1, fragUV));
	vec3 tex_spec = vec3(texture(material.texture_specular1, fragUV));
	
    vec3 ambient  = light.ambient  * tex_dif;
    vec3 diffuse  = light.diffuse  * diff * tex_dif;
    vec3 specular = light.specular * spec * tex_spec;
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return ambient + diffuse + specular;
} 

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
	
	vec3 lightDir = normalize(light.position - fragPos);  
	
	float theta     = dot(lightDir, normalize(-light.front));
	float epsilon   = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);    
			
	
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // combine results
	
	
	vec3 tex_dif = vec3(texture(material.texture_diffuse1, fragUV));
	vec3 tex_spec = vec3(texture(material.texture_specular1, fragUV));
	
    vec3 ambient  = light.ambient  * tex_dif;
    vec3 diffuse  = light.diffuse  * diff * tex_dif;
    vec3 specular = light.specular * spec * tex_spec;
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
	ambient  *= intensity;
    diffuse  *= intensity;
    specular *= intensity;
    return ambient + diffuse + specular;
	
}



