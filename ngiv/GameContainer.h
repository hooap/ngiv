#pragma once
#include <string>
#include "OBJ.h"
#include <vector>
#include <fstream>
#include "Misc.h"
#include "ModelLoader.h"
#include "PhysicsWorld.h"

namespace ngiv {

	class GameContainer
	{
	public:
		GameContainer(){}
		~GameContainer(){}

		void loadfromfile(std::string filename) {
			std::ifstream inp;
			inp.open(filename, std::ofstream::binary);

			if (inp.fail()) {
				ngiv::o("ERROR LOAD FILE COULDNT OPENED");
				perror(filename.c_str());
				return;
			}

			std::stringstream buffer;
			buffer << inp.rdbuf();
			std::string data = buffer.str();

			int index = data.find("#");
			while (index != data.npos) {
				int nextindex = data.find("#", index + 2);
				index++;


				std::string name = getstringfromstring(data, index);
				glm::vec3 pos = getvec3fromstring(data, index);
				glm::vec3 rot = getvec3fromstring(data, index);
				glm::vec3 centerpos = getvec3fromstring(data, index);
				glm::vec3 scale = getvec3fromstring(data, index);
				std::string filepath = getstringfromstring(data, index);


				glm::vec3 cobpos = getvec3fromstring(data, index);
				glm::vec3 cobrot = getvec3fromstring(data, index);
				glm::vec3 cobcenter = getvec3fromstring(data, index);
				glm::vec3 cobveloc = getvec3fromstring(data, index);
				bool cobdynam = getboolfromstring(data, index);


				//create stuff
				OBJ* t = ModelLoader::loadModel(name,pos, filepath, true, scale);
				t->setRot(rot);
				t->setCenterRelative(centerpos);


				//create collision object
				Collision_Object* cob = new Collision_Object(cobdynam);
				cob->setExtraPos(cobpos);
				cob->setRotation(cobrot);
				cob->setCenterofmass(cobcenter);
				cob->setVelocity(cobveloc);

				int bind = index;
				index = data.find("[",index);

				while (index != data.npos && (index < nextindex || nextindex == data.npos)) {
					index++;
					glm::vec3 boxpos = getvec3fromstring(data, index);
					glm::vec3 boxrot = getvec3fromstring(data, index);
					glm::vec3 boxcenter = getvec3fromstring(data, index);
					glm::vec3 boxscale = getvec3fromstring(data, index);

					Collision_Box box(boxpos, boxcenter, Collision_Object::makeCollisionBox(boxscale), boxscale, boxrot);
					cob->addBox(box);
					index = data.find("[",index);
				}
 				index = bind;
				index = data.find("(",index);

				while (index != data.npos && (index < nextindex || nextindex == data.npos)) {
					index++;
					glm::vec3 sphpos = getvec3fromstring(data, index);
					float sphradius = getfloatfromstring(data, index);


					Collision_Sphere sphere(sphpos, sphradius);
					cob->addSphere(sphere);
					index = data.find("(",index);
				}
				t->setCollisionObject(cob);
				//object is ready
				objs.push_back(t);

				index = nextindex;
			}


		}

		void savetofile(std::string filename) {

			std::ofstream of;
			of.open(filename, std::ofstream::binary);

			if (of.fail()) {
				ngiv::o("ERROR SAVE FILE COULDNT OPENED");
				perror(filename.c_str());
				return;
			}


			std::string data = "";
			for (int i = 0; i < objs.size(); i++) {
				OBJ* o = objs[i];
				data.append("#");

				addtostring(data, o->getName());
				//write pos;
				addtostring(data, o->getPos());
				//write rot;
				addtostring(data, o->getRot());
				//write centerpos
				addtostring(data, o->getCenterPosRelative());
				//write scale
				addtostring(data, o->getscale());
				//write filepath
				addtostring(data, o->getfilepath());
				//write collision boxs

				Collision_Object* cobject = objs[i]->getCollision_Object();
				std::vector<Collision_Box>* cobox = cobject->getBoxes();
				std::vector<Collision_Sphere>* cosph = cobject->getSpheres();

				//extrapos
				addtostring(data, cobject->getExtraPos());
				//rotation
				addtostring(data, cobject->getRotation());
				//centerofmass
				addtostring(data, cobject->getCenterofmass());
				//velocity
				addtostring(data, cobject->getVelocity());
				//dynamic
				addtostring(data, cobject->getDynamic());

				//collisionboxes
				for (int j = 0; j < cobox->size(); j++) {
					addtostring(data, (std::string)"[", false);

					Collision_Box box = (*cobox)[j];

					//pos
					addtostring(data, box.getPos());
					//rot
					addtostring(data, box.getRotation());
					//centerofmass
					addtostring(data, box.getCenter());
					//scale
					addtostring(data, box.getScale());
				}
				for (int j = 0; j < cosph->size(); j++) {
					addtostring(data, (std::string)"(", false);
					Collision_Sphere sphere = (*cosph)[j];
					//pos
					addtostring(data, sphere.getPos());
					//rot
					addtostring(data, sphere.getRadius());
				}

			}

			of << data;
			of.close();

		}

