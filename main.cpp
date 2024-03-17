#include "Angel.h"
#include "TriMesh.h"
#include "Camera.h"
#include "MeshPainter.h"
#include "Body.h"
#include "animation.h"
#include <shader.h>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
int WIDTH = 900;
int HEIGHT = 900;
const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
const unsigned long long delay = 25;
unsigned int depthMapFBO;
unsigned int depthMap;
unsigned int mirrorFBO;
unsigned int mirrorTexture;
unsigned int mirrordepthMapFBO;
unsigned int mirrordepthMap;

int mainWindow;
bool is_click = false;
double NowTime,NextTime;

#pragma execution_character_set("utf-8")

glm::mat4 lightSpaceMatrix;
Simpleobj debugobj;
Simpleobj dilation, depth_of_field;

Light* uplight = new Light();
Light* light ;
Light* matchlight = new Light();


Camera* camera = new Camera();
Camera* mirror_camera = new Camera();
MeshPainter* painter = new MeshPainter();


std::vector<TriMesh*> meshList;

glm::mat4 matchmodelMatrix;

class MatrixStack {
	int		_index;
	int		_size;
	glm::mat4* _matrices;

public:
	MatrixStack(int numMatrices = 100) :_index(0), _size(numMatrices)
	{
		_matrices = new glm::mat4[numMatrices];
	}

	~MatrixStack()
	{
		delete[]_matrices;
	}

	void push(const glm::mat4& m) {
		assert(_index + 1 < _size);
		_matrices[_index++] = m;
	}

	glm::mat4& pop() {
		assert(_index - 1 >= 0);
		_index--;
		return _matrices[_index];
	}
};
TriMesh* plane[10];

Man man;

BackGround bg;

Mirror mirror;
MirrorBack mirror_back;

inline bool checkwalk(double x, double y, glm::vec3 pos, bool flag)
{
	if ((man.move->x + x - pos.x) * (man.move->x + x - pos.x) + (man.move->y + y - pos.y) * (man.move->y + y - pos.y) < 0.5 && flag == false)
		return false;
	if (man.move->x+x <= -5.5 || man.move->x+x >= 5.5 || man.move->y+y <= -5.5 || man.move->y+y >= 5.5)
		return false;
	return true;
}

void createTextureFramebuffer() {

	glGenFramebuffers(1, &mirrordepthMapFBO);
	// create depth texture
	glGenTextures(1, &mirrordepthMap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindTexture(GL_TEXTURE_2D, mirrordepthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, mirrordepthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mirrordepthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glGenFramebuffers(1, &mirrorFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, mirrorFBO);
    // Éú³ÉÑÕÉ«»º³åÇøµÄÎÆÀí¶ÔÏó²¢°ó¶¨µ½framebufferÉÏ
    glGenTextures(1, &mirrorTexture);
    glBindTexture(GL_TEXTURE_2D, mirrorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mirrorTexture, 0);
    
    // ÏÂÃæÕâ¶Î´úÂë²»Ê¹ÓÃÎÆÀí×÷ÎªÉî¶È»º³åÇø¡£
    GLuint depthBufferID;
    glGenRenderbuffers(1, &depthBufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBufferID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, SHADOW_WIDTH, SHADOW_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferID);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        // framebufferÉú³ÉÊ§°Ü
		printf("error");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void initScreenQuad(unsigned int& VAO, 
                    unsigned int& VBO,
                    unsigned int& framebuffer,
                    unsigned int& texture_colorbuffer,
                    unsigned int& texture_positionbuffer,
                    unsigned int& framebuffer2,
                    unsigned int& dilationbuffer,
					int buf_width, int buf_height) 
{
    float quad_vertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
    // screen quad VAO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), &quad_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// create a color attachment texture
	glGenTextures(1, &texture_colorbuffer);
	glBindTexture(GL_TEXTURE_2D, texture_colorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, buf_width, buf_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_colorbuffer, 0);

	glGenTextures(1, &texture_positionbuffer);
	glBindTexture(GL_TEXTURE_2D, texture_positionbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, buf_width, buf_height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, texture_positionbuffer, 0);
	
	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, drawBuffers);

    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, buf_width, buf_height); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        fprintf(stderr, "Error::FRAMEBUFFER::Framebuffer is not complete!\n");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glGenFramebuffers(1, &framebuffer2);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer2);

	// create a color attachment texture
	glGenTextures(1, &dilationbuffer);
	glBindTexture(GL_TEXTURE_2D, dilationbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, buf_width, buf_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, dilationbuffer, 0);
}

