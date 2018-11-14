#pragma once

#include <glm\glm.hpp>
#include <GLAD\glad.h>
#include <vector>
#include "Vertex.h"

class obj_box
{
public:
	obj_box();
	~obj_box();

	void init(glm::vec3 pos, glm::vec3 dims, GLuint tex_dif, GLuint tex_spec);

private:


	glm::vec3 _pos;
	glm::vec3 _dims;
	GLuint _tex_dif;
	GLuint _tex_spec;
	
	std::vector<Vertex> _verts;

};