		void draw_setalways(ngiv::Renderer_3D& rend) {
			for (int i = 0; i < objs.size(); i++) {
				rend.addtodraw(objs[i]);
			}
		}

		void draw_setcollisionalways(ngiv::Renderer_3D& rend) {
			for (int i = 0; i < objs.size(); i++) {
				rend.addtodrawCollisionBox(objs[i]->getCollision_Object());
			}
		}

		void addObj(OBJ* obj) {
			objs.push_back(obj);
		}

		void addalltoWorld(PhysicsWorld& world) {
			for (int i = 0; i < objs.size(); i++) {
				world.add(objs[i]->getCollision_Object());
			}
		}


		OBJ* getObjbyIndex(int i) { return objs[i]; }
		OBJ* getObjbyName(std::string name) {
			for (int i = 0; i < objs.size(); i++) {
				if (objs[i]->getName() == name) {
					return objs[i];
				}
			}

			throw "OBJECT DOESNT EXIST";

			return NULL;
		}


		void clear_delete() {
			for (int i = 0; i < objs.size(); i++) {
				delete objs[i];
			}
			objs.clear();
		}
		void clear() {
			objs.clear();
		}
		void updateall() {
			for (int i = 0; i < objs.size(); i++) {
				objs[i]->updateObject();
			}
		}



	private:

		void addtostring(std::string& d, glm::vec3 v) {
			d.append(std::to_string(v.x));
			d.append("+");
			d.append(std::to_string(v.y));
			d.append("+");
			d.append(std::to_string(v.z));
			d.append("+");
		}

		void addtostring(std::string& d, std::string s, bool addplus = true) {
			d.append(s);
			if (addplus) { d.append("+"); }

		}
		void addtostring(std::string& d, bool v) {
			d.append(std::to_string((int)v));
			d.append("+");
		}
		void addtostring(std::string& d, float v) {
			d.append(std::to_string(v));
			d.append("+");
		}


		std::string getstringfromstring(std::string& d, int& index) {
			int end = d.find("+", index);
			std::string s = d.substr(index, end - index);
			index = end + 1;
			return s;
		}
		glm::vec3 getvec3fromstring(std::string& d, int& index) {
			int end1 = d.find("+", index);
			int end2 = d.find("+", end1 + 1);
			int end3 = d.find("+", end2 + 1);

			glm::vec3 vec;
			std::string s1 = d.substr(index, end1 - index);
			std::string s2 = d.substr(end1 + 1, end2 - end1 - 1);
			std::string s3 = d.substr(end2 + 1, end3 - end2 - 1);


			vec.x = std::stof(s1);
			vec.y = std::stof(s2);
			vec.z = std::stof(s3);

			index = end3 + 1;
			return vec;
		}
		bool getboolfromstring(std::string& d, int& index) {

			char c = d[index] - 48;
			index += 1;
			if (c) {
				return true;
			}
			return false;

		}
		float getfloatfromstring(std::string& d, int& index) {
			int end1 = d.find("+", index);

			std::string st = d.substr(index, end1 - index);
			index = end1 + 1;
			return std::stof(st);
		}


		std::vector<OBJ*> objs;

	};






}
