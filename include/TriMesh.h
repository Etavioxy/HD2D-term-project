#ifndef _TRI_MESH_H_
#define _TRI_MESH_H_

#include "Angel.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <float.h>


// 三角面片中的顶点序列
typedef struct vIndex {
	unsigned int x, y, z;
	vIndex(int ix, int iy, int iz) : x(ix), y(iy), z(iz) {}
} vec3i;

class TriMesh
{
public:
	TriMesh();
	~TriMesh();

	std::vector<glm::vec3> getVertexPositions();
	std::vector<glm::vec3> getVertexColors();
	std::vector<glm::vec3> getVertexNormals();
	std::vector<glm::vec2> getVertexTextures();

	std::vector<vec3i> getFaces();
	std::vector<glm::vec3> getPoints();
	std::vector<glm::vec3> getColors();
	std::vector<glm::vec3> getNormals();
	std::vector<glm::vec2> getTextures();
	void setTextures(const std::vector<glm::vec2>&);

	void computeTriangleNormals();
	void computeVertexNormals();

	// 获取和设置物体的旋转平移变化
	glm::vec3 getTranslation();
	glm::vec3 getRotation();
	glm::vec3 getScale();
	glm::mat4 getChangeModelMatrix();
	glm::mat4 getModelMatrix();

	void setNormalize(bool do_norm);
	bool getNormalize();
	float getDiagonalLength();

	void setTranslation(glm::vec3 translation);
	void setRotation(glm::vec3 rotation);
	void setScale(glm::vec3 scale);
	void setChangeModelMatrix(glm::mat4 matrix);
	void changeTranslation(glm::vec3 translation);



	// 设置物体材质
	glm::vec4 getAmbient();
	glm::vec4 getDiffuse();
	glm::vec4 getSpecular();
	float getShininess();

	void setAmbient(glm::vec4 _ambient );
	void setDiffuse(glm::vec4 _diffuse);
	void setSpecular(glm::vec4 _specular);
	void setShininess(float shininess);

	// 这里添加各种形状的、模型的读取顶点的函数
	void generateCube();
	void generateTriangle(glm::vec3 color);
	void generateSquare(glm::vec3 color);
	void generateGround(glm::vec3 color);
	void readOff(const std::string& filename);
	void readObj(const std::string& filename);
	// 将读取的顶点根据三角面片上的顶点下标逐个加入
	// 要传递给GPU的points等容器内
	void storeFacesPoints();
	float getLenX();
	float getLenY();
	float getLenZ();

	// 清除数据
	void cleanData();

protected:
	std::vector<glm::vec3> vertex_positions;	// 顶点坐标
	std::vector<glm::vec3> vertex_colors;	// 顶点颜色
	std::vector<glm::vec3> vertex_normals;	// 顶点法向量
	std::vector<glm::vec2> vertex_textures;	// 顶点纹理坐标，注意是vec2

	std::vector<vec3i> faces;	// 三角面片上每个顶点对应的顶点坐标的下标
	std::vector<vec3i> normal_index;	// 每个三角面片的顶点对应的顶点法向量的下标
	std::vector<vec3i> color_index;		// 每个三角面片的顶点对应的顶点颜色的下标
	std::vector<vec3i> texture_index;	// 每个三角面片的顶点对应纹理坐标的下标

	std::vector<glm::vec3> face_normals;	// 每个三角面片的法向量

	std::vector<glm::vec3> points;	// 传入着色器的绘制点
	std::vector<glm::vec3> colors;	// 传入着色器的颜色
	std::vector<glm::vec3> normals;	// 传入着色器的法向量
	std::vector<glm::vec2> textures;	// 传入着色器的纹理坐标，注意是vec2

	bool do_normalize_size;        // 是否将物体大小归一化
	float diagonal_length;      // 物体包围盒对角线长度，作为物体归一化系数
	float LENX = 0;				//确定x轴宽度；

	glm::vec3 up_corner;				// 物体包围盒的上对角顶点
	glm::vec3 down_corner;           // 物体包围盒的下对角顶点
	glm::vec3 center;                // 物体的包围盒中心

	glm::vec3 translation;			// 物体的平移参数
	glm::vec3 rotation;				// 物体的旋转参数
	glm::vec3 scale;					// 物体的缩放参数
	glm::mat4 change_model_matrix;
	
	glm::vec4 ambient = { 0.2,0.2,0.2,1.0 };				// 环境光
	glm::vec4 diffuse= { 0.7,0.7,0.7,0.0 };				// 漫反射
	glm::vec4 specular= { 0.0,0.0,0.0,1.0 };				// 镜面反射
	float shininess = {1.0};			// 高光系数

};


class Light : public TriMesh {
public:
	enum {
		LIGHT,
		DARK
	};
	// 把实验3.2的阴影投影矩阵也加到Light类中
	glm::mat4 getShadowProjectionMatrix();

	// 设置衰减系数的参数
	// https://learnopengl-cn.readthedocs.io/zh/latest/02%20Lighting/05%20Light%20casters/

	void setAttenuation(float _constant, float _linear, float _quadratic);

	float getConstant();
	float getLinear();
	float getQuadratic();

protected:
	// 光源衰减系数的三个参数
	float constant=1.0; // 常数项
	float linear=0.045;	// 一次项
	float quadratic=0.0075;// 二次项
};


#endif
