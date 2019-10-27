#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;


in VS_OUT {
	in vec3 VertexPos;
    in vec3 FragPos;
	in vec2 TexCoords;
} gs_in[];


out vec3 FragPos;
out vec2 TexCoords;

out vec3 Normal;

void main() {    
    vec3 a = vec3(gs_in[1].VertexPos - gs_in[0].VertexPos);
    vec3 b = vec3(gs_in[2].VertexPos - gs_in[0].VertexPos);
    vec3 N = normalize( cross( b, a ) );

	for( int i=0; i<gl_in.length( ); ++i )
    {
        gl_Position = gl_in[i].gl_Position;
        Normal = N;
		TexCoords = gs_in[i].TexCoords;
		FragPos = gs_in[i].FragPos;
        EmitVertex( );
    }

    EndPrimitive( );



}  
