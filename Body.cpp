#include "Body.h"
#include "Camera.h"
#include "MeshPainter.h"
const double gravity = 0.02;
const double movespeed = 0.075;

void MeshAndObject::init(const std::string& str, std::string& vshader, std::string& fshader, glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale) {
    mesh = new TriMesh();
    obj = new openGLObject();
    mesh->setNormalize(true);
    mesh->readObj("./assets/" + str + ".obj");
    mesh->setTranslation(translation);
    mesh->setRotation(rotation);
    mesh->setScale(scale);
    MeshPainter::addMesh(*mesh, *obj, "./assets/" + str + ".png", vshader, fshader);
}
void MeshAndObject::draw(int type, Light* light, Camera* camera, glm::mat4 lightSpaceMatrix, bool mirror) {
    if (type) 
        MeshPainter::drawMesh(*mesh, *obj, light, camera, lightSpaceMatrix, mirror);
    else 
        MeshPainter::simpledrawMesh(*mesh, *obj, lightSpaceMatrix, mirror);
}
void MeshAndObject::update_light_model(int Mode) {
    if (Mode == Light::DARK) {
        mesh->setAmbient(glm::vec4(0.1, 0.1, 0.1, 1.0));
        mesh->setDiffuse(glm::vec4(0.9, 0.9, 0.9, 1.0));
    }
    else {
        mesh->setAmbient(glm::vec4(0.2, 0.2, 0.2, 1.0));
        mesh->setDiffuse(glm::vec4(0.8, 0.8, 0.8, 1.0));
    }
    mesh->setSpecular(glm::vec4(0.0, 0.0, 0.0, 1.0));
}
void MeshAndObject::clear() {
    mesh->cleanData();
    delete mesh;
    mesh = NULL;
#ifdef __APPLE__
    glDeleteVertexArraysAPPLE(1, &opengl_objects[i].vao);
#else
    glDeleteVertexArrays(1, &obj->vao);
#endif
    glDeleteBuffers(1, &obj->vbo);
    glDeleteProgram(obj->program);
    delete obj;
    obj = NULL;
}


// define Man

void Man::init() {
    mesh.resize(6);
    for (int i = 0; i < 6; i++) 
        mesh[i] = new TriMesh();
    obj.resize(6);
    for (int i = 0; i < 6; i++)
        obj[i] = new openGLObject();

    std::string vshader = "shaders/vshader.glsl";
    std::string fshader = "shaders/fshader.glsl";

    float ratio;
    std::string str;
    //身体
	mesh[Body]->setNormalize(true);
	mesh[Body]->readObj("./assets/body.obj");
    ratio = BODYX / mesh[Body]->getLenX();//设置缩放大小
    mesh[Body]->setTranslation(glm::vec3(0.0, 0.0, 0.0));
    mesh[Body]->setRotation(glm::vec3(0.0, 0.0, 0.0));
    mesh[Body]->setScale(glm::vec3(ratio, ratio, ratio));
    str = "./assets/body.png";
	MeshPainter::addMesh(*mesh[Body], *obj[Body], str, vshader, fshader);

	// 头
    
	mesh[Head]->setNormalize(true);
	mesh[Head]->readObj("./assets/head.obj");
    ratio = HEADX / mesh[Head]->getLenX();//设置缩放大小
    mesh[Head]->setTranslation(glm::vec3(0.0, 0.0,0.5*HEADZ));
    mesh[Head]->setRotation(glm::vec3(0.0, 0.0, 0.0));
    mesh[Head]->setScale(glm::vec3(ratio, ratio, ratio));
	str = "./assets/head.png";
	MeshPainter::addMesh(*mesh[Head], *obj[Head],str, vshader, fshader);

	//左手
	mesh[LeftHand]->setNormalize(true);
	mesh[LeftHand]->readObj("./assets/lefthand.obj");
    ratio = HANDX / mesh[LeftHand]->getLenX();//设置缩放大小
    mesh[LeftHand]->setTranslation(glm::vec3(0.0, 0.0, -HANDZ / 6 * 2));
    mesh[LeftHand]->setRotation(glm::vec3(0.0, 0.0, 0.0));
    mesh[LeftHand]->setScale(glm::vec3(ratio, ratio, ratio));
	str = "./assets/lefthand.png";
	MeshPainter::addMesh(*mesh[LeftHand], *obj[LeftHand], str, vshader, fshader);

	//右手
	mesh[RightHand]->setNormalize(true);
	mesh[RightHand]->readObj("./assets/righthand.obj");
    ratio = HANDX / mesh[RightHand]->getLenX();//设置缩放大小
    mesh[RightHand]->setTranslation(glm::vec3(0.0, 0.0, -HANDZ / 6 * 2));
    mesh[RightHand]->setRotation(glm::vec3(0.0, 0.0, 0.0));
    mesh[RightHand]->setScale(glm::vec3(ratio, ratio, ratio));
	str = "./assets/righthand.png";
	MeshPainter::addMesh(*mesh[RightHand], *obj[RightHand], str, vshader, fshader);

	//左脚
	mesh[LeftLeg]->setNormalize(true);
	mesh[LeftLeg]->readObj("./assets/leftleg.obj");
    ratio = HANDX / mesh[LeftLeg]->getLenX();//设置缩放大小
    mesh[LeftLeg]->setTranslation(glm::vec3(0.0, 0.0, -0.5 * LEGZ));
    mesh[LeftLeg]->setRotation(glm::vec3(0.0, 0.0, 0.0));
    mesh[LeftLeg]->setScale(glm::vec3(ratio, ratio, ratio));
	str = "./assets/leftleg.png";
	MeshPainter::addMesh(*mesh[LeftLeg], *obj[LeftLeg], str, vshader, fshader);
	
	//右脚
	mesh[RightLeg]->setNormalize(true);
	mesh[RightLeg]->readObj("./assets/rightleg.obj");
    ratio = HANDX / mesh[RightLeg]->getLenX();//设置缩放大小
    mesh[RightLeg]->setTranslation(glm::vec3(0.0,0.0,-0.5*LEGZ));
    mesh[RightLeg]->setRotation(glm::vec3(0.0, 0.0, 0.0));
    mesh[RightLeg]->setScale(glm::vec3(ratio, ratio, ratio));
	str = "./assets/rightleg.png";
	MeshPainter::addMesh(*mesh[RightLeg], *obj[RightLeg], str, vshader, fshader);

    move[Body].y += 0.5f;
}

