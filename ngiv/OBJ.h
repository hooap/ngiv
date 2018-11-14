#pragma once

#include "GLSLProgram.h"
#include "Mesh.h"
#include <glm\glm.hpp>
#include "ColorRGBA8.h"
#include "Collision_Box.h"

namespace ngiv {

	//DO NOT CREATE OBJ MODEL LOADER CREATES THAT
	class OBJ
	{
	public:
		friend class ModelLoader;
		~OBJ() {
			dispose();
			empty = true;
		}
		
		bool isEmpty() { return empty; }

		void dispose() {
			if (!empty) {
				empty = true;
				for (int i = 0; i < meshes.size(); i++) {
					meshes[i].dispose();
				}
				meshes.clear();
				pos = glm::vec3(0, 0, 0);
			}
		}		

		std::vector<Mesh>* getMeshes() { return &meshes; }

		glm::vec3* getRotPointer() { return &rot; }
		glm::vec3 getRot() { return rot; }
		glm::vec3* getPosPointer() { return &pos; }
		glm::vec3 getPos() { return pos; }	
		void setPos(glm::vec3 pos) { this->pos = pos; 	}		
		void setCenterPos(glm::vec3 centerpos) { this->pos = centerpos - center_of_mass; }
		glm::vec3 getCenterPos() { return pos + center_of_mass; }
		std::string directory;
		glm::vec3 get_center_of_mass() { return center_of_mass; }
		glm::vec3 getscale() { return scale; }
		

		Collision_Object* setCollisionObject(Collision_Object collisionObject) {
			collision_boxes = collisionObject;

			return &collision_boxes;
		}
		Collision_Object* setCollisionObject(Collision_Sphere collisionObject) {
			collision_boxes.setExtraPos(pos);
			collision_boxes.addSphere(collisionObject);	
			collision_boxes.setCenterofmass(center_of_mass);
			pos = collisionObject.getPos();

			return &collision_boxes;
		}
		Collision_Object* setCollisionObject(Collision_Box collisionObject) {
			collision_boxes.setExtraPos(getCenterPos());
			collision_boxes.addBox(collisionObject);
			collision_boxes.setCenterofmass(center_of_mass);
			

			return &collision_boxes;
		}
		
		Collision_Object* getCollision_Object() { return &collision_boxes; }

		void updateObject() {
			pos = collision_boxes.getExtraPos();
			rot = collision_boxes.getRotation();
		}
		
		float xdif;
		float ydif;
		float zdif;
	protected:
		Collision_Object collision_boxes;

		OBJ() {
			empty = true;
			pos = glm::vec3(0);
			rot = glm::vec3(0);
		};
		

		bool empty = true;
		glm::vec3 center_of_mass;

		glm::vec3 pos;
		glm::vec3 rot;

		std::vector<Mesh> meshes;
		glm::vec3 scale;
	};
	
	

}