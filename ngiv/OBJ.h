#pragma once

#include "GLSLProgram.h"
#include "Mesh.h"
#include <glm/glm.hpp>
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
				for (int i = 0; i < meshes->size(); i++) {
					(*meshes)[i].dispose();
				}
				meshes->clear();
				delete meshes;
				pos = glm::vec3(0, 0, 0);
			}
		}

		const std::vector<Mesh>* getMeshes() { return meshes; }

		glm::vec3* getRotPointer() { return &rot; }
		glm::vec3 getRot() { return rot; }
		glm::vec3* getPosPointer() { return &pos; }
		glm::vec3 getPos() { return pos; }
		void setPos(glm::vec3 pos) { this->pos = pos; 	}
		void setCenterPos(glm::vec3 centerpos) { this->pos = centerpos - center_of_mass; }
		void setCenterRelative(glm::vec3 centerpos) { center_of_mass = centerpos; }

		glm::vec3 getCenterPos() { return pos + center_of_mass; }
		std::string directory;
		glm::vec3 getCenterPosRelative() { return center_of_mass; }
		glm::vec3 getscale() { return scale; }
		std::string getfilepath() { return filepath; }
		void setRot(glm::vec3 r) { rot = r; }

		std::string getName() { return name; }


		unsigned int getRendererID() { return __renderer_id; }
		void setRendererID(unsigned int id) { __renderer_id = id; }



		Collision_Object* createCollisionObject() {
			collision_boxes = new Collision_Object();

			return collision_boxes;
		}
		Collision_Object* creatensetCollisionObject(Collision_Sphere collisionObject, bool dynamic) {
			collision_boxes = new Collision_Object();
			collision_boxes->setExtraPos(pos);

			collision_boxes->addSphere(collisionObject);
			collision_boxes->setCenterofmass(center_of_mass);
			collision_boxes->setDynamic(dynamic);
			return collision_boxes;
		}
		Collision_Object* creatensetCollisionObject(Collision_Box collisionObject, bool dynamic) {
			collision_boxes = new Collision_Object();

			collision_boxes->setExtraPos(pos);

			collision_boxes->addBox(collisionObject);
			collision_boxes->setCenterofmass(center_of_mass);
			collision_boxes->setDynamic(dynamic);

			return collision_boxes;
		}

		Collision_Object* getCollision_Object() { return collision_boxes; }

		void setCollisionObject(Collision_Object* cob) { collision_boxes = cob; }

		void updateObject() {
			pos = collision_boxes->getExtraPos();
			rot = collision_boxes->getRotation();
		}

		float xdif;
		float ydif;
		float zdif;
	protected:
		unsigned int __renderer_id = -1;




		std::string filepath;

		OBJ() {
			empty = true;
			pos = glm::vec3(0);
			rot = glm::vec3(0);
		};

		Collision_Object* collision_boxes = nullptr;



		std::string name;
		bool empty = true;
		glm::vec3 center_of_mass;

		glm::vec3 pos;
		glm::vec3 rot;

		std::vector<Mesh>* meshes = nullptr;
		glm::vec3 scale;
	};



}
