#include "Renderer_3D.h"
#include "Misc.h"
#include <functional>
#include <SDL\SDL_keycode.h>
#include "IOManager.h"
#include "TextureLoader.h"
#include "stb_image.h"

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


				float xdegree = 2 * xpercent * M_PI;
				float ydegree = 2 * ypercent * M_PI;

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
	
	Renderer_3D::Renderer_3D()
	{
	}
	Renderer_3D::~Renderer_3D()
	{
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
		_shading_glsl.compileShaders("Shaders//defaultShading.vs", "Shaders//defaultShading.fs");

		_shading_glsl.addAttribute("aPos");
		_shading_glsl.addAttribute("aTexCoords");
		_shading_glsl.linkShaders();


		//Create g_glsl
		_g_glsl.compileShaders("Shaders//Gshader.vs", "Shaders//Gshader.fs");
		_g_glsl.addAttribute("aPos");
		_g_glsl.addAttribute("aNormal");
		_g_glsl.addAttribute("aTexCoords");
		_g_glsl.linkShaders();


		//create lightbox_glsl
		_lightbox_glsl.compileShaders("Shaders//DefaultLightBox.vs", "Shaders//DefaultLightBox.fs");
		_lightbox_glsl.addAttribute("aPos");
		_lightbox_glsl.linkShaders();

		_skybox_glsl.compileShaders("Shaders//defaultSkybox.vs", "Shaders//defaultSkybox.fs");
		_skybox_glsl.addAttribute("aPos");
		_skybox_glsl.linkShaders();

	//	_basicglsl = new ngiv::GLSLProgram();
	//	_basicglsl->compileShaders("Shaders//basic3D.vs", "Shaders//basic3D.fs");
	//	_basicglsl->addAttribute("aPos");
	//	_basicglsl->addAttribute("aNormal");
	//	_basicglsl->addAttribute("aTexCoords");
	//	_basicglsl->linkShaders();
		
		//create VAO
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		for (int i = 0; i < 1; i++) {
			lightpositions.push_back(glm::vec3(2, 6, 1));
			lightcolors.push_back(glm::vec3(1, 1, 1));
		}
				
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


		//get default depth size
		GLint defaultdepth;
		glGetIntegerv(GL_DEPTH_BITS, &defaultdepth);

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

		path = "Skybox\\" + path;
		int width, height, nrChannels;	
		unsigned int tid;
		glGenTextures(1, &tid);
		glBindTexture(GL_TEXTURE_CUBE_MAP, tid);

		for (int i = 0; i < facenames.size(); i++) {
			unsigned char *data = stbi_load((path  + "\\" + facenames[i]).c_str(), &width, &height, &nrChannels, 0);
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


	void Renderer_3D::drawMultipleMesh(const std::vector<std::vector<Mesh>>& meshes,const std::vector<std::vector<glm::vec3>>& poss,glm::vec3 scale) {	
				for (int i = 0; i < meshes.size(); i++) {
			for (int j = 0; j < meshes[i].size(); j++) {
				_meshes.push_back(meshes[i][j]);
				_meshes_poss.push_back(poss[i][j]);
				_meshes_scale.push_back(scale);
			}
		}
	}

	void Renderer_3D::draw(OBJ* m) {
		std::vector<Mesh>* meshes = m->getMeshes();
		glm::vec3 pos = m->getPos();
		for (int x = 0; x < meshes->size(); x++) {
			_meshes.push_back((*meshes)[x]);
			_meshes_poss.push_back(pos);
			_meshes_scale.push_back(m->getscale());
		}

	}

	void Renderer_3D::drawCollisionBox(Collision_Object* sp) {
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
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		_g_glsl.unuse();


	//	lightpositions.back() = _cam->getPos();
		//2 LIGHTING PASS

		glDisable(GL_DEPTH);
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
				(-linear + std::sqrtf(linear * linear - 4 * quadratic * (constant - (256.0 / 5.0) * lightMax)))
				/ (2 * quadratic);
			_shading_glsl.setFloat("lights[" + std::to_string(i) + "].Radius", radius);
		}
		_shading_glsl.setVec3("viewPos", _cam->getPos());
		//render a screenwide quad to calculate shading for each pixel
		renderQuad();
		_shading_glsl.unuse();
		glEnable(GL_DEPTH);


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
		//VERTEX
		std::vector<Vertex3D> vertics;
		std::vector<Vertex3D> vt;

		//INDICIES
		std::vector<unsigned int> indics;
		std::vector<unsigned int> in;

		int num = 0;
		std::vector<unsigned int> locs;
		for (int i = 0; i < _meshes.size(); i++) {
			Mesh* m = &_meshes[i];
			vt = m->vertices;
			vertics.insert(vertics.end(), vt.begin(), vt.end());

			in = m->indices;
			locs.push_back(num);
			indics.insert(indics.end(), in.begin(), in.end());
			num += in.size();
		}
			
		
		//START
		glBindVertexArray(VAO);
		glEnable(GL_DEPTH_TEST);

		//upload VBO
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertics.size() * sizeof(Vertex3D), vertics.data(), GL_DYNAMIC_DRAW);

		//upload EBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indics.size() * sizeof(unsigned int), indics.data(), GL_DYNAMIC_DRAW);
		
		glUniformMatrix4fv(glsl.getUniformLocation("projection"), 1, GL_FALSE, &(_cam->getProjection()[0][0]));
		glUniformMatrix4fv(glsl.getUniformLocation("view"), 1, GL_FALSE, &(_cam->getView()[0][0]));
		
		int indicindex = 0;
		int textcounter = 0;

		//DRAW MESHES
		glActiveTexture(GL_TEXTURE0);
		for (int i = 0; i < _meshes.size(); i++) {
			Mesh* mesh = &_meshes[i];

			glm::mat4 model = glm::mat4(1.0f);

			glm::vec3 p = _meshes_poss[i];
			model = glm::translate(model, p);
			model = glm::scale(model, _meshes_scale[i]);

			glUniformMatrix4fv(glsl.getUniformLocation("model"), 1, GL_FALSE, &(model[0][0]));

			//LOAD TEXTURES FOR THIS MESH
			int difn = 1;
			int specn = 1;

			for (int x = 0; x < mesh->textures.size(); x++) {
				glActiveTexture(GL_TEXTURE0 + textcounter);

				std::string name = mesh->textures[x].type;
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

				glsl.setInt(name, textcounter++);
				glBindTexture(GL_TEXTURE_2D, mesh->textures[x].id);
			}
			glDrawElementsBaseVertex(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, (void*)(locs[i] * sizeof(unsigned int)), indicindex);

			textcounter = 0;
			indicindex += mesh->vertices.size();
		}

		vertics.clear();
		indics.clear();

		_meshes.clear();
		_meshes_poss.clear();

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