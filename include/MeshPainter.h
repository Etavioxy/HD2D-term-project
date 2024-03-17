#ifndef _MESH_PAINTER_H_
#define _MESH_PAINTER_H_

#include "TriMesh.h"
#include "Angel.h"

#include "Camera.h"

#include <vector>


struct openGLObject
{
	// 顶点数组对象
	GLuint vao;
	// 顶点缓存对象
	GLuint vbo;

	// 着色器程序
	GLuint program;
	// 着色器文件
	std::string vshader;
	std::string fshader;
	// 着色器变量
	GLuint pLocation;
	GLuint cLocation;
	GLuint nLocation;
	GLuint tLocation;
	GLuint clipLocation;
	GLuint clipplaneLocation;

	// 纹理
	std::string texture_image;
	GLuint texture;

	// 投影变换变量
	GLuint modelLocation;
	GLuint viewLocation;
	GLuint projectionLocation;
	GLuint lightspaceLocation;
	// 阴影变量
	GLuint shadowLocation;
	//材质变量
	GLuint textureLocation;
	GLuint shadowMapLocation;

	struct 
	{
		// 顶点数组对象
		GLuint vao;
		// 顶点缓存对象
		GLuint vbo;

		// 着色器程序
		GLuint program;
		// 着色器文件
		std::string vshader;
		std::string fshader;
		// 着色器变量
		// 模型和光照矩阵
		GLuint pLocation;
		GLuint modelLocation;
		GLuint lightspaceLocation;
	}simpleobject;
};

struct Simpleobj
{
	GLuint program;
};

class MeshPainter
{

public:
	MeshPainter();
	~MeshPainter();

	std::vector<std::string> getMeshNames();

	std::vector<TriMesh*> getMeshes();
	std::vector<openGLObject> getOpenGLObj();
	static Simpleobj simpleobj,obj;
	// 读取纹理文件
	static void load_texture_STBImage(const std::string& file_name, GLuint& texture);
	//计算深度着色器
	static void bindAgain(TriMesh& mesh, openGLObject& object);
	static void simplebindObjectAndData(TriMesh& mesh, openGLObject& object, const std::string& vshader, const std::string& fshader);
	// 传递光线材质数据的
	// void bindLightAndMaterial( int mesh_id, int light_id, Camera* camera );
	static void bindLightAndMaterial(TriMesh& mesh, openGLObject& object, Light* light, Camera* camera);

	static void bindObjectAndData(TriMesh& mesh, openGLObject& object, const std::string& texture_image, const std::string& vshader, const std::string& fshader);

	// 添加物体
	void addMesh(TriMesh& mesh, const std::string& name, const std::string& texture_image, const std::string& vshader, const std::string& fshader);
	static inline void addMesh(TriMesh& mesh, openGLObject& object, const std::string& texture_image, const std::string& vshader, const std::string& fshader)
	{
		// 绑定openGL对象，并传递顶点属性的数据
		bindObjectAndData(mesh, object, texture_image, vshader, fshader);
		std::string simplevshader = "shaders/shadow_mapping_depth_vshader.glsl";
		std::string simplefshader = "shaders/shadow_mapping_depth_fshader.glsl";
		simplebindObjectAndData(mesh, object, simplevshader, simplefshader);
		
	}

	// 用于绘制阴影模型
	static void simpledrawMesh( TriMesh& mesh, openGLObject& object, glm::mat4 lightSpaceMatrix, bool mirror);
	//绘制物体
	static void drawMesh(TriMesh& mesh, openGLObject& object, Light* light, Camera* camera, glm::mat4 lightSpaceMatrix, bool mirror);
	static void drawMirror(glm::mat4 PVMatrix, TriMesh& mesh, openGLObject& object, Light* light, Camera* camera, glm::mat4 lightSpaceMatrix, bool mirror_flag);
	// 绘制多个物体
	void simpledrawMeshes(glm::mat4 lightSpaceMatrix, bool mirror);
	void drawMeshes(Light* light, Camera* camera, glm::mat4 lightSpaceMatrix, bool mirror);
	// 清空数据
	void cleanMeshes();
	static unsigned int depthMap;

private:
	std::vector<std::string> mesh_names;
	std::vector<TriMesh*> meshes;
	std::vector<openGLObject> opengl_objects;
};

#endif