void Man::wavehand()
{
    if (walking == 0)
    {
        theta[LeftHand] = 0;
        theta[RightHand] = 0;
        theta[LeftLeg] = 0;
        theta[RightLeg] = 0;
        return;
    }
    double wavedic = 0;
    if (walkcnt > wavetime / 4 * 3)
        wavedic = 40.0 / wavetime * 4.0;
    if (walkcnt <= wavetime / 4 * 3 && walkcnt > wavetime / 2)
        wavedic = -40.0 / wavetime * 4;
    if (walkcnt == wavetime / 2 && iswalking == 0)
    {
        walkcnt = 0;
        walking = 0;
    }
    if (walkcnt <= wavetime / 2 && walkcnt > wavetime / 4)
        wavedic = -40.0 / wavetime * 4.;
    if (walkcnt <= wavetime / 4 && walkcnt > 0)
        wavedic = 40.0 / wavetime * 4;
    theta[LeftHand] += wavedic;
    theta[RightHand] -= wavedic;
    theta[LeftLeg] -= wavedic;
    theta[RightLeg] += wavedic;
    walkcnt--;
    if (walkcnt == 0)
        walking = 0;
    return;
}

bool Man::checkwalk(double x, double y, glm::vec2 pos, bool flag)
{
	if ((move->x + x - pos.x) * (move->x + x - pos.x) + (move->y + y - pos.y) * (move->y + y - pos.y) < 0.5 && flag == false)
		return false;
	if (move->x+x <= -6.5 || move->x+x >= 6.5 || move->y+y <= -6.5 || move->y+y >= 6.5)
		return false;
    if (move->x+x <= 2.5 && move->x + x >= -2.5 && (move->y + y) * (move->y) <= 0)
        return false;
	return true;
}


