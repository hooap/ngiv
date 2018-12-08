#pragma once
#include "OBJ.h"
#include <vector>
#include "TerrainGenerator.h"
#include "Collision_Box.h"

namespace ngiv {


	struct BodyStruct {
		glm::vec3 centerofmass;

		glm::vec3 pos;
		glm::vec3 rot;

		glm::vec3* rotPointer;
		glm::vec3* posPointer;

		glm::vec3 dir;
		float velocity;
		bool dynamic;

	};

	class PhysicsWorld
	{
	public:
		PhysicsWorld();
		~PhysicsWorld();

		void init(float gravity);
		

		//DO NOT MOVE THE OBJECTS OUTSIDE OF PHYSICS WORLD
		void add(Collision_Object* object) {
			objects.push_back(object);
		}	

		//DO NOT MOVE THE OBJECTS OUTSIDE OF PHYSICS WORLD
	

		void update(bool gravit);

		void clear() {
			objects.clear();
		}

	private:

		void collision_sphere_box(Collision_Sphere* s, Collision_Box* b, Collision_Object* o1, Collision_Object* o2);
		void collision_sphere_sphere(Collision_Sphere* s1, Collision_Sphere* s2, Collision_Object* o1, Collision_Object* o2);
		void collision_box_box(Collision_Box* b1, Collision_Box* b2, Collision_Object* o1, Collision_Object* o2);


		bool initialized = false;

		std::vector<Collision_Object*> objects;

		float _gravity;

	};



}