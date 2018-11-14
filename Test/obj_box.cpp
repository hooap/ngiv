#include "obj_box.h"



obj_box::obj_box()
{
}


obj_box::~obj_box()
{
}


void obj_box::init(glm::vec3 pos, glm::vec3 dims, GLuint tex_dif, GLuint tex_spec) {
	_pos = pos;
	_dims = dims;
	_tex_dif = tex_dif;
	_tex_spec = tex_spec;

	_verts.resize(36);

	
				
	

}