void Man::walk(Camera *camera)
{
    int dic_x = walkdic[0] - walkdic[1];
    int dic_y = walkdic[2] - walkdic[3];
    double x = 0;
    double y = 0;
    if (dic_x == 1 && dic_y == 0)
    {
        x = -cos(glm::radians(1.0 * camera->aerfa)); // cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        y = -sin(glm::radians(1.0 * camera->aerfa)); // sin(glm::radians(pitch));
        theta[Body] = camera->aerfa - 90;
        theta[HeadShake] = 0.0;
    }
    else if (dic_x == -1 && dic_y == 0)
    {
        x = -cos(glm::radians(1.0 * camera->aerfa + 180)); // cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        y = -sin(glm::radians(1.0 * camera->aerfa + 180)); // sin(glm::radians(pitch));
        theta[Body] = camera->aerfa - 90;
        theta[HeadShake] = 0.0;
    }
    else if (dic_x == 0 && dic_y == 1)
    {
        x = -cos(glm::radians(1.0 * camera->aerfa + 90)); // cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        y = -sin(glm::radians(1.0 * camera->aerfa + 90)); // sin(glm::radians(pitch));
        theta[Body] = camera->aerfa - 90 - 45;
        theta[HeadShake] = 45;
    }
    else if (dic_x == 0 && dic_y == -1)
    {
        x = -cos(glm::radians(1.0 * camera->aerfa + 270)); // cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        y = -sin(glm::radians(1.0 * camera->aerfa + 270)); // sin(glm::radians(pitch));
        theta[Body] = camera->aerfa - 90 + 45;
        theta[HeadShake] = -45;
    }
    else if (dic_x == 1 && dic_y == 1)
    {
        x = -cos(glm::radians(1.0 * camera->aerfa + 45)); // cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        y = -sin(glm::radians(1.0 * camera->aerfa + 45)); // sin(glm::radians(pitch));
        theta[Body] = camera->aerfa - 90 + 45;
        theta[HeadShake] = -45;
    }
    else if (dic_x == -1 && dic_y == 1)
    {
        x = -cos(glm::radians(1.0 * camera->aerfa + 135)); // cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        y = -sin(glm::radians(1.0 * camera->aerfa + 135)); // sin(glm::radians(pitch));
        theta[Body] = camera->aerfa - 90 - 45;
        theta[HeadShake] = 45;
    }
    else if (dic_x == -1 && dic_y == -1)
    {
        x = -cos(glm::radians(1.0 * camera->aerfa + 225)); // cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        y = -sin(glm::radians(1.0 * camera->aerfa + 225));
        theta[Body] = camera->aerfa - 90 + 45;
        theta[HeadShake] = -45;
    }
    else if (dic_x == 1 && dic_y == -1)
    {
        x = -cos(glm::radians(1.0 * camera->aerfa + 315)); // cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        y = -sin(glm::radians(1.0 * camera->aerfa + 315)); // sin(glm::radians(pitch));
        theta[Body] = camera->aerfa - 90 - 45;
        theta[HeadShake] = +45;
    }
    move[Body].x += x * movespeed;
    move[Body].y += y * movespeed;
    // move the position of camera
    camera->move.x += x * movespeed;
    camera->move.y += y * movespeed;
}

void Man::setwalk()
{
    if (walking == 0)
    {
        walking = 1;
        walkcnt = wavetime;
    }
    return;
}

void Man::jump(Camera *camera)
{
    if (upspeed == 0)
        return;
    height += upspeed;
    upspeed -= gravity;
    if (height < 0)
    {
        height = 0;
        upspeed = 0;
    }
    camera->move.z = height;
    // std::cout << height << "\n";
}

void Man::keyboard(int key, int action, int mode, GLfloat thetaOfBody) {
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		walkdic[2] = 1;
		iswalking++;
	}
	else if (key == GLFW_KEY_A && action == GLFW_RELEASE)
	{
		walkdic[2] = 0;
		iswalking--;
		theta[Body] = thetaOfBody;
		theta[HeadShake] = 0;
	}
	else if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		walkdic[3] = 1;
		iswalking++;
	}
	else if (key == GLFW_KEY_D && action == GLFW_RELEASE)
	{
		walkdic[3] = 0;
		iswalking--;
		theta[Body] = thetaOfBody;
		theta[HeadShake] = 0;
	}
	else if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		walkdic[0] = 1;
		iswalking++;
	}
	else if (key == GLFW_KEY_W && action == GLFW_RELEASE)
	{
		walkdic[0] = 0;
		iswalking--;
		theta[Body] = thetaOfBody;
		theta[HeadShake] = 0;
	}
	else if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		walkdic[1] = 1;
		iswalking++;
	}
	else if (key == GLFW_KEY_S && action == GLFW_RELEASE)
	{
		walkdic[1] = 0;
		iswalking--;
		theta[Body] = thetaOfBody;
		theta[HeadShake] = 0;
	}
	else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		if(height==0)
			upspeed = jumpspeed;
	}
}