void init()
{
	std::string str, vshader, fshader;
	// ¶ÁÈ¡×ÅÉ«Æ÷²¢Ê¹ÓÃ
	vshader = "shaders/shadow_mapping_depth_vshader.glsl";
	fshader = "shaders/shadow_mapping_depth_fshader.glsl";
	MeshPainter::simpleobj.program = InitShader(vshader.c_str(), fshader.c_str());
	vshader = "shaders/vshader.glsl";
	fshader = "shaders/fshader.glsl";
	// °ó¶¨openGL¶ÔÏó£¬²¢´«µÝ¶¥µãÊôÐÔµÄÊý¾Ý
	MeshPainter::obj.program = InitShader(vshader.c_str(), fshader.c_str());

	// ÉèÖÃ¹âÔ´Î»ÖÃÒÔ¼°²ÎÊý
	//¶¥²¿¹âÔ´
	uplight->setTranslation(glm::vec3(7.5,7.5, 10.0));
	uplight->setAmbient(glm::vec4(1.0, 1.0, 1.0, 1.0)); // »·¾³¹â
	uplight->setDiffuse(glm::vec4(1.0, 1.0, 1.0, 1.0)); // Âþ·´Éä
	uplight->setSpecular(glm::vec4(1.0, 1.0, 1.0, 1.0)); // ¾µÃæ·´Éä
	uplight->setAttenuation(1.0, 0.0, 0.0);
	//»ð°Ñ¹âÔ´
	matchlight->setTranslation(glm::vec3(0.5, 1.0, 1.0));
	matchlight->setAmbient(glm::vec4(1.0, 1.0, 1.0, 1.0)); // »·¾³¹â
	matchlight->setDiffuse(glm::vec4(1.0, 1.0, 1.0, 1.0)); // Âþ·´Éä
	matchlight->setSpecular(glm::vec4(1.0, 1.0, 1.0, 1.0)); // ¾µÃæ·´Éä
	matchlight->setAttenuation(1.0,0.01,0.025);
	// matchlight->setAttenuation(1.0, 0.0, 0.0);


	man.init();
	bg.init("shaders/vshader.glsl", "shaders/fshader.glsl", painter);	
	mirror.init();
	mirror_back.init();
	light = uplight;
	man.update_light_model(Light::LIGHT);
	bg.update_light_model(Light::LIGHT);
	mirror.update_light_model(Light::LIGHT);
	mirror_back.update_light_model(Light::LIGHT);
	glClearColor(1.0, 1.0, 1.0, 0.0);
}

inline void calmodelMatrix()
{
	man.jump(camera);
	if (man.iswalking)
	{
		man.setwalk();
		man.walk(camera);
	}
	if (light == matchlight)
		matchlight->setTranslation(glm::vec3(man.move[man.Body].x - cos(glm::radians(1.0 * camera->aerfa)), man.move[man.Body].y - sin(glm::radians(1.0 * camera->aerfa)), 2.0 + man.height));
	/*	else if(camera->camera_type)
			man.theta[man.Body] = camera->aerfa - 90*/;
	man.wavehand();

	glm::mat4 modelMatrix = glm::mat4(1.0);

	MatrixStack mstack;

	modelMatrix = glm::translate(modelMatrix, glm::vec3(man.move[man.Body].x, man.move[man.Body].y, 0.5 * man.BODYZ + man.LEGZ + man.height));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(man.theta[man.Body]), glm::vec3(0.0, 0.0, 1.0));
	man.mesh[man.Body]->setChangeModelMatrix(modelMatrix);
	
	mstack.push(modelMatrix);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0, 0.0, 0.5 * man.BODYZ));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(man.theta[man.HeadShake]), glm::vec3(0.0, 0.0, 1.0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(man.theta[man.Head]), glm::vec3(1.0, 0.0, 0.0));
	man.mesh[man.Head]->setChangeModelMatrix(modelMatrix);
	modelMatrix = mstack.pop();

	mstack.push(modelMatrix);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.5 * man.BODYX - 0.5 * man.HANDX, 0.0, man.BODYZ / 3));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(man.theta[man.LeftHand]), glm::vec3(1.0, 0.0, 0.0));
	man.mesh[man.LeftHand]->setChangeModelMatrix(modelMatrix);

	mstack.push(modelMatrix);
	modelMatrix = mstack.pop();
	modelMatrix = mstack.pop();

	mstack.push(modelMatrix);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5 * man.BODYX + 0.5 * man.HANDX, 0.0, man.BODYZ / 3));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(man.theta[man.RightHand]), glm::vec3(1.0, 0.0, 0.0));
	man.mesh[man.RightHand]->setChangeModelMatrix(modelMatrix);

	mstack.push(modelMatrix);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0, 0.0, -man.BODYZ / 3 * 2));
	modelMatrix = glm::rotate(modelMatrix, 0.0f, glm::vec3(1.0, 0.0, 0.0));

    modelMatrix = mstack.pop();

	modelMatrix = mstack.pop();

	mstack.push(modelMatrix);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.5 * man.BODYX + 0.5 * man.LEGX, 0.0, -0.5 * man.BODYZ));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(man.theta[man.LeftLeg]), glm::vec3(1.0, 0.0, 0.0));
	man.mesh[man.LeftLeg]->setChangeModelMatrix(modelMatrix);
	modelMatrix = mstack.pop();

	mstack.push(modelMatrix);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5 * man.BODYX - 0.5 * man.LEGX, 0.0, -0.5 * man.BODYZ));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(man.theta[man.RightLeg]), glm::vec3(1.0, 0.0, 0.0));
	man.mesh[man.RightLeg]->setChangeModelMatrix(modelMatrix);
	modelMatrix = mstack.pop();
}

