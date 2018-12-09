#version 330 core
in vec3 vertexPos; // <vec2 pos, vec2 tex>
in vec2 vertexTex;
out vec2 TexCoords;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(vertexPos.xy, 0.0, 1.0);
	gl_Position.z = vertexPos.z;


    TexCoords = vertexTex;
}  