void Man::draw(int type, Light* light, Camera* camera, glm::mat4 lightSpaceMatrix, bool mirror) {
    if (type) {
        MeshPainter::drawMesh(*mesh[Body], *obj[Body], light, camera, lightSpaceMatrix, mirror);
        MeshPainter::drawMesh(*mesh[Head], *obj[Head], light, camera, lightSpaceMatrix, mirror);
        MeshPainter::drawMesh(*mesh[LeftHand], *obj[LeftHand], light, camera, lightSpaceMatrix, mirror);
        MeshPainter::drawMesh(*mesh[RightHand], *obj[RightHand], light, camera, lightSpaceMatrix, mirror);
        MeshPainter::drawMesh(*mesh[LeftLeg], *obj[LeftLeg], light, camera, lightSpaceMatrix, mirror);
        MeshPainter::drawMesh(*mesh[RightLeg], *obj[RightLeg], light, camera, lightSpaceMatrix, mirror);
    }
    else {
        MeshPainter::simpledrawMesh(*mesh[Body],*obj[Body], lightSpaceMatrix, mirror);
        MeshPainter::simpledrawMesh(*mesh[Head], *obj[Head], lightSpaceMatrix, mirror);
        MeshPainter::simpledrawMesh(*mesh[LeftHand], *obj[LeftHand], lightSpaceMatrix, mirror);
        MeshPainter::simpledrawMesh(*mesh[RightHand], *obj[RightHand], lightSpaceMatrix, mirror);
        MeshPainter::simpledrawMesh(*mesh[LeftLeg], *obj[LeftLeg], lightSpaceMatrix, mirror);
        MeshPainter::simpledrawMesh(*mesh[RightLeg], *obj[RightLeg], lightSpaceMatrix, mirror);
    }
}

void Man::update_light_model(int Mode) {
    for (int i = 0; i < 6; i++) {
        if (Mode == Light::DARK) {
            mesh[i]->setAmbient(glm::vec4(0.1, 0.1, 0.1, 1.0));
            mesh[i]->setDiffuse(glm::vec4(0.9, 0.9, 0.9, 1.0));
        }
        else {
            mesh[i]->setAmbient(glm::vec4(0.2, 0.2, 0.2, 1.0));
            mesh[i]->setDiffuse(glm::vec4(0.8, 0.8, 0.8, 1.0));
        }
        mesh[i]->setSpecular(glm::vec4(0.0, 0.0, 0.0, 1.0));
    }
}

void Man::clear() {
    for (int i = 0; i < 6; i++) {
        mesh[i]->cleanData();
        delete mesh[i];
        mesh[i] = NULL;
#ifdef __APPLE__
        glDeleteVertexArraysAPPLE(1, &opengl_objects[i].vao);
#else
        glDeleteVertexArrays(1, &obj[i]->vao);
#endif
        glDeleteBuffers(1, &obj[i]->vbo);
        glDeleteProgram(obj[i]->program);
        delete obj[i];
        obj[i] = NULL;

    }
}

void GrassLand::init(std::string& vshader, std::string& fshader, MeshPainter* painter) {
    vshader = "shaders/vshader.glsl";
    fshader = "shaders/fshader.glsl";
    for (int i = 1; i <= 5; i++) {
        for (int j = 1; j <= 5; j++) {
            int idx = 5 * (i - 1) + j - 1;
            mesh[idx] = new  TriMesh();
            obj[idx] = new openGLObject();
            mesh[idx]->generateGround(glm::vec3(1.2, 1.6, 0));
            mesh[idx]->setNormalize(false);
            //--------
            mesh[idx]->setTranslation(glm::vec3((i-3)*8.0, (j-3)*8.0, -0.01));//平面偏移，往z轴负方向稍微第一点，防止颜色重合
            mesh[idx]->setRotation(glm::vec3(0.0, 0.0, 0.0));//不旋转
            mesh[idx]->setScale(glm::vec3(2.0, 2.0, 2.0));//大小放大
            //传递平面顶点数据。
            std::string str = "./assets/neoground.png";
            MeshPainter::addMesh(*mesh[idx], *obj[idx], str, vshader, fshader); 	// 指定纹理与着色器
            // 我们创建的这个加入一个容器内，为了程序结束时将这些数据释放
        }
    }
}

void GrassLand::draw(int type, Light* light, Camera* camera, glm::mat4 lightSpaceMatrix, bool mirror) {
    for (int i = 1; i <= 5; i++) 
        for (int j = 1; j <= 5; j++) {
            int idx = (i - 1) * 5 + j - 1;
            if (type) 
                MeshPainter::drawMesh(*mesh[idx], *obj[idx], light, camera, lightSpaceMatrix, mirror);
            else
                MeshPainter::simpledrawMesh(*mesh[idx], *obj[idx], lightSpaceMatrix, mirror);
        }
}

