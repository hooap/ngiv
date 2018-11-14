#pragma once
#include <glm\glm.hpp>

namespace ngiv {

	class Collision_Sphere
	{
	public:
		Collision_Sphere(glm::vec3 pos, float radius) {
			_pos = pos;
			_radius = radius;
		}
		Collision_Sphere(){	}
		~Collision_Sphere() {}

		void init(glm::vec3 pos, float radius) {
			_pos = pos;
			_radius = radius;
		}

		glm::vec3 getPos() { return _pos; }
		float getRadius() { return _radius; }


		Mesh renderMesh;
	private:

		glm::vec3 _pos;
		float _radius;
	};




	class Collision_Box
	{
	public:
		Collision_Box() {}
		Collision_Box(glm::vec3 pos, glm::vec3 center, std::vector<glm::vec3> vert, glm::vec3 scale) {
			this->vert = vert;
			this->center = center;
			this->pos = pos;
			this->scale = scale;
		}

		~Collision_Box() {}

		void init(glm::vec3 pos, glm::vec3 center, std::vector<glm::vec3> vert, glm::vec3 scale) {
			this->vert = vert;
			this->center = center;
			this->pos = pos;
			this->scale = scale;
		}

		glm::vec3 getPos() { return pos; }
		std::vector<glm::vec3>& getVerticies() { return vert; }
		glm::vec3 getCenter() { return center; }
		glm::vec3 getScale() { return scale; }


		Mesh renderMesh;
	private:
		glm::vec3 scale;
		glm::vec3 pos;
		glm::vec3 center;
		std::vector<glm::vec3> vert;
	};


	class Collision_Object {
	public:
		Collision_Object() {}

		void setDynamic(bool dynamic) {
			_dynamic = dynamic;
		}
		bool getDynamic() { return _dynamic; }

		void addSphere(Collision_Sphere sphere) {
			spheres.push_back(sphere);
		}
		void addBox(Collision_Box box) {
			boxes.push_back(box);
		}


		glm::vec3 getVelocity() { return _velocity; }
		void setVelocity(glm::vec3 newVelocity) { _velocity = newVelocity; }
		void addVelocity(glm::vec3 velocity) { _velocity += velocity; }

		glm::vec3 getExtraPos() {return _extrapos;	}
		void setExtraPos(glm::vec3 newpos) { _extrapos = newpos; }
		void addExtraPos(glm::vec3 pos) { _extrapos += pos; }



		std::vector<Collision_Box>* getBoxes() { return &boxes; }
		std::vector<Collision_Sphere>* getSpheres() { return &spheres; }


		void setRotation(glm::vec3 rotation) { _rotation = rotation; }
		glm::vec3 getRotation() { return _rotation; }

		glm::vec3 getCenterofmass() { return _centerofmass; }
		void setCenterofmass(glm::vec3 cm) { _centerofmass = cm; }


		static std::vector<glm::vec3> makeCollisionBox(glm::vec3 scaling) {
			scaling /= 2.0f;
			std::vector<glm::vec3> verts;


			verts.push_back(glm::vec3(-scaling.x, -scaling.y, -scaling.z));
			verts.push_back(glm::vec3(scaling.x, -scaling.y, -scaling.z));
			verts.push_back(glm::vec3(scaling.x, - scaling.y, scaling.z));
			verts.push_back(glm::vec3(-scaling.x, -scaling.y, scaling.z));

			verts.push_back(glm::vec3(-scaling.x, scaling.y, -scaling.z));
			verts.push_back(glm::vec3(scaling.x, scaling.y, -scaling.z));
			verts.push_back(glm::vec3(scaling.x, scaling.y, scaling.z));
			verts.push_back(glm::vec3(-scaling.x, scaling.y, scaling.z));

			return verts;
		}



	private:


		bool _dynamic = true;
		glm::vec3 _extrapos;


		std::vector<Collision_Box> boxes;
		std::vector<Collision_Sphere> spheres;

		glm::vec3 _velocity;
		glm::vec3 _rotation;
		glm::vec3 _centerofmass;

	};
	


	

}