inline void DrawAllModel(int type, Camera* camera, bool mirror_flag = false) 
{
	man.draw(type, light, camera, lightSpaceMatrix, mirror_flag);
	mirror.draw(type, light, camera, lightSpaceMatrix, mirror_flag);
	bg.draw_bg(type, light, camera, lightSpaceMatrix, mirror_flag);
	if (!mirror_flag) 
		mirror_back.draw(type, light, camera, lightSpaceMatrix, mirror_flag);
}

float near_plane = 0.1f, far_plane = 21.0f;

inline void display(glm::uint framebuffer)
{
	// uplight->setTranslation(glm::vec3(delta[0], delta[1], delta[2]));
	// 启用面剔除
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	// 启用深度测试
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	
	// 渲染状态设置
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	camera->updateCamera();
	mirror_camera->mirrorTo(camera, mirror.norm);
	
	calmodelMatrix();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// 1. render depth of scene to texture (from light's perspective)
	// --------------------------------------------------------------
	glm::mat4 lightProjection, lightView;
	// mirror->setTranslation(glm::vec3(delta[0], delta[1], delta[2]));
	
	if (light == uplight)
	{
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		lightView = glm::lookAt(light->getTranslation(), glm::vec3(0.0f,0.0f, 0.0f), glm::vec3(0.0, 0.0, 1.0));
	}
	else
	{
		lightProjection = glm::perspective(179.0f,1.0f, 0.1f, light->getTranslation().z);
		lightView = glm::lookAt(light->getTranslation(), glm::vec3(light->getTranslation().x+0.0001f, light->getTranslation().y+0.0001f,0.0f), glm::vec3(0.0, 0.0, 1.0));
	}
	lightSpaceMatrix = lightProjection * lightView;
	

    glEnable(GL_CLIP_DISTANCE0);
	glBindFramebuffer(GL_FRAMEBUFFER, mirrordepthMapFBO);
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glClear(GL_DEPTH_BUFFER_BIT);
	DrawAllModel(0, mirror_camera, true);
	MeshPainter::depthMap = mirrordepthMap;

	glBindFramebuffer(GL_FRAMEBUFFER, mirrorFBO);
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DrawAllModel(1, mirror_camera, true);
    glDisable(GL_CLIP_DISTANCE0);

	mirror.update_texture_and_PVMatrix(mirrorTexture, mirror_camera);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glClear(GL_DEPTH_BUFFER_BIT);
	DrawAllModel(0, camera);
	MeshPainter::depthMap = depthMap;
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// reset viewport
	glViewport(0, 0, WIDTH, HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DrawAllModel(1, camera);
}


void printHelp()
{
	std::cout << "Keyboard Usage" << std::endl;
	std::cout <<
		"[Move]" << std::endl <<
		"W: move forword" << std::endl <<
		"S: move backword" << std::endl <<
		"A: move left" << std::endl <<
		"D: move right" << std::endl <<
		"SPACE: jump" << std::endl <<		
		std::endl <<
		"[Camera & Light]" << std::endl <<
		"T: change light mode" << std::endl <<
		"F1 put the camera behind the man" << std::endl <<
		"F2 put the camera in front of the man" << std::endl <<
		"F3 mouse only control the camera" << std::endl <<
		"mouse scroll up: make camera away" << std::endl <<
		"mouse scroll down: make camera close" << std::endl <<
		std::endl <<
		"ESC:		Exit" << std::endl;
}


void mainWindow_key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	float tmp;
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	float shininess;

///////////
	if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		auto ggg = bg.goat[0].mesh->getRotation();
		ggg.x+=10;
		bg.goat[0].mesh->setRotation(ggg);
		printf("line %d: %f %f %f\n", __LINE__, ggg.x, ggg.y, ggg.z);
	}
	if (key == GLFW_KEY_Y && action == GLFW_PRESS)
	{
		auto ggg = bg.goat[0].mesh->getRotation();
		ggg.y+=10;
		bg.goat[0].mesh->setRotation(ggg);
		printf("line %d: %f %f %f\n", __LINE__, ggg.x, ggg.y, ggg.z);
	}
	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
	{
		auto ggg = bg.goat[0].mesh->getRotation();
		ggg.z+=10;
		bg.goat[0].mesh->setRotation(ggg);
		printf("line %d: %f %f %f\n", __LINE__, ggg.x, ggg.y, ggg.z);
	}
