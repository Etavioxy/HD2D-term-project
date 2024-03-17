#pragma once

#include "TriMesh.h"
#include "Camera.h"
#include "MeshPainter.h"
#include "animation.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <float.h>

class MeshAndObject {
public:
	TriMesh* mesh;
	openGLObject* obj;
	void init(const std::string& str, std::string& vshader, std::string& fshader, glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale);
	void draw(int type, Light* light, Camera* camera, glm::mat4 lightSpaceMatrix, bool mirror);
	void update_light_model(int Mode);
	void clear();
};

struct MatchAndFire: public MeshAndObject {
	std::vector<TriMesh*> mesh;
	std::vector<openGLObject*> obj;
	enum {
		Match,
		Fire,
		Smoke
	};
	
	int fireupcnt = 0;
	bool fireorsmoke = 0;	
	void init();
	void fireup();
	void draw(int type, Light* light, Camera* camera, glm::mat4 lightSpaceMatrix, bool mirror);

};

class Man {
public:
	const double gravity = 0.02;
	const double movespeed = 0.075;
	const double jumpspeed = 0.2;

	const float HEADX = 0.4;
	const float HEADY = 0.4;
	const float HEADZ = 0.4;
	const float BODYX = 0.4;
	const float BODYY = 0.2;
	const float BODYZ = 0.6;
	const float HANDX = 0.2;
	const float HANDY = 0.2;
	const float HANDZ = 0.6;
	const float LEGX = 0.2;
	const float LEGY = 0.2;
	const float LEGZ = 0.6;
	enum {
		Body,			// 躯干
		Head,			// 头部
		LeftHand,
		RightHand,
		LeftLeg,
		RightLeg,
		HeadShake     //头的左右偏移
	};
	// 关节角大小
	GLfloat theta[10] = {
		0.0,    // Torso
		0.0,    // Head
		0.0,    // LeftHand,
		0.0,    // RightHand,
		0.0,    // LeftLeg,
		0.0,	//RightLeg
		0.0		//HeadShake
	};

	glm::vec2 move[7] = {
		glm::vec2(0,0),
		glm::vec2(0,0),
		glm::vec2(0,0),
		glm::vec2(0,0),
		glm::vec2(0,0),
		glm::vec2(0,0),
		glm::vec2(0,0)
	};

	float faceto;
	double height;
	//MatchAndFire match;

	int throw_cnt=0;

	std::vector<TriMesh*> mesh;
	std::vector<openGLObject*> obj;

	bool walking = 0;
	int walkcnt = 0;
	bool walkdic[4] = { 0 };//对应前后左右四个状态;
	int iswalking = 0;
	double upspeed = 0;
	const int wavetime=40;

	void init();

	void wavehand();
	void walk(Camera *camera);
	bool checkwalk(double x, double y, glm::vec2 pos, bool flag);
	void setwalk();
	void jump(Camera *camera);
	void throw_act();

	inline bool isKeyboard(int key) {
		if (key == GLFW_KEY_A || key == GLFW_KEY_D || key == GLFW_KEY_W || key == GLFW_KEY_S || key == GLFW_KEY_SPACE)
			return true;
		return false;
	}
	void keyboard(int key, int action, int mode, GLfloat thetaOfBody);
	void draw(int type, Light* light, Camera* camera, glm::mat4 lightSpaceMatrix, bool mirror);
	void update_light_model(int Mode);
	void clear();
};

struct GrassLand {
	TriMesh* mesh[30];
	openGLObject* obj[30];
	void init(std::string& vshader, std::string& fshader, MeshPainter* painter);
	void update_light_model(int Mode);
	void draw(int type, Light* light, Camera* camera, glm::mat4 lightSpaceMatrix, bool mirror);
	void clear();
};

struct Goat {
	TriMesh* mesh;
	openGLObject* obj;
	int id;
	void init(std::string vshader, std::string fshader, MeshPainter* painter, AnimationStateManager& animation_manager);
	void update_light_model(int Mode);
	void draw(int type, Light* light, Camera* camera, glm::mat4 lightSpaceMatrix, bool mirror);
	void clear();
	std::vector<float> calculate_p(int id);
};

class BackGround {
public:
	enum {
		Top,
		Front,
		Back,
		Left,
		Right,
		Bottom
	};
	std::vector<TriMesh*> mesh;
	std::vector<openGLObject*> obj;
	MeshAndObject table;
	MeshAndObject cottage;
	GrassLand grass_land;
	Goat goat[3];
    AnimationStateManager goat_animation_manager[3];
	void update_light_model(int Mode);
	void init(std::string vshader, std::string fshader, MeshPainter* painter);
	void draw_bg(int type, Light* light, Camera* camera, glm::mat4 lightSpaceMatrix, bool mirror);
	void clear();
};

struct Mirror: public MeshAndObject{
	glm::vec4 norm = glm::vec4(0.0, -1.0, 0.0, 0.0);
	glm::mat4 PVMatrix = glm::mat4(1.0);
	void init();
	void update_light_model(int Mode);
	void update_texture_and_PVMatrix(unsigned int texture, Camera* camera);
	void draw(int type, Light* light, Camera* camera, glm::mat4 lightSpaceMatrix, bool mirror);
};

struct MirrorBack: public MeshAndObject {
	glm::vec4 norm = glm::vec4(0.0, -1.0, 0.0, 0.0);
	void init();
	void update_light_model(int Mode);
	void draw(int type, Light* light, Camera* camera, glm::mat4 lightSpaceMatrix, bool mirror);
};
