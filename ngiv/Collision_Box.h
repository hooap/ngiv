#pragma once
#include <glm/glm.hpp>

namespace ngiv {

	class Collision_Sphere
	{
	public:
		Collision_Sphere(glm::vec3 pos, float radius) {
			_pos = pos;
			_radius = radius;
		}
		Collision_Sphere() {	}
		~Collision_Sphere() {}

		void init(glm::vec3 pos, float radius) {
			_pos = pos;
			_radius = radius;
		}

		glm::vec3 getPos() { return _pos; }
		float getRadius() { return _radius; }
		void setPos(glm::vec3 pos) { _pos = pos; }


		Mesh renderMesh;
	private:

		glm::vec3 _pos;
		float _radius;
	};




	class Collision_Box
	{
	public:
		Collision_Box() {}
		Collision_Box(glm::vec3 pos, glm::vec3 center, std::vector<glm::vec3> vert, glm::vec3 scale = glm::vec3(1), glm::vec3 rot = glm::vec3(0)) {
			init(pos, center, vert, scale, rot);
		}

		~Collision_Box() {}

		void init(glm::vec3 pos, glm::vec3 center, std::vector<glm::vec3> vert, glm::vec3 scale = glm::vec3(1), glm::vec3 rot = glm::vec3(0)) {
			this->vert = vert;
			this->center = center;
			this->pos = pos;
			this->scale = scale;
			this->rot = rot;
			calculatefaces();
		}

		glm::vec3 getPos() { return pos; }
		std::vector<glm::vec3>& getVerticies() { return vert; }
		glm::vec3 getCenter() { return center; }
		glm::vec3 getScale() { return scale; }
		glm::vec3 getRotation() { return rot; }


		void setPos(glm::vec3 pos) { this->pos = pos; }

		std::vector<glm::vec3> getfacebyindex(int i) {
			return vertfaces[i];
		}

		Mesh renderMesh;
	private:


		/*
		0 bottom
		1 front
		2 right
		3 back
		4 left
		5 up
		*/
		//counter clockwise
		void calculatefaces() {
			vertfaces.emplace_back();
			vertfaces.back().push_back(vert[0]);
			vertfaces.back().push_back(vert[3]);
			vertfaces.back().push_back(vert[2]);
			vertfaces.back().push_back(vert[1]);
			vertfaces.emplace_back();
			vertfaces.back().push_back(vert[0]);
			vertfaces.back().push_back(vert[4]);
			vertfaces.back().push_back(vert[5]);
			vertfaces.back().push_back(vert[1]);
			vertfaces.emplace_back();
			vertfaces.back().push_back(vert[1]);
			vertfaces.back().push_back(vert[5]);
			vertfaces.back().push_back(vert[6]);
			vertfaces.back().push_back(vert[2]);
			vertfaces.emplace_back();
			vertfaces.back().push_back(vert[2]);
			vertfaces.back().push_back(vert[3]);
			vertfaces.back().push_back(vert[7]);
			vertfaces.back().push_back(vert[6]);
			vertfaces.emplace_back();
			vertfaces.back().push_back(vert[0]);
			vertfaces.back().push_back(vert[4]);
			vertfaces.back().push_back(vert[7]);
			vertfaces.back().push_back(vert[3]);
			vertfaces.emplace_back();
			vertfaces.back().push_back(vert[5]);
			vertfaces.back().push_back(vert[6]);
			vertfaces.back().push_back(vert[7]);
			vertfaces.back().push_back(vert[4]);
		}

		glm::vec3 rot;
		glm::vec3 scale;
		glm::vec3 pos;
		glm::vec3 center;
		std::vector<glm::vec3> vert;
		std::vector<std::vector<glm::vec3>> vertfaces;



	};


	class Collision_Object {
	public:
		Collision_Object() {
			_velocity = glm::vec3(0);
			_rotation = glm::vec3(0);
			_centerofmass = glm::vec3(0);
			_extrapos = glm::vec3(0);
		}
		Collision_Object(bool dynamic) {
			_velocity = glm::vec3(0);
			_rotation = glm::vec3(0);
			_centerofmass = glm::vec3(0);
			_extrapos = glm::vec3(0);
			_dynamic = dynamic;
		}



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

		glm::vec3 getExtraPos() { return _extrapos; }
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
			verts.push_back(glm::vec3(scaling.x, -scaling.y, scaling.z));
			verts.push_back(glm::vec3(-scaling.x, -scaling.y, scaling.z));

			verts.push_back(glm::vec3(-scaling.x, scaling.y, -scaling.z));
			verts.push_back(glm::vec3(scaling.x, scaling.y, -scaling.z));
			verts.push_back(glm::vec3(scaling.x, scaling.y, scaling.z));
			verts.push_back(glm::vec3(-scaling.x, scaling.y, scaling.z));

			return verts;
		}

		unsigned int getRendererID() { return __renderer_id; }
		void setRendererID(unsigned int id) { __renderer_id = id; }


	private:
		unsigned int __renderer_id = -1;



		bool _dynamic = false;
		glm::vec3 _extrapos;


		std::vector<Collision_Box> boxes;
		std::vector<Collision_Sphere> spheres;

		glm::vec3 _velocity;
		glm::vec3 _rotation;
		glm::vec3 _centerofmass;

	};





}
