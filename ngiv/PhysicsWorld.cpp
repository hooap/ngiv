
#include "PhysicsWorld.h"
#include "Misc.h"
#include <glm/glm.hpp>
#include <glm/gtx/intersect.hpp>

namespace ngiv {

	PhysicsWorld::PhysicsWorld()
	{
	}


	PhysicsWorld::~PhysicsWorld()
	{
	}

	void PhysicsWorld::init(float gravity) {
		initialized = true;
		_gravity = gravity;

	}



	glm::vec3 getdistancebetweensphereandplane(glm::vec3 sphereloc, float radius, std::vector<glm::vec3> verts, glm::vec3 boxadditionalpos, glm::vec3 boxrotation) {

		glm::vec3 vert0 = verts[0];
		glm::vec3 vert1 = verts[1];
		glm::vec3 vert2 = verts[2];
		glm::vec3 vert3 = verts[3];


		glm::vec3 p = vert0 + vert1 + vert2 + vert3;
		p /= 4.0f;
		glm::vec3 normal = glm::cross(glm::vec3(vert3 - vert0), glm::vec3(vert1 - vert0));


		vert0 = p - vert0;
		vert1 = p - vert1;
		vert2 = p - vert2;
		vert3 = p - vert3;

		float l0 = glm::length(vert0) + radius;
		float l1 = glm::length(vert1) + radius;
		float l2 = glm::length(vert2) + radius;
		float l3 = glm::length(vert3) + radius;

		vert0 = glm::normalize(vert0) * l0 + p;
		vert1 = glm::normalize(vert1) * l1 + p;
		vert2 = glm::normalize(vert2) * l2 + p;
		vert3 = glm::normalize(vert3) * l3 + p;


		glm::vec3 fp;
		bool i = glm::intersectRayTriangle(sphereloc, -normal, vert0 + boxadditionalpos, vert3 + boxadditionalpos, vert2 + boxadditionalpos, fp);
		if (i != 1) {
			i = glm::intersectRayTriangle(sphereloc, -normal, vert0 + boxadditionalpos, vert2 + boxadditionalpos, vert1 + boxadditionalpos, fp);
			if (i != 1) {
				return glm::vec3(0);
			}
		}


		glm::vec3 distancefromcenters = sphereloc - (p + boxadditionalpos);
		normal = glm::normalize(normal);
		float angle = glm::dot(normal, glm::normalize(distancefromcenters));

		normal *= (angle * glm::length(distancefromcenters));

		float l = glm::length(normal) - radius;

		if (l < 0) {
			return normal;
		}
		return glm::vec3(0, 0, 0);
	}


