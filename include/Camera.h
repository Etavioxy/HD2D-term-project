#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Angel.h"

class Camera
{
public:
	Camera();
	~Camera();

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix( bool isOrtho );

	glm::mat4 lookAt(const glm::vec4& eye, const glm::vec4& at, const glm::vec4& up);

	glm::mat4 ortho(const GLfloat left, const GLfloat right,
		const GLfloat bottom, const GLfloat top,
		const GLfloat zNear, const GLfloat zFar);

	glm::mat4 perspective(const GLfloat fov, const GLfloat aspect,
		const GLfloat zNear, const GLfloat zFar);

	glm::mat4 frustum(const GLfloat left, const GLfloat right,
		const GLfloat bottom, const GLfloat top,
		const GLfloat zNear, const GLfloat zFar);

	// 每次更改相机参数后更新一下相关的数值
	void updateCamera();
	void setCamera(glm::vec3 lightp);
	// 处理相机的键盘操作
	void keyboard(int key, int action, int mode);
	void mousemove(double xpos, double ypos);

	// 模视矩阵
	glm::mat4 viewMatrix;
	glm::mat4 projMatrix;

	// 相机位置参数
	float radius = 5.0;
	float rotateAngle = 0.0;
	float upAngle = 0.0;
	glm::vec4 eye;
	glm::vec4 at;
	glm::vec4 up;
	glm::vec4 free_at;

	// 投影参数
	float zNear = 0.1;
	float zFar = 100.0;
	// 透视投影参数
	float fov = 45.0;
	float aspect = 1.0;
	// 正交投影参数
	float scale = 1.5;
	//移动相关参数
	glm::vec3 move = glm::vec3(0.0,0.0,0.0);
	glm::vec3 free_move = glm::vec3(0.0, 0.0, 0.0);
	//相机随鼠标移动相关参数
	double lastX, lastY;
	bool firstMouse = 1;
	double aerfa=90, beita=90;
	//相机相关状态参数
	int camera_type = 1;
	bool tmp = 0;

	// 镜面摄像机更新
	void mirrorTo(Camera* target_camera, glm::vec4 plane) {
		glm::vec3 plane_normal = glm::normalize(glm::vec3(plane.x, plane.y, plane.z));

		glm::vec3 mirror_at = glm::reflect(glm::vec3(target_camera->at), plane_normal);
		glm::vec3 mirror_up = glm::reflect(glm::vec3(target_camera->up), plane_normal);

		glm::vec3 plane_center = plane_normal * plane.w;
		// printf("%lf %lf %lf\n", plane.x, plane.y, plane.z);
		
		glm::vec3 eye_vector = plane_center - glm::vec3(target_camera->eye);
		float len = glm::length(eye_vector);
		eye_vector = glm::normalize(eye_vector);
		eye_vector = glm::reflect(eye_vector, plane_normal);
		eye_vector = eye_vector * len;
		glm::vec3 mirror_eye = plane_center - eye_vector;
    
    // GLKVector3 planeCenter = GLKVector3MultiplyScalar(planeNormal, plane.w);
    // GLKVector3 eyeVector = GLKVector3Subtract(planeCenter, self.position);
    // CGFloat eyeVectorLength = GLKVector3Length(eyeVector);
    // eyeVector = GLKVector3Normalize(eyeVector);
    // GLKVector3 mirrorEyeVector = GLKVector3Normalize([self reflect:eyeVector normalVector:planeNormal]);
    // mirrorEyeVector = GLKVector3MultiplyScalar(mirrorEyeVector, eyeVectorLength);
    // GLKVector3 mirrorPosition = GLKVector3Subtract(planeCenter, mirrorEyeVector);
    
    // targetCamera.position = mirrorPosition;
    // targetCamera.up = mirrorUp;
    // targetCamera.forward = mirrorForward;

		eye = glm::vec4(mirror_eye, target_camera->eye.w);
		up = glm::vec4(mirror_up, target_camera->up.w);
		at = glm::vec4(mirror_at, target_camera->at.w);
		free_at = at;
		// printf("%lf %lf %lf\n", eye.x, eye.y, eye.z);
		// printf("%lf %lf %lf\n", target_camera->eye.x, target_camera->eye.y, target_camera->eye.z);

		// 模视矩阵
	// // 相机位置参数
	// 	rotateAngle = target_camera->rotateAngle;
	// 	upAngle = target_camera->upAngle;

	// // // 投影参数
	// 	radius = target_camera->radius;

	// 	zNear = target_camera->zNear;
	// 	zFar = target_camera->zFar;
	// 	scale = target_camera->scale;
	// 	fov = target_camera->fov;
	// 	aspect = target_camera->aspect;
	// // // glm::vec3 move = glm::vec3(0.0,0.0,0.0);
	// // // glm::vec3 free_move = glm::vec3(0.0, 0.0, 0.0);
	// 	aerfa = target_camera->aerfa, beita = target_camera->beita;
	// 	camera_type = target_camera->camera_type;
	// 	tmp = target_camera->tmp;

	}
};
#endif