void GrassLand::update_light_model(int Mode) {
    for (int i = 1; i <= 5; i++) {
        for (int j = 1; j <= 5; j++) {
            int idx = (i - 1) * 5 + j - 1;
            if (Mode == Light::DARK) {
                mesh[idx]->setAmbient(glm::vec4(0.1, 0.1, 0.1, 1.0));
                mesh[idx]->setDiffuse(glm::vec4(0.9, 0.9, 0.9, 1.0));
            }
            else {
                mesh[idx]->setAmbient(glm::vec4(0.2, 0.2, 0.2, 1.0));
                mesh[idx]->setDiffuse(glm::vec4(0.8, 0.8, 0.8, 1.0));
            }
            mesh[idx]->setSpecular(glm::vec4(0.0, 0.0, 0.0, 1.0));
        }
    }
}

void GrassLand::clear() {
    for (int i = 1; i <= 3; i++) 
        for (int j = 1; j <= 3; j++) {
            int idx = (i - 1) * 3 + j - 1;
            mesh[idx]->cleanData();
            delete mesh[idx];
            mesh[idx] = NULL;
#ifdef __APPLE__
            glDeleteVertexArraysAPPLE(1, &opengl_objects[i].vao);
#else
            glDeleteVertexArrays(1, &obj[idx]->vao);
#endif
            glDeleteBuffers(1, &obj[idx]->vbo);
            glDeleteProgram(obj[idx]->program);
            delete obj[idx];
            obj[idx] = NULL;
        }

}

std::vector<float> Goat::calculate_p(int id) {
	bool flag = 0;
	if( id<0 ) flag = 1, id = -id;

	int width = 208;
	int height = 152;

    int x, y, w, h;
	if( id>=4 ){
		w = 74;
		h = 42;
		x = 0 + (id-4)*w;
		y = 106;
	}else if( id==3 ){
		w = 68;
		h = 50;
		x = 0;
		y = 52;
	}else{
		w = 68;
		h = 50;
		x = 68 + (id-1)*w;
		y = 0;
	}

	if( flag==0 ) return {
    	(y / (float)height),
    	((y + h) / (float)height),
    	x / (float)width,
    	(x + w) / (float)width,
		(float)h/w
	};
	else return {
    	(y / (float)height),
    	((y + h) / (float)height),
    	(x + w) / (float)width,
    	x / (float)width,
		(float)h/w
	};
}
float scale0 = 0.45;
void Goat::init(std::string vshader, std::string fshader, MeshPainter* painter, AnimationStateManager& animation_manager) {
    mesh = new TriMesh();
    obj = new openGLObject();
    mesh->generateGround(glm::vec3(0.5, 0.5, 0));
	//mesh->vertex_textures;
	//vertex_textures.push_back(glm::vec2(0.0, 0.0));
	//vertex_textures.push_back(glm::vec2(1.0, 0.0));
	//vertex_textures.push_back(glm::vec2(1.0, 1.0));
	//vertex_textures.push_back(glm::vec2(0.0, 1.0));
    mesh->setNormalize(false);
    //--------
    mesh->setTranslation(glm::vec3(2.0, 2.0, 0.85));
    mesh->setRotation(glm::vec3(90.0, 0.0, 210.0));
    mesh->setScale(glm::vec3(scale0, scale0, scale0));
    //传递平面顶点数据。
    std::string str = "./assets/goat.png";
    MeshPainter::addMesh(*mesh, *obj, str, vshader, fshader); 	// 指定纹理与着色器

	animation_manager.AddState( "idle-L", AnimationState(&id,{ {2.5f,2}, {0.6f,3}, {0.3f,1} }));
	animation_manager.AddState( "idle-R", AnimationState(&id,{ {2.5f,-2}, {0.6f,-3}, {0.3f,-1} }));

	animation_manager.AddState( "eat-L", AnimationState(&id,{ {1.8f,4}, {1.8f,5}}));
	animation_manager.AddState( "eat-R", AnimationState(&id,{ {1.8f,-4}, {1.8f,-5}}));
}