	void PhysicsWorld::collision_sphere_box(Collision_Sphere* s, Collision_Box* b, Collision_Object* o1, Collision_Object* o2) {
		//check for each side

		glm::vec3 firstp = o1->getExtraPos();
		glm::vec3 secondp = o2->getExtraPos();

		bool firstdynamic = o1->getDynamic();
		bool seconddynamic = o2->getDynamic();


		float fr = s->getRadius();
		for (int f = 0; f < 6; f++) {
			std::vector<glm::vec3> face = b->getfacebyindex(f);
			glm::vec3 v = getdistancebetweensphereandplane(s->getPos() + firstp, fr, face, secondp + b->getPos(), o2->getRotation() + b->getRotation());
			
			if (v != glm::vec3(0, 0, 0)) {
				//reflect the sphere if its dynamic

				o1->setExtraPos(firstp + glm::normalize(v) * (fr - glm::length(v)));

				glm::vec3 vel1 = o1->getVelocity();
				glm::vec3 vel2 = o2->getVelocity();

				float v1 = glm::length(vel1);
				float v2 = glm::length(vel2);


				float m1 = 1.0f;
				float m2 = 1.0f;
				glm::vec3 mom1 = m1 * vel1;
				glm::vec3 mom2 = m2 * vel2;

				glm::vec3 lastmom = mom1 + mom2;
				glm::vec3 tvel = vel1 - vel2;


				if (firstdynamic && seconddynamic) {
					vel1 = glm::reflect(mom1, -v);
					vel1 *= m1 / m2;

					vel2 = glm::reflect(mom2, -v);
					vel2 *= m2 / m1;

					vel1 += mom2 - vel2;
					vel2 += mom1 - vel1;

					vel1 /= m1;
					vel2 /= m2;
					o1->setVelocity(vel1);
					o2->setVelocity(vel2);
				}
				else if (firstdynamic) {

					vel1 = glm::reflect(mom1, -v);

					vel1 += mom2 - vel2;
					vel1 /= m1;

					o1->setVelocity(vel1);
				}
				else if (seconddynamic) {


				}			   
				

			}
			
		}

	}
	void PhysicsWorld::collision_sphere_sphere(Collision_Sphere* s1, Collision_Sphere* s2, Collision_Object* o1, Collision_Object* o2) {
				
		//check distance between spheres
		glm::vec3 firstp = o1->getExtraPos();
		glm::vec3 secondp = o2->getExtraPos();

		glm::vec3 ppf = s1->getPos() + firstp;
		glm::vec3 pps = s2->getPos() + secondp;

		float distance = glm::length(ppf - pps);
		float sumradius = s1->getRadius() + s2->getRadius();

		if (distance <= sumradius) {
			//3d collision
			std::cout << "COLLISION";
			//lets get 3d vector of collision

			glm::vec3 vel1 = o1->getVelocity();
			glm::vec3 vel2 = o2->getVelocity();

			float v1 = glm::length(vel1);
			float v2 = glm::length(vel2);

			glm::vec3 centerofmass1 = o1->getCenterofmass() + firstp;
			glm::vec3 centerofmass2 = o2->getCenterofmass() + secondp;

			glm::vec3 normal = centerofmass1 - centerofmass2;
			normal = glm::normalize(normal);
			float m1 = 1.0f;
			float m2 = 1.0f;

			glm::vec3 mom1 = m1 * vel1;
			glm::vec3 mom2 = m2 * vel2;

			glm::vec3 lastmom = mom1 + mom2;

			glm::vec3 tvel = vel1 - vel2;

			//lets look from object one
			vel1 = glm::reflect(mom1, -normal);
			vel1 *= m1 / m2;

			vel2 = glm::reflect(mom2, -normal);
			vel2 *= m2 / m1;

			vel1 += mom2 - vel2;
			vel2 += mom1 - vel1;

			vel1 /= m1;
			vel2 /= m2;

			o1->setVelocity(vel1);
			o2->setVelocity(vel2);
		}
		
	}
	void PhysicsWorld::collision_box_box(Collision_Box* b1, Collision_Box* b2, Collision_Object* o1, Collision_Object* o2){
		//implement this


		return;
	}

	void PhysicsWorld::update(bool gravit) {
		for (int i = 0; i < objects.size(); i++) {

			//add gravity if dynamic
			if (gravit) {
				if (objects[i]->getDynamic()) {
					objects[i]->addVelocity(glm::vec3(0, -_gravity, 0));
				}
			}
			//move the objects along velocity
			objects[i]->addExtraPos(objects[i]->getVelocity());


			float slowingfactor = 0.01;
			//stop objects slowly
			objects[i]->addVelocity(-objects[i]->getVelocity() * slowingfactor);




			for (int j = i + 1; j < objects.size(); j++) {
				//collision between spheres


				std::vector<Collision_Sphere>* firstSpheres = objects[i]->getSpheres();
				std::vector<Collision_Sphere>* secondSpheres = objects[j]->getSpheres();

				std::vector<Collision_Box>* firstBoxes = objects[i]->getBoxes();
				std::vector<Collision_Box>* secondBoxes = objects[j]->getBoxes();
				

				//i and j objects
				//lets get individual collision boxes with each other

				for (int k = 0; k < firstSpheres->size(); k++) {	

					for (int l = 0; l < secondSpheres->size(); l++) {
						collision_sphere_sphere( &(*firstSpheres)[k], &(*secondSpheres)[l], objects[i], objects[j]);
					}
					for (int l = 0; l < secondBoxes->size(); l++) {
						collision_sphere_box(&(*firstSpheres)[k], &(*secondBoxes)[l], objects[i], objects[j]);
					}

				}

				for (int k = 0; k < firstBoxes->size(); k++) {
					for (int l = 0; l < secondSpheres->size(); l++) {
						collision_sphere_box(&(*secondSpheres)[l], &(*firstBoxes)[k], objects[j], objects[i]);
					}
					for (int l = 0; l < secondBoxes->size(); l++) {
						collision_box_box(&(*secondBoxes)[k], &(*firstBoxes)[k], objects[j], objects[i]);
					}
				}




			}			

		}


	}

}