///////////////

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	else if (key == GLFW_KEY_H && action == GLFW_PRESS)
	{
		printHelp();
	}
	else if (man.isKeyboard(key) && camera->camera_type)  
	{
		man.keyboard(key, action, mode, camera->aerfa - 90);
	}
	else if (key == GLFW_KEY_T && action == GLFW_PRESS)
	{
		if (light == uplight)
		{
			light = matchlight;
			man.update_light_model(Light::DARK);
			bg.update_light_model(Light::DARK);
			mirror.update_light_model(Light::DARK);
			mirror_back.update_light_model(Light::DARK);

		}
		else
		{
			light = uplight;
			man.update_light_model(Light::LIGHT);
			bg.update_light_model(Light::LIGHT);
			mirror.update_light_model(Light::LIGHT);
			mirror_back.update_light_model(Light::LIGHT);
		}
			
	}
	else {
		camera->keyboard(key, action, mode);
	}

}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) { }

void scroll_callback(GLFWwindow* window, double xpos, double ypos)
{
	camera->radius += ypos * 0.1;
	camera->radius = std::max(1.0f, camera->radius);
	camera->radius = std::min(10.0f, camera->radius);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	camera->mousemove(xpos, ypos);

	if (camera->camera_type == 0)
		return;
	man.theta[man.Head] = -(camera->beita -90)/2-10; 
	man.faceto = camera->aerfa - 90;
	double sum=man.theta[man.Body]+man.theta[man.HeadShake];
	double deviation = man.faceto - sum;//¼ÆËãÆ«×ªÖµ
	if (deviation > 0)
	{
		if (man.theta[man.HeadShake] + deviation > 45.0)
		{
			man.theta[man.Body] += man.theta[man.HeadShake] + deviation - 45.0;
			man.theta[man.HeadShake] = 45;
		}
		else
			man.theta[man.HeadShake] += deviation;
	}
	else if (deviation < 0)
	{
		if (man.theta[man.HeadShake] + deviation < -45.0)
		{
			man.theta[man.Body] += man.theta[man.HeadShake] + deviation + 45.0;
			man.theta[man.HeadShake] = -45;
		}
		else
			man.theta[man.HeadShake] += deviation;
	}
}

