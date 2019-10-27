#version 330 core
in vec3 aPos;
in vec2 aTexCoords;
in float aIndex;
in vec4 aOffset4y;
in vec2 aOffsetxz;


out VS_OUT {
	out vec3 VertexPos;
    out vec3 FragPos;
	out vec2 TexCoords;
} vs_out;



uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
	vec2 xzof = aOffsetxz;
	vec4 y4of = aOffset4y;

	vec3 ppos = aPos;
	ppos += vec3(xzof.x,0,xzof.y);
	
	
	float b = aIndex;
	
	if(b == 0){
		ppos.y = y4of.x;		
	}	
	if (aIndex == 1){
		ppos.y += y4of.y;
	}	
	if (aIndex == 2.0){
		ppos.y = y4of.z;
	}	
	if (aIndex == 3.0){
		ppos.y = y4of.w;		
	}
	
	
//	ppos.y += float(aIndex) * 0.0001;
	
	
	vs_out.VertexPos = ppos;
    vec4 worldPos = model * vec4(ppos, 1.0);
    vs_out.FragPos = worldPos.xyz; 
    vs_out.TexCoords = aTexCoords;
    
    gl_Position = projection * view * worldPos;
}