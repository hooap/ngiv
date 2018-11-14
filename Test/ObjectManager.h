#pragma once
#include <glm\glm.hpp>
#include <GLAD\glad.h>

#include "Vertex.h"
#include "obj_box.h"


class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();
	void init();

	obj_box createBox(glm::vec3 pos, glm::vec3 dims, GLuint tex_dif, GLuint tex_spec);


private:


};

