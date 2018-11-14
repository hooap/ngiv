#include "PhysicsWorld.h"


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

	void PhysicsWorld::update() {
		for (int i = 0; i < objects.size(); i++) {
			for (int j = i + 1; j < objects.size(); j++) {
				//collision between spheres

				glm::vec3 extrafirstpos = objects[i]->getExtraPos();
				glm::vec3 extrasecondpos = objects[j]->getExtraPos();

				std::vector<Collision_Sphere>* firstSpheres = objects[i]->getSpheres();
				std::vector<Collision_Sphere>* secondSpheres = objects[j]->getSpheres();



				std::vector<Collision_Box>* secondBoxes = objects[j]->getBoxes();

				//i and j objects
				//lets get individual collision boxes with each other
				for (int k = 0; k < firstSpheres->size(); k++) {
					Collision_Sphere* first = &(*firstSpheres)[k];
					for (int l = 0; l < secondSpheres->size(); l++) {
						//check collision between these collision boxes
						Collision_Sphere* second = &(*secondSpheres)[l];

						//check distance between spheres

						glm::vec3 ppf = first->getPos() + extrafirstpos;
						glm::vec3 pps = second->getPos() + extrasecondpos;

						float distance = glm::length(ppf - pps);
						float sumradius = first->getRadius() + second->getRadius();

						if (distance <= sumradius) {
							//3d collision
							std::cout << "COLLISION";

							//lets get 3d vector of collision


							glm::vec3 vel1 = objects[i]->getVelocity();
							glm::vec3 vel2 = objects[j]->getVelocity();

							float v1 = glm::length(vel1);
							float v2 = glm::length(vel2);


							glm::vec3 centerofmass1 = objects[i]->getCenterofmass() + ppf;
							glm::vec3 centerofmass2 = objects[j]->getCenterofmass() + pps;
							
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

							objects[i]->setVelocity(vel1);
							objects[j]->setVelocity(vel2);			
						}
					}


					//collision between spheres and boxes
					for (int l = 0; l < secondBoxes->size(); l++) {
						Collision_Box* second = &(*secondBoxes)[l];


					}





				}


				



			}
			//move the objects along velocity
			objects[i]->addExtraPos(objects[i]->getVelocity());

			//add gravity if dynamic
			if (objects[i]->getDynamic()) {
	//			objects[i]->addVelocity(glm::vec3(0, -_gravity, 0));
			}

			float slowingfactor = 0.01;
			//stop objects slowly
			objects[i]->addVelocity(-objects[i]->getVelocity() * slowingfactor);

		}


	}

}