void Goat::draw(int type, Light* light, Camera* camera, glm::mat4 lightSpaceMatrix, bool mirror) {
	auto p = calculate_p(id);
	//auto t = mesh->getPoints();
	//auto m = mesh->getTextures();
	//std::vector<float> v;
	//for(int i=0;i<t.size();i++){
	//	printf("line %d:\n", __LINE__);
	//	v.push_back(t[i].x);
	//	v.push_back(t[i].y);
	//	v.push_back(t[i].z);
	//	printf("line %d:\n", __LINE__);
	//	v.push_back(m[i].x);
	//	v.push_back(m[i].y);
	//}
	//int cnt = 0;
	//for(auto x:v){
	//	printf("%f,", x);
	//	if( ++cnt%5==0 ) puts("");
	//} puts("");
	//	printf("line %d: id %d\n", __LINE__, id);
	//for(auto x:p) printf("%f,", x); puts("");
	mesh->setTextures({
			glm::vec2(p[2], 1-p[1]),
			glm::vec2(p[3], 1-p[1]),
			glm::vec2(p[3], 1-p[0]),
			glm::vec2(p[2], 1-p[1]),
			glm::vec2(p[3], 1-p[0]),
			glm::vec2(p[2], 1-p[0])
//			glm::vec2(p[0], p[2]),
//			glm::vec2(p[0], p[3]),
//			glm::vec2(p[1], p[3]),
//			glm::vec2(p[0], p[2]),
//			glm::vec2(p[1], p[3]),
//			glm::vec2(p[1], p[2])
	});
    mesh->setScale(glm::vec3(scale0/p[4], scale0, scale0));
	MeshPainter::bindAgain(*mesh, *obj);
    if (type) 
        MeshPainter::drawMesh(*mesh, *obj, light, camera, lightSpaceMatrix, mirror);
    else
        MeshPainter::simpledrawMesh(*mesh, *obj, lightSpaceMatrix, mirror);
}

void Goat::update_light_model(int Mode) {
    if (Mode == Light::DARK) {
        mesh->setAmbient(glm::vec4(0.1, 0.1, 0.1, 1.0));
        mesh->setDiffuse(glm::vec4(0.9, 0.9, 0.9, 1.0));
    }
    else {
        mesh->setAmbient(glm::vec4(0.2, 0.2, 0.2, 1.0));
        mesh->setDiffuse(glm::vec4(0.8, 0.8, 0.8, 1.0));
    }
    mesh->setSpecular(glm::vec4(0.0, 0.0, 0.0, 1.0));
}

void Goat::clear() {
    mesh->cleanData();
    delete mesh;
    mesh = NULL;
    glDeleteVertexArrays(1, &obj->vao);
    glDeleteBuffers(1, &obj->vbo);
    glDeleteProgram(obj->program);
    delete obj;
    obj = NULL;
}