void cleanData() {

	delete camera;
	camera = NULL;

	delete uplight;
	delete matchlight;
	uplight = NULL;
	matchlight = NULL;
	light = NULL;

	man.clear();
	bg.clear();	
	mirror.clear();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
int main(int argc, char **argv)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	GLFWwindow* mainwindow = glfwCreateWindow(900, 900, "OpenGL", NULL, NULL);
	if (mainwindow == NULL)
	{
		std::cout << "Failed to create GLFW window!" << std::endl;
		glfwTerminate();
		return -1;
	}
	// bind callbacks
	glfwMakeContextCurrent(mainwindow);
	glfwSetFramebufferSizeCallback(mainwindow, framebuffer_size_callback);
	glfwSetKeyCallback(mainwindow,mainWindow_key_callback);
	glfwSetMouseButtonCallback(mainwindow, mouse_button_callback);
	glfwSetInputMode(mainwindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(mainwindow, 450, 450);
	glfwSetCursorPosCallback(mainwindow, mouse_callback);
	glfwSetScrollCallback(mainwindow, scroll_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	createTextureFramebuffer();

    unsigned int quadVAO;
    unsigned int quadVBO;
    unsigned int framebuffer;
    unsigned int texture_colorbuffer;
    unsigned int texture_positionbuffer;
    unsigned int framebuffer2;
    unsigned int dilationbuffer;
    initScreenQuad(quadVAO, quadVBO, framebuffer, texture_colorbuffer, texture_positionbuffer, framebuffer2, dilationbuffer, 900, 900); 

	init();

	std::vector<AnimationStateManager*> animation_manager_list;
	animation_manager_list.push_back(&bg.goat_animation_manager[0]);
	animation_manager_list.push_back(&bg.goat_animation_manager[1]);
	animation_manager_list.push_back(&bg.goat_animation_manager[2]);

    printHelp();

	debugobj.program= InitShader("shaders/debugv.glsl", "shaders/debugf.glsl");
	unsigned long long last_time = clock();

//...shaderProgram = depth_of_fieild.glsl frag shader
	depth_of_field.program= InitShader("shaders/screen_vshader.glsl", "shaders/depth_of_field.glsl");
	dilation.program= InitShader("shaders/screen_vshader.glsl", "shaders/dilation.glsl");

	GLuint parametersLocation = glGetUniformLocation(dilation.program, "parameters");
	GLuint FocusPointLocation = glGetUniformLocation(depth_of_field.program, "FocusPoint");
	GLuint nearFarLocation = glGetUniformLocation(depth_of_field.program, "nearFar");

	float delta_time = 0.0f;
	float last_frame = 0.0f;

	while (!glfwWindowShouldClose(mainwindow))
    {
		if (clock() - last_time < delay)
			continue;
        float currentFrame = static_cast<float>(glfwGetTime());
        delta_time = currentFrame - last_frame;
        last_frame = currentFrame;
		for(auto e:animation_manager_list) e->Update(delta_time);

        glEnable(GL_DEPTH_TEST); 
		display(framebuffer);
		
        glDisable(GL_DEPTH_TEST);
        // clear all relevant buffers
        //glClear(GL_COLOR_BUFFER_BIT);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer2);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		// 将shaderProgram绘制到屏幕
    	glUseProgram(dilation.program);
		float parameterValues[2] = {2.0f, 1.5f};
		glUniform2fv(parametersLocation, 1, parameterValues);
    	// 传递纹理数据 将生成的纹理传给shader
    	glUniform1i(glGetUniformLocation(dilation.program, "colorTexture"), 0);
    	// 绘制
        glBindVertexArray(quadVAO);
    	glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_colorbuffer);

        //glBindTexture(GL_TEXTURE_2D, texture_positionbuffer);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		// 将shaderProgram绘制到屏幕
    	glUseProgram(depth_of_field.program);
		float nearfarValues[2] = {near_plane, far_plane};
		glUniform2fv(nearFarLocation, 1, nearfarValues);
		glm::vec2 BodyPoint = man.move[man.Body];
		glm::vec3 HeadPoint = man.mesh[man.Head]->getTranslation();
		float focusPointValues[3] = {BodyPoint.x, BodyPoint.y, HeadPoint.z};
		glUniform3fv(FocusPointLocation, 1, focusPointValues);

    	glUniform1i(glGetUniformLocation(depth_of_field.program, "positionTexture"), 0);
    	glUniform1i(glGetUniformLocation(depth_of_field.program, "focusTexture"), 1);
    	glUniform1i(glGetUniformLocation(depth_of_field.program, "outOfFocusTexture"), 2);
    	// 绘制
        glBindVertexArray(quadVAO);
    	glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_positionbuffer);
    	glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture_colorbuffer);
    	glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, dilationbuffer);
        glDrawArrays(GL_TRIANGLES, 0, 6);

		last_time = clock();
        glfwSwapBuffers(mainwindow);
        glfwPollEvents();
    }
	cleanData();
    glfwTerminate();
    return 0;
}

