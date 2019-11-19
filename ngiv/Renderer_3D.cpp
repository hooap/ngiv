#include "Renderer_3D.h"
#include "Misc.h"
#include <functional>
#include <SDL/SDL_keycode.h>
#include "IOManager.h"
#include "TextureLoader.h"
#include "stb_image.h"
#include <numeric>
#include <GLAD/glad.h>
#include <math.h>

namespace ngiv {

	// -----------------------------------------renderQuad() renders a 1x1 XY quad in NDC
	unsigned int quadVAO = 0;
	unsigned int quadVBO;
	void renderQuad()
	{
		if (quadVAO == 0)
		{
			float quadVertices[] = {
				// positions        // texture Coords
				-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
				1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			};

			std::vector<unsigned int> indicies;
			indicies.push_back(0);
			indicies.push_back(1);
			indicies.push_back(3);
			indicies.push_back(0);
			indicies.push_back(3);
			indicies.push_back(2);


			// setup plane VAO
			glGenVertexArrays(1, &quadVAO);
			glGenBuffers(1, &quadVBO);
			glBindVertexArray(quadVAO);

			glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		}
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	//	glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	// -------------------------------------------------
	unsigned int cubeVAO = 0;
	unsigned int cubeVBO = 0;
	void renderCube()
	{
		// initialize (if necessary)
		if (cubeVAO == 0)
		{
			float vertices[] = {
				// back face
				-1.0f, -1.0f, -1.0f,
				1.0f,  1.0f, -1.0f,
				1.0f, -1.0f, -1.0f,
				1.0f,  1.0f, -1.0f,
				-1.0f, -1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,

						// front face
						-1.0f, -1.0f,  1.0f,
						1.0f, -1.0f,  1.0f,
						1.0f,  1.0f,  1.0f,
						1.0f,  1.0f,  1.0f,
						-1.0f,  1.0f,  1.0f,
						-1.0f, -1.0f,  1.0f,
						// left face
						-1.0f,  1.0f,  1.0f,
						-1.0f,  1.0f, -1.0f,
						-1.0f, -1.0f, -1.0f,
						-1.0f, -1.0f, -1.0f,
						-1.0f, -1.0f,  1.0f,
						-1.0f,  1.0f,  1.0f,
								// right face
								1.0f,  1.0f,  1.0f,
								1.0f, -1.0f, -1.0f,
								1.0f,  1.0f, -1.0f,
								1.0f, -1.0f, -1.0f,
								1.0f,  1.0f,  1.0f,
								1.0f, -1.0f,  1.0f,
																					// bottom face
							-1.0f, -1.0f, -1.0f,
							1.0f, -1.0f, -1.0f,
							1.0f, -1.0f,  1.0f,
							1.0f, -1.0f,  1.0f,
							-1.0f, -1.0f,  1.0f,
							-1.0f, -1.0f, -1.0f,
																					// top face
						-1.0f,  1.0f, -1.0f,
						1.0f,  1.0f , 1.0f,
						1.0f,  1.0f, -1.0f,
						1.0f,  1.0f,  1.0f,
						-1.0f,  1.0f, -1.0f,
						-1.0f,  1.0f,  1.0f,
};
			glGenVertexArrays(1, &cubeVAO);
			glGenBuffers(1, &cubeVBO);

			glBindVertexArray(cubeVAO);

			glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

			// link vertex attributes
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glBindVertexArray(0);

		}
		// render Cube
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}


	Mesh getSphereMesh(float radius, unsigned int rings, unsigned int sectors)
	{
		static std::map<std::pair<float, float>, Mesh> map;

		auto it = map.find(std::make_pair(rings,sectors));
		if (it != map.end()) {
			return it->second;
		}
		Mesh m;

		std::vector<Vertex3D> verticies;

		for (int i = 0; i < rings; i++) {
			for (int j = 0; j < sectors; j++) {
				Vertex3D vertex;
				vertex.TexCoords = glm::vec2(0);
				vertex.Normal = glm::vec3(0);

				float xpercent = (float)i / (rings);
				float ypercent = (float)j / (sectors);


				if (i == 14 && j == 14)
					int jj = 0;


				float xdegree = (float)(2 * xpercent * M_PI);
				float ydegree = (float)(2 * ypercent * M_PI);

				float xt = cos(xdegree);
				float yt = cos(ydegree);
				float zt = sin(xdegree);

				float y = yt;
				float left = 1 - pow(y, 2);

				float x = left * xt;
				float z = left * zt;

				vertex.Position.y = y* radius;
				vertex.Position.x = x* radius;
				vertex.Position.z = z* radius;
				m.vertices.push_back(vertex);
			}
		}
		map.insert(std::make_pair(std::make_pair(rings, sectors), m));
		return m;
	}
	Mesh getBoxMesh(glm::vec3 scale) {
		static glm::vec3 lastscale = glm::vec3(0);
		static Mesh m;

		if (lastscale == scale) {
			return m;
		}

		//last scale is not equal to this scale so lets look in to the map
		static std::map<std::pair<float,std::pair<float,float>>, Mesh> map;

		auto it = map.find(std::make_pair(scale.x, std::make_pair(scale.y, scale.z)));
		if (it != map.end()) {
			m = it->second;
			lastscale = scale;

			return it->second;
		}
		//box doesnt exist lets create
		Mesh newm;

		std::vector<Vertex3D> verticies;


		Vertex3D v0,v1,v2,v3,v4,v5,v6,v7;

		{
			v0.Position = glm::vec3(-scale.x, -scale.y, -scale.z);
			v1.Position = glm::vec3(scale.x, -scale.y, -scale.z);
			v2.Position = glm::vec3(scale.x, -scale.y, scale.z);
			v3.Position = glm::vec3(-scale.x, -scale.y, scale.z);

			v4.Position = glm::vec3(-scale.x, scale.y, -scale.z);
			v5.Position = glm::vec3(scale.x, scale.y, -scale.z);
			v6.Position = glm::vec3(scale.x, scale.y, scale.z);
			v7.Position = glm::vec3(-scale.x, scale.y, scale.z);
		}

		{
			//alt
			verticies.push_back(v0);
			verticies.push_back(v2);
			verticies.push_back(v1);
			verticies.push_back(v0);
			verticies.push_back(v3);
			verticies.push_back(v2);
			//ön
			verticies.push_back(v0);
			verticies.push_back(v1);
			verticies.push_back(v4);
			verticies.push_back(v1);
			verticies.push_back(v5);
			verticies.push_back(v4);
			//sað
			verticies.push_back(v1);
			verticies.push_back(v2);
			verticies.push_back(v5);
			verticies.push_back(v2);
			verticies.push_back(v6);
			verticies.push_back(v5);
			//sol
			verticies.push_back(v0);
			verticies.push_back(v4);
			verticies.push_back(v7);
			verticies.push_back(v0);
			verticies.push_back(v7);
			verticies.push_back(v3);
			//arka
			verticies.push_back(v2);
			verticies.push_back(v3);
			verticies.push_back(v6);
			verticies.push_back(v3);
			verticies.push_back(v7);
			verticies.push_back(v6);
			//üst
			verticies.push_back(v5);
			verticies.push_back(v7);
			verticies.push_back(v4);
			verticies.push_back(v5);
			verticies.push_back(v6);
			verticies.push_back(v7);
		}

		std::vector<Texture> t;
		std::vector<unsigned int> i;
		newm.init(t, i, verticies);
		map.insert(std::make_pair(std::make_pair(scale.x, std::make_pair(scale.y, scale.z)), m));
		m = newm;
		lastscale = scale;
		return newm;
	}
	Mesh getBoxMesh(std::vector<glm::vec3> vert) {
		static glm::vec3 lastscale = glm::vec3(0);
		static Mesh m;

		//box doesnt exist lets create
		Mesh newm;

		std::vector<Vertex3D> verticies;

		Vertex3D v0, v1, v2, v3, v4, v5, v6, v7;

		{
			v0.Position = vert[0];
			v1.Position = vert[1];
			v2.Position = vert[2];
			v3.Position = vert[3];

			v4.Position = vert[4];
			v5.Position = vert[5];
			v6.Position = vert[6];
			v7.Position = vert[7];
		}

		{
			verticies.push_back(v0);
			verticies.push_back(v1);
			verticies.push_back(v3);
			verticies.push_back(v2);
			verticies.push_back(v7);
			verticies.push_back(v6);
			verticies.push_back(v3);
			verticies.push_back(v7);
			verticies.push_back(v4);
			verticies.push_back(v3);
			verticies.push_back(v0);
			verticies.push_back(v4);
			verticies.push_back(v5);
			verticies.push_back(v1);
			verticies.push_back(v2);
			verticies.push_back(v2);
			verticies.push_back(v5);
			verticies.push_back(v6);
			verticies.push_back(v2);
			//end but add more to show
			verticies.push_back(v0);
			verticies.push_back(v5);
			verticies.push_back(v7);
			verticies.push_back(v6);
			verticies.push_back(v4);
		}

		std::vector<Texture> t;
		std::vector<unsigned int> i;
		newm.init(t, i, verticies);
		return newm;
	}

	Renderer_3D::Renderer_3D()	{
	}
	Renderer_3D::~Renderer_3D()	{
		dispose();
	}


	void Renderer_3D::init(Camera3D* cam, int width, int height, GLSLProgram* glsl) {
		if(initialized){
			throw "Already initialized";
		}
		initialized = true;

		_width = width;
		_height = height;

		_cam = cam;

		if (glsl != nullptr) {
			throw "Custom GLSL not implemented";
		}

		ownglsl = true;

		//coloring glsls
		_shading_glsl.compileShaders("Shaders//default_3DShading.vs", "Shaders//default_3DShading.fs");
		_shading_glsl.addAttribute("aPos");
		_shading_glsl.addAttribute("aTexCoords");
		_shading_glsl.linkShaders();


		//writing data glsls
		_g_glsl.compileShaders("Shaders//default_Gshader.vs", "Shaders//default_Gshader.fs");
		_g_glsl.addAttribute("aPos");
		_g_glsl.addAttribute("aNormal");
		_g_glsl.addAttribute("aTexCoords");
		_g_glsl.linkShaders();

		_g_glsl_instanced.compileShaders("Shaders//default_Gshader_instanced.vs", "Shaders//default_Gshader_instanced.fs","Shaders//default_Gshader_instanced.gs");
		_g_glsl_instanced.addAttribute("aPos");
		_g_glsl_instanced.addAttribute("aTexCoords");
		_g_glsl_instanced.addAttribute("aIndex");
		_g_glsl_instanced.addAttribute("aOffset4y");
		_g_glsl_instanced.addAttribute("aOffsetxz");
		_g_glsl_instanced.linkShaders();



		// util glsls
		_lightbox_glsl.compileShaders("Shaders//default_LightBox.vs", "Shaders//default_LightBox.fs");
		_lightbox_glsl.addAttribute("aPos");
		_lightbox_glsl.linkShaders();

		_skybox_glsl.compileShaders("Shaders//default_Skybox.vs", "Shaders//default_Skybox.fs");
		_skybox_glsl.addAttribute("aPos");
		_skybox_glsl.linkShaders();
		//


		for (int i = 0; i < 1; i++) {
			lightpositions.push_back(glm::vec3(2, 6, 1));
			lightcolors.push_back(glm::vec3(1, 1, 1));
		}

		//create VAO
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &VBO_INS_D);
		glGenBuffers(1, &VBO_INS_I);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		// set the vertex attribute pointers
		// vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, Vertex3D::Position));
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, Vertex3D::Normal));
		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, Vertex3D::TexCoords));
		glBindVertexArray(0);

		//CREATE INSTANCED VBO
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_INS_D);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D_Instanced), (void*)offsetof(Vertex3D_Instanced, Vertex3D_Instanced::Position));

		// vertex texture coords
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D_Instanced), (void*)offsetof(Vertex3D_Instanced, Vertex3D_Instanced::TexCoords));
		//set offset
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex3D_Instanced), (void*)offsetof(Vertex3D_Instanced, Vertex3D_Instanced::index));

		glBindBuffer(GL_ARRAY_BUFFER, VBO_INS_I);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Instance_Offset_Data), (void*) offsetof(Instance_Offset_Data, Instance_Offset_Data::y4offset));

		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Instance_Offset_Data), (void*) offsetof(Instance_Offset_Data, Instance_Offset_Data::xzoffset));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);

		glBindVertexArray(0);


		//get default depth size
		GLint defaultdepth = 0;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_DEPTH, GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE,&defaultdepth);
		o("\ndefault depth : " + (int)defaultdepth);
		//defaultdepth = 32;

		//Create g_buffer

		glGenFramebuffers(1, &gBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

		// - position color buffer
		glGenTextures(1, &gPosition);
		glBindTexture(GL_TEXTURE_2D, gPosition);

		//GL_RGB16F
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _width, _height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
		// _g_glsl
		// - normal color buffer
		glGenTextures(1, &gNormal);
		glBindTexture(GL_TEXTURE_2D, gNormal);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _width, _height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

		// - color + specular color buffer
		glGenTextures(1, &gAlbedoSpec);
		glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);
		// - tell OpenGL which color attachments we'll use (of this framebuffer) for rendering

		//DEAL WITH DEPTH BUFFER
		glGenRenderbuffers(1, &rboDepth);
		glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);

		switch (defaultdepth)
		{
		case 16:
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, _width, _height);
			break;
		case 24:
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, _width, _height);
			break;
		case 32:
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, _width, _height);
			break;
		default:
			error("unsupported depth size:" + defaultdepth, true);
			break;
		}

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

		unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
		glDrawBuffers(3, attachments);

		// finally check if framebuffer is complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void Renderer_3D::loadSkybox(std::string path, std::vector<std::string> facenames) {
		if (facenames.empty()) {

        #ifdef __linux__
        path = ".//" + path;
        #endif // __linux__

			std::vector<std::string> defaultfaces = {
			"right.jpg",
				"left.jpg",
				"top.jpg",
				"bottom.jpg",
				"front.jpg",
				"back.jpg"
			};
			facenames = defaultfaces;
		}

		path = "Skybox//" + path;
		int width, height, nrChannels;
		unsigned int tid;
		glGenTextures(1, &tid);
		glBindTexture(GL_TEXTURE_CUBE_MAP, tid);

		for (int i = 0; i < facenames.size(); i++) {
			unsigned char *data = stbi_load((path  + "//" + facenames[i]).c_str(), &width, &height, &nrChannels, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
				);
				stbi_image_free(data);
			}
			else
			{
				std::cout << "RENDERER_3D::CUBE_MAP::LOADERROR::" << facenames[i] << std::endl;
				stbi_image_free(data);
			}

		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		_skyboxtexture = tid;
	}


	//Draw logic

	/*
	int loc = 0;


		vertics.reserve(_meshes_static_data.size());
		std::vector < Vertex3D> v;
		std::vector<unsigned int> in;

		for (int i = 0; i < _meshes_static_data.size(); i++) {
			v = _meshes_static_data[i].vertices;
			vertics.insert(vertics.end(), v.begin(), v.end());
			in = _meshes_static_data[i].indices;
			locs.push_back(loc);
			indics.insert(indics.end(), in.begin(), in.end());
			loc += in.size();
		}
		static_vertics = vertics;
		static_indics = indics;
		static_locs = locs;
		static_last_loc = loc;

	*/


	void Renderer_3D::redraw(OBJ* m, int id) {
        _OBJDATA _data = _database[id];

        int startval = _locs[id].first;
        int endval = _locs[id].second;

        const std::vector<Mesh>* meshes = m->getMeshes();

		int num = 0;

		std::vector<Vertex3D> vt;
        std::vector<unsigned int> in;
		for (int i = 0; i < _data.mesh_size; i++) {
			const Mesh* m = &(*meshes)[i];
			vt = m->vertices;
			std::vector<Vertex3D>::iterator nth = _vertics.begin() + _data.index_loc_i;
			_vertics.insert(nth, vt.begin(), vt.end());
			in = m->indices;
            std::vector<unsigned int>::iterator inth = _indics.begin() + _data.index_loc_i;
			_indics.insert(inth, in.begin(), in.end());
		}

        glm::vec3 pos = m->getPos();
        glm::mat4 model = glm::mat4(1);
        model = glm::translate(model, pos);
        model = glm::scale(model, m->getscale());
        _mesh_models[_data.index_i]= model;

	}

	int Renderer_3D::addtodraw(OBJ* m) {


        ///insert render_data
        const std::vector<Mesh>* meshes = m->getMeshes();

        //insert meshes
        _meshes.insert(_meshes.end(), meshes->begin(), meshes->end());

        //insert model
        glm::vec3 pos = m->getPos();
        glm::mat4 model = glm::mat4(1);
        model = glm::translate(model, pos);
        model = glm::scale(model, m->getscale());
        _mesh_models.push_back(model);

        //loc logic
        int v_start, v_const_start;
		if(_locs.empty()){
            v_start = 0;
		}else{
            v_start = _locs.back().second;
		}
        v_const_start = v_start;
        int loc_i = _locs.size();

        std::vector<Vertex3D> vt;
        std::vector<unsigned int> in;
        for(int i = 0; i < meshes->size(); i++){
            const Mesh* mesh = &(*meshes)[i];



            vt = mesh->vertices;
            in = mesh->indices;

            //insert vertics for each mesh
            _vertics.insert(_vertics.end(), vt.begin(), vt.end());
            //insert indics for each mesh
            _indics.insert(_indics.end(), in.begin(), in.end());

            //insert locs for each mesh
            int indicsize = in.size();
            _locs.push_back(std::make_pair(v_start,v_start + indicsize));
            v_start = v_start + indicsize;
        }


        ///insert obj data
        _OBJDATA new_object;
        //insert mesh_size
        new_object.mesh_size = meshes->size();
        //insert index_i
        new_object.index_i = _database.size();
        new_object.index_loc_i = loc_i;
        new_object.index_start_loc = v_const_start;
        new_object.index_end_loc = v_start;
        _database.push_back(new_object);

		return new_object.index_i;
	}



	int Renderer_3D::addtodrawMeshInstanced(const Mesh_I& mesh, const std::vector<Instance_Offset_Data>& posoffsets) {

		_meshes_instanced_data.push_back(mesh);
		_meshes_instanced_offsetpos.push_back(posoffsets);


		return -1;
	}

    void Renderer_3D::redrawMeshInstanced(int id,const Mesh_I& mesh, const std::vector<Instance_Offset_Data>& posoffsets){

        _meshes_instanced_data.push_back(mesh);
        _meshes_instanced_offsetpos.push_back(posoffsets);
        return;
    }



	void Renderer_3D::redrawCollisionBox(int id, Collision_Object* sp) {
		glm::vec3 addpos = sp->getExtraPos();
		std::vector<Collision_Sphere>* spheres = sp->getSpheres();
		for (int i = 0; i < spheres->size(); i++) {
			if ((*spheres)[i].renderMesh.vertices.empty()) {
				(*spheres)[i].renderMesh = getSphereMesh((*spheres)[i].getRadius(), 16, 16);
			}

			Mesh m = (*spheres)[i].renderMesh;
			_mesh_strip.push_back(m);
			glm::vec3 pos = (*spheres)[i].getPos();
			_mesh_strip_poss.push_back(pos+ addpos);
		}

		//lets draw our box
		std::vector<Collision_Box>* boxes = sp->getBoxes();

		for (int i = 0; i < boxes->size(); i++) {
			//lets check if mesh exist and if its not create one for it
			if ((*boxes)[i].renderMesh.vertices.empty()) {
				(*boxes)[i].renderMesh = getBoxMesh((*boxes)[i].getVerticies());
			}
			Mesh m = (*boxes)[i].renderMesh;
			_mesh_strip.push_back(m);
			glm::vec3 pos = (*boxes)[i].getPos();
			_mesh_strip_poss.push_back(pos + addpos);
		}
	}

	int Renderer_3D::addtodrawCollisionBox(Collision_Object* sp) {
		glm::vec3 addpos = sp->getExtraPos();
		std::vector<Collision_Sphere>* spheres = sp->getSpheres();
		for (int i = 0; i < spheres->size(); i++) {
			if ((*spheres)[i].renderMesh.vertices.empty()) {
				(*spheres)[i].renderMesh = getSphereMesh((*spheres)[i].getRadius(), 16, 16);
			}

			Mesh m = (*spheres)[i].renderMesh;
			_mesh_strip.push_back(m);
			glm::vec3 pos = (*spheres)[i].getPos();
			_mesh_strip_poss.push_back(pos + addpos);
		}

		//lets draw our box
		std::vector<Collision_Box>* boxes = sp->getBoxes();

		for (int i = 0; i < boxes->size(); i++) {
			//lets check if mesh exist and if its not create one for it
			if ((*boxes)[i].renderMesh.vertices.empty()) {
				(*boxes)[i].renderMesh = getBoxMesh((*boxes)[i].getVerticies());
			}
			Mesh m = (*boxes)[i].renderMesh;
			_mesh_strip.push_back(m);
			glm::vec3 pos = (*boxes)[i].getPos();
			_mesh_strip_poss.push_back(pos + addpos);
		}

		return -1;
	}



	//render logic
	void Renderer_3D::render() {
		glDisable(GL_BLEND);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//1 GEOMETRY PASS
		_g_glsl.use();
		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderWithGLSL(_g_glsl);

		if (!_mesh_strip.empty()) {
			renderStrips(_g_glsl);
		}

		_g_glsl.unuse();
		_g_glsl_instanced.use();
		renderWithGLSLinstanced(_g_glsl_instanced);
        renderWithGLSL(_g_glsl);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		_g_glsl_instanced.unuse();


	//	lightpositions.back() = _cam->getPos();
		//2 LIGHTING PASS

		_shading_glsl.use();
		_shading_glsl.setInt("gPosition", 0);
		_shading_glsl.setInt("gNormal", 1);
		_shading_glsl.setInt("gAlbedoSpec", 2);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gPosition);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gNormal);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);

		//Send lightpositions to fragment shader
		for (int i = 0; i < lightpositions.size(); i++) {
			_shading_glsl.setVec3("lights[" + std::to_string(i) + "].Position", lightpositions[i]);
			_shading_glsl.setVec3("lights[" + std::to_string(i) + "].Color", lightcolors[i]);
			const float constant = 1.0f;
		//	const float linear = 0.7f;
		//	const float quadratic = 1.8f;
			const float linear = 0.02f;
			const float quadratic = 0.01f;
			_shading_glsl.setFloat("lights[" + std::to_string(i) + "].Linear", linear);
			_shading_glsl.setFloat("lights[" + std::to_string(i) + "].Quadratic", quadratic);

			float lightMax = std::fmaxf(std::fmaxf(lightcolors[i].r, lightcolors[i].g), lightcolors[i].b);
			float radius =
				(-linear + sqrtf(linear * linear - 4 * quadratic * (constant - (256.0f / 5.0f) * lightMax)))
				/ (2 * quadratic);
			_shading_glsl.setFloat("lights[" + std::to_string(i) + "].Radius", radius);
		}
		_shading_glsl.setVec3("viewPos", _cam->getPos());
		//render a screenwide quad to calculate shading for each pixel
		renderQuad();
		_shading_glsl.unuse();

		//2.5
		//copy content of geometry's depth buffer to default frame buffer's depth buffer
		glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, _width, _height, 0, 0, _width, _height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		glDepthFunc(GL_LEQUAL);
		//2.7 lets render skybox
		_skybox_glsl.use();
		glBindVertexArray(cubeVAO);
		glActiveTexture(GL_TEXTURE0);
		glUniformMatrix4fv(_skybox_glsl.getUniformLocation("projection"), 1, GL_FALSE, &(_cam->getProjection()[0][0]));
		glm::mat4 v = glm::mat4(glm::mat3(_cam->getView()));
		glUniformMatrix4fv(_skybox_glsl.getUniformLocation("view"), 1, GL_FALSE, &(v[0][0]));
		_skybox_glsl.setInt("skybox", 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP,_skyboxtexture);
		renderCube();
		_skybox_glsl.unuse();
		glDepthFunc(GL_LESS);



		//3 Render LightBoxes
		_lightbox_glsl.use();
		glUniformMatrix4fv(_lightbox_glsl.getUniformLocation("projection"), 1, GL_FALSE, &(_cam->getProjection()[0][0]));
		glUniformMatrix4fv(_lightbox_glsl.getUniformLocation("view"), 1, GL_FALSE, &(_cam->getView()[0][0]));

		for (int i = 0; i < lightpositions.size(); i++) {
			glm::mat4 model = glm::mat4();
			model = glm::translate(model, lightpositions[i]);
			model = glm::scale(model, glm::vec3(0.3f));
			glUniformMatrix4fv(_lightbox_glsl.getUniformLocation("model"), 1, GL_FALSE, &(model[0][0]));
			_lightbox_glsl.setVec3("lightColor", lightcolors[i]);
			renderCube();
		}
		_lightbox_glsl.unuse();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void Renderer_3D::renderWithGLSL(GLSLProgram& glsl) {

		_cam->updateMatrix();

		//START
		glBindVertexArray(VAO);
		glEnable(GL_DEPTH_TEST);

		//upload VBO
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, _vertics.size() * sizeof(Vertex3D), _vertics.data(), GL_STATIC_DRAW);

		//upload EBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indics.size() * sizeof(unsigned int), _indics.data(), GL_STATIC_DRAW);

		glUniformMatrix4fv(glsl.getUniformLocation("projection"), 1, GL_FALSE, &(_cam->getProjection()[0][0]));
		glUniformMatrix4fv(glsl.getUniformLocation("view"), 1, GL_FALSE, &(_cam->getView()[0][0]));


		int indicindex = 0;
		//DRAW MESHES
		int id = 0;
		int idcounter = 0;
		for (int i = 0; i < _meshes.size(); i++) {
			id = _database[idcounter].index_start_loc;
			int tillid = _locs[idcounter].second;

			for (int c = id; c < tillid; c++) {
				render_individual(_meshes[i], _mesh_models[id], _locs[i], &glsl, indicindex);
				i++;
			}
			i--;
			idcounter++;
		}

		glBindVertexArray(0);
		glActiveTexture(GL_TEXTURE0);
	}

	void Renderer_3D::render_individual(const Mesh& mesh, const glm::mat4& model,unsigned int loc, GLSLProgram* glsl, int& indiccounter) {


		glUniformMatrix4fv(glsl->getUniformLocation("model"), 1, GL_FALSE, &(model[0][0]));

		//LOAD TEXTURES FOR THIS MESH
		int difn = 1;
		int specn = 1;

		int texturecounter = 0;
		for (int x = 0; x < mesh.textures.size(); x++) {
			glActiveTexture(GL_TEXTURE0 + texturecounter);

			std::string name = mesh.textures[x].type;
			std::string number;

			if (name == "texture_diffuse") {
				number = std::to_string(difn++);
			}
			else if (name == "texture_specular") {
				number = std::to_string(specn++);
			}
			else {
				ngiv::o("unknown texture");
			}

			glsl->setInt(name, texturecounter++);
			glBindTexture(GL_TEXTURE_2D, mesh.textures[x].id);
		}
		glDrawElementsBaseVertex(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, (void*)(loc * sizeof(unsigned int)), indiccounter);

		indiccounter += mesh.vertices.size();

	}

	void Renderer_3D::renderWithGLSLinstanced(GLSLProgram& glsl) {
		_cam->updateMatrix();


		for (int i = 0; i < _meshes_instanced_data.size(); i++) {

			Mesh_I* m = &_meshes_instanced_data[i];

			int size = _meshes_instanced_offsetpos[i].size();

			std::vector<Vertex3D_Instanced> vertics = _meshes_instanced_data[i].vertices;
			std::vector<unsigned int> indics = _meshes_instanced_data[i].indices;


			glBindVertexArray(VAO);
			glEnable(GL_DEPTH_TEST);

			//upload VBO
			glBindBuffer(GL_ARRAY_BUFFER, VBO_INS_D);




			//create the buffer;
			std::vector<Instance_Offset_Data>* data = &_meshes_instanced_offsetpos[i];



			glBufferData(GL_ARRAY_BUFFER, vertics.size() * sizeof(Vertex3D_Instanced), vertics.data(), GL_DYNAMIC_DRAW);


			glBindBuffer(GL_ARRAY_BUFFER, VBO_INS_I);

			glBufferData(GL_ARRAY_BUFFER, data->size() * sizeof(Instance_Offset_Data), data->data(), GL_DYNAMIC_DRAW);





			//upload EBO
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indics.size() * sizeof(unsigned int), indics.data(), GL_DYNAMIC_DRAW);

			glUniformMatrix4fv(glsl.getUniformLocation("projection"), 1, GL_FALSE, &(_cam->getProjection()[0][0]));
			glUniformMatrix4fv(glsl.getUniformLocation("view"), 1, GL_FALSE, &(_cam->getView()[0][0]));



			//prepare data


			int difn = 1;
			int specn = 1;
			int texturecounter = 0;
			for (int x = 0; x < m->textures.size(); x++) {
				glActiveTexture(GL_TEXTURE0 + texturecounter);

				std::string name = m->textures[x].type;
				std::string number;

				if (name == "texture_diffuse") {
					number = std::to_string(difn++);
				}
				else if (name == "texture_specular") {
					number = std::to_string(specn++);
				}
				else {
					ngiv::o("unknown texture");
				}

				glsl.setInt(name, texturecounter++);
				glBindTexture(GL_TEXTURE_2D, m->textures[x].id);
			}



			glm::mat4 model = glm::mat4();
			glUniformMatrix4fv(glsl.getUniformLocation("model"), 1, GL_FALSE, &(model[0][0]));
		 	glDrawElementsInstanced(GL_TRIANGLES, indics.size(), GL_UNSIGNED_INT, NULL, size);


		}


        for (int i = 0; i < _dynamic_meshes_instanced_data.size(); i++) {

			Mesh_I* m = &_dynamic_meshes_instanced_data[i];

			int size = _dynamic_meshes_instanced_offsetpos[i].size();

			std::vector<Vertex3D_Instanced> vertics = _dynamic_meshes_instanced_data[i].vertices;
			std::vector<unsigned int> indics = _dynamic_meshes_instanced_data[i].indices;


			glBindVertexArray(VAO);
			glEnable(GL_DEPTH_TEST);

			//upload VBO
			glBindBuffer(GL_ARRAY_BUFFER, VBO_INS_D);




			//create the buffer;
			std::vector<Instance_Offset_Data>* data = &_dynamic_meshes_instanced_offsetpos[i];



			glBufferData(GL_ARRAY_BUFFER, vertics.size() * sizeof(Vertex3D_Instanced), vertics.data(), GL_DYNAMIC_DRAW);


			glBindBuffer(GL_ARRAY_BUFFER, VBO_INS_I);

			glBufferData(GL_ARRAY_BUFFER, data->size() * sizeof(Instance_Offset_Data), data->data(), GL_DYNAMIC_DRAW);





			//upload EBO
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indics.size() * sizeof(unsigned int), indics.data(), GL_DYNAMIC_DRAW);

			glUniformMatrix4fv(glsl.getUniformLocation("projection"), 1, GL_FALSE, &(_cam->getProjection()[0][0]));
			glUniformMatrix4fv(glsl.getUniformLocation("view"), 1, GL_FALSE, &(_cam->getView()[0][0]));



			//prepare data


			int difn = 1;
			int specn = 1;
			int texturecounter = 0;
			for (int x = 0; x < m->textures.size(); x++) {
				glActiveTexture(GL_TEXTURE0 + texturecounter);

				std::string name = m->textures[x].type;
				std::string number;

				if (name == "texture_diffuse") {
					number = std::to_string(difn++);
				}
				else if (name == "texture_specular") {
					number = std::to_string(specn++);
				}
				else {
					ngiv::o("unknown texture");
				}

				glsl.setInt(name, texturecounter++);
				glBindTexture(GL_TEXTURE_2D, m->textures[x].id);
			}



			glm::mat4 model = glm::mat4();
			glUniformMatrix4fv(glsl.getUniformLocation("model"), 1, GL_FALSE, &(model[0][0]));
		 	glDrawElementsInstanced(GL_TRIANGLES, indics.size(), GL_UNSIGNED_INT, NULL, size);


		}


		glBindVertexArray(0);
		glActiveTexture(GL_TEXTURE0);
	}




	void Renderer_3D::renderStrips(GLSLProgram& glsl) {
		//VERTEX
		std::vector<Vertex3D> vertics;
		std::vector<Vertex3D> vt;

		for (int i = 0; i < _mesh_strip.size(); i++) {
			Mesh* m = &_mesh_strip[i];
			vt = m->vertices;
			vertics.insert(vertics.end(), vt.begin(), vt.end());
		}
		//START
		glBindVertexArray(VAO);

		//upload VBO
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertics.size() * sizeof(Vertex3D), vertics.data(), GL_DYNAMIC_DRAW);

		glUniformMatrix4fv(glsl.getUniformLocation("projection"), 1, GL_FALSE, &(_cam->getProjection()[0][0]));
		glUniformMatrix4fv(glsl.getUniformLocation("view"), 1, GL_FALSE, &(_cam->getView()[0][0]));

		glLineWidth(8);

		unsigned int c = 0;
		//DRAW MESHES
		for (int i = 0; i < _mesh_strip.size(); i++) {
			Mesh* mesh = &_mesh_strip[i];

			glm::mat4 model = glm::mat4(1.0f);
			glm::vec3 p = _mesh_strip_poss[i];
			model = glm::translate(model, p);
			glUniformMatrix4fv(glsl.getUniformLocation("model"), 1, GL_FALSE, &(model[0][0]));
			//LOAD TEXTURES FOR THIS MESH

			glDrawArrays(GL_LINE_STRIP, c,mesh->vertices.size());
			c += mesh->vertices.size();
		}

		vertics.clear();
		_mesh_strip.clear();
		_mesh_strip_poss.clear();

		glBindVertexArray(0);

		glActiveTexture(GL_TEXTURE0);
	}

	void Renderer_3D::dispose() {

		if (initialized) {
			initialized = false;
			glDeleteVertexArrays(1, &VAO);
			glDeleteBuffers(1, &VBO);
			glDeleteVertexArrays(1, &quadVAO);
			glDeleteBuffers(1, &quadVBO);
			glDeleteVertexArrays(1, &cubeVAO);
			glDeleteBuffers(1, &cubeVBO);

			VAO = 0;
			quadVAO = 0;
			cubeVAO = 0;

			_g_glsl.dispose();
			_shading_glsl.dispose();
			_lightbox_glsl.dispose();
		}



	}

}