void BackGround::init(std::string vshader, std::string fshader, MeshPainter* painter) {
    mesh.resize(6);
    for (int i = 0; i < 6; i++) 
        mesh[i] = new TriMesh();
    obj.resize(6);
    for (int i = 0; i < 6; i++)
        obj[i] = new openGLObject();
	float pos = 40.0;
    std::string str;
    glm::vec3 Black(0, 0, 0);
    mesh[Front]->setNormalize(false);
    mesh[Front]->generateSquare(Black);
    mesh[Front]->setTranslation(glm::vec3(0.0, pos, 0.0));
    mesh[Front]->setScale(glm::vec3(2*pos, 2*pos, 2*pos));
    mesh[Front]->setRotation(glm::vec3(90, 0, 0.0));
    str = "./assets/skybox2/front.jpg";
    MeshPainter::addMesh(*mesh[Front], *obj[Front], str, vshader, fshader);

    mesh[Bottom]->setNormalize(false);
    mesh[Bottom]->generateSquare(Black);
    mesh[Bottom]->setTranslation(glm::vec3(0.0, 0.0, -pos));
    mesh[Bottom]->setScale(glm::vec3(2*pos, 2*pos, 2*pos));
    mesh[Bottom]->setRotation(glm::vec3(0.0, 0.0, 0.0));
    str = "./assets/skybox2/bottom.jpg";
    MeshPainter::addMesh(*mesh[Bottom], *obj[Bottom], str, vshader, fshader);

    mesh[Top]->setNormalize(false);
    mesh[Top]->generateSquare(Black);
    mesh[Top]->setTranslation(glm::vec3(0.0, 0.0, pos));
    mesh[Top]->setScale(glm::vec3(2*pos, 2*pos, 2*pos));
    mesh[Top]->setRotation(glm::vec3(0.0, 180.0, 0.0));
    str = "./assets/skybox2/top.jpg";
    MeshPainter::addMesh(*mesh[Top], *obj[Top], str, vshader, fshader);


    mesh[Right]->setNormalize(false);
    mesh[Right]->generateSquare(Black);
    mesh[Right]->setTranslation(glm::vec3(-pos, 0.0, 0.0));
    mesh[Right]->setScale(glm::vec3(2*pos, 2*pos, 2*pos));
    mesh[Right]->setRotation(glm::vec3(90.0, 0.0, 90.0));
    str = "./assets/skybox2/right.jpg";
    MeshPainter::addMesh(*mesh[Right], *obj[Right], str, vshader, fshader);

    mesh[Left]->setNormalize(false);
    mesh[Left]->generateSquare(Black);
    mesh[Left]->setTranslation(glm::vec3(pos, 0.0, 0.0));
    mesh[Left]->setScale(glm::vec3(2*pos, 2*pos, 2*pos));
    mesh[Left]->setRotation(glm::vec3(90.0, 0.0, 270.0));
    str = "./assets/skybox2/left.jpg";
    MeshPainter::addMesh(*mesh[Left], *obj[Left], str, vshader, fshader);    

    mesh[Back]->setNormalize(false);
    mesh[Back]->generateSquare(Black);
    mesh[Back]->setTranslation(glm::vec3(0.0, -pos, 0.0));
    mesh[Back]->setScale(glm::vec3(2*pos, 2*pos, 2*pos));
    mesh[Back]->setRotation(glm::vec3(90.0, 0.0, 180.0));
    str = "./assets/skybox2/back.jpg";
    MeshPainter::addMesh(*mesh[Back], *obj[Back], str, vshader, fshader);    

    table.init(str = "hd2d-table-001", vshader, fshader, glm::vec3(11, 8, 0.0), glm::vec3(90.0, 0.0, 90.0), glm::vec3(1.0));
    cottage.init(str = "CottageOBJ", vshader, fshader, glm::vec3(13, -4, 0.0), glm::vec3(90.0, 0.0, 0.0), glm::vec3(0.10));
    grass_land.init(vshader, fshader, painter);
	//Goat goat[3] = {
	//	Goat(config_manager, goat_animation_manager[0], floor_shader),
	//	Goat(config_manager, goat_animation_manager[1], floor_shader),
	//	Goat(config_manager, goat_animation_manager[2], floor_shader)
	//};

	goat[0].init("shaders/vshader.glsl", "shaders/fshader.glsl", painter, goat_animation_manager[0]);
	goat[1].init("shaders/vshader.glsl", "shaders/fshader.glsl", painter, goat_animation_manager[1]);
	goat[2].init("shaders/vshader.glsl", "shaders/fshader.glsl", painter, goat_animation_manager[2]);
    goat[0].mesh->setTranslation(glm::vec3(9.0, -6.0, 0.85) + glm::vec3{-1.5,  0, 0});
    goat[1].mesh->setTranslation(glm::vec3(9.0, -6.0, 0.85) + glm::vec3{-2.5,  2, 0});
    goat[2].mesh->setTranslation(glm::vec3(9.0, -6.0, 0.85) + glm::vec3{-4.0, -3, 0});
	goat_animation_manager[0].SetState("idle-R");
	goat_animation_manager[1].SetState("idle-L");
	goat_animation_manager[2].SetState("eat-L");
}


void BackGround::update_light_model(int Mode) {
    for (int i = 0; i < 6; i++) {
        if (Mode == Light::LIGHT) mesh[i]->setAmbient(glm::vec4(1.0, 1.0, 1.0, 1.0));
        else mesh[i]->setAmbient(glm::vec4(0.1, 0.1, 0.1, 1.0));
        mesh[i]->setDiffuse(glm::vec4(0.0, 0.0, 0.0, 1.0));
        mesh[i]->setSpecular(glm::vec4(0.0, 0.0, 0.0, 1.0));
    }
    table.update_light_model(Mode);
    cottage.update_light_model(Mode);
    grass_land.update_light_model(Mode);
	goat[0].update_light_model(Mode);
	goat[1].update_light_model(Mode);
	goat[2].update_light_model(Mode);
}

void BackGround::draw_bg(int type, Light* light, Camera* camera, glm::mat4 lightSpaceMatrix, bool mirror) {
    for (int i = 0; i < 6; i++) {
        if (type) 
            MeshPainter::drawMesh(*mesh[i], *obj[i], light, camera, lightSpaceMatrix, mirror);
        else 
            MeshPainter::simpledrawMesh(*mesh[i], *obj[i], lightSpaceMatrix, mirror);
    }
    table.draw(type, light, camera, lightSpaceMatrix, mirror);
    cottage.draw(type, light, camera, lightSpaceMatrix, mirror);
    grass_land.draw(type, light, camera, lightSpaceMatrix, mirror);
	goat[0].draw(type, light, camera, lightSpaceMatrix, mirror);
	goat[1].draw(type, light, camera, lightSpaceMatrix, mirror);
	goat[2].draw(type, light, camera, lightSpaceMatrix, mirror);
}

void BackGround::clear() {
    for (int i = 0; i < 6; i++) {
        mesh[i]->cleanData();
        delete mesh[i];
        mesh[i] = NULL;
#ifdef __APPLE__
        glDeleteVertexArraysAPPLE(1, &opengl_objects[i].vao);
#else
        glDeleteVertexArrays(1, &obj[i]->vao);
#endif
        glDeleteBuffers(1, &obj[i]->vbo);
        glDeleteProgram(obj[i]->program);
        delete obj[i];
        obj[i] = NULL;
    }
    table.clear();
    cottage.clear();
    grass_land.clear();
	goat[0].clear();
	goat[1].clear();
	goat[2].clear();
}


void Mirror::init() {
    mesh = new TriMesh();
    mesh->setNormalize(false);
    mesh->generateSquare(glm::vec3(1.0, 1.0, 1.0));
    mesh->setTranslation(glm::vec3(0.0, 0.0, 2.5));
    mesh->setScale(glm::vec3(5.0, 5.0, 5.0));
    mesh->setRotation(glm::vec3(90.0, 0, 180.0));
    obj = new openGLObject();
    std::string str = "./assets/skybox2/front.jpg";
    std::string vshader = "shaders/vshader.glsl";
    std::string fshader = "shaders/mirror_fshader.glsl";
    MeshPainter::addMesh(*mesh, *obj, str, vshader, fshader);	
}

void Mirror::update_light_model(int Mode) {
    if (Mode == Light::DARK) {
        mesh->setAmbient(glm::vec4(0.1, 0.1, 0.1, 1.0));
        mesh->setDiffuse(glm::vec4(0.9, 0.9, 0.9, 1.0));
    }
    else {
        mesh->setAmbient(glm::vec4(0.3, 0.3, 0.3, 1.0));
        mesh->setDiffuse(glm::vec4(0.8, 0.8, 0.8, 1.0));
    }
    mesh->setSpecular(glm::vec4(0.7, 0.7, 0.7, 1.0));
}

void Mirror::update_texture_and_PVMatrix(unsigned int texture, Camera* camera) {
    obj->texture = texture;
    PVMatrix = camera->getProjectionMatrix(false) * camera->getViewMatrix();
}
void Mirror::draw(int type, Light* light, Camera* camera, glm::mat4 lightSpaceMatrix, bool mirror)
{
    glEnable(GL_CULL_FACE);
    if (type)
        MeshPainter::drawMirror(PVMatrix, *mesh, *obj, light, camera, lightSpaceMatrix, mirror);
    else
        MeshPainter::simpledrawMesh(*mesh, *obj, lightSpaceMatrix, mirror);
    glDisable(GL_CULL_FACE);
}

void MirrorBack::init() {
    mesh = new TriMesh();
    mesh->setNormalize(false);
    mesh->generateSquare(glm::vec3(1.0, 1.0, 1.0));
    mesh->setTranslation(glm::vec3(0.0, -0.01f, 2.5));
    mesh->setScale(glm::vec3(5.0, 5.0, 5.0));
    mesh->setRotation(glm::vec3(90.0, 0, 0.0));
    obj = new openGLObject();
    std::string str = "./assets/skybox2/bottom.jpg";
    std::string vshader = "shaders/vshader.glsl";
    std::string fshader = "shaders/fshader.glsl";
    MeshPainter::addMesh(*mesh, *obj, str, vshader, fshader);	
}
void MirrorBack::update_light_model(int Mode) {
    if (Mode == Light::DARK) {
        mesh->setAmbient(glm::vec4(0.1, 0.1, 0.1, 1.0));
        mesh->setDiffuse(glm::vec4(0.3, 0.3, 0.3, 1.0));
    }
    else {
        mesh->setAmbient(glm::vec4(0.2, 0.2, 0.2, 1.0));
        mesh->setDiffuse(glm::vec4(0.2, 0.2, 0.2, 1.0));
    }
    mesh->setSpecular(glm::vec4(0.0, 0.0, 0.0, 1.0));
}
void MirrorBack::draw(int type, Light* light, Camera* camera, glm::mat4 lightSpaceMatrix, bool mirror) {
    glEnable(GL_CULL_FACE);
    if (type)
        MeshPainter::drawMesh(*mesh, *obj, light, camera, lightSpaceMatrix, mirror);
    else
        MeshPainter::simpledrawMesh(*mesh, *obj, lightSpaceMatrix, mirror);
    glDisable(GL_CULL_FACE);
}
