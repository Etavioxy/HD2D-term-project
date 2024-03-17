#include "Camera.h"

Camera::Camera() { updateCamera(); };
Camera::~Camera() {}

glm::mat4 Camera::getViewMatrix()
{
	if(camera_type)
		return this->lookAt(eye, at, up);
	return this->lookAt(eye, free_at, up);
}

glm::mat4 Camera::getProjectionMatrix(bool isOrtho)
{
	if (isOrtho) {
		return this->ortho(-scale, scale, -scale, scale, this->zNear, this->zFar);
	}
	else {
		return this->perspective(fov, aspect, this->zNear, this->zFar);
	}
}

glm::mat4 Camera::lookAt(const glm::vec4& eye, const glm::vec4& at, const glm::vec4& up)
{
	// use glm.
			

	glm::vec3 eye_3 =  eye;
	glm::vec3 at_3 = at;
	if (camera_type)
	{
		eye_3.x *= camera_type;
		eye_3.y *= camera_type;
	}
	eye_3 += at_3;
	glm::vec3 up_3 = up;
	eye_3.z = std::max(0.01f, eye_3.z);
	glm::mat4 view = glm::lookAt(eye_3, at_3, up_3);

	return view;

}

glm::mat4 Camera::ortho(const GLfloat left, const GLfloat right,
	const GLfloat bottom, const GLfloat top,
	const GLfloat zNear, const GLfloat zFar)
{
	glm::mat4 c = glm::mat4(1.0f);
	c[0][0] = 2.0 / (right - left);
	c[1][1] = 2.0 / (top - bottom);
	c[2][2] = -2.0 / (zFar - zNear);
	c[3][3] = 1.0;
	c[0][3] = -(right + left) / (right - left);
	c[1][3] = -(top + bottom) / (top - bottom);
	c[2][3] = -(zFar + zNear) / (zFar - zNear);

	c = glm::transpose(c);
	return c;
}

glm::mat4 Camera::perspective(const GLfloat fovy, const GLfloat aspect,
	const GLfloat zNear, const GLfloat zFar)
{
	GLfloat top = tan(fovy * M_PI / 180 / 2) * zNear;
	GLfloat right = top * aspect;

	glm::mat4 c = glm::mat4(1.0f);
	c[0][0] = zNear / right;
	c[1][1] = zNear / top;
	c[2][2] = -(zFar + zNear) / (zFar - zNear);
	c[2][3] = -(2.0 * zFar * zNear) / (zFar - zNear);
	c[3][2] = -1.0;
	c[3][3] = 0.0;

	c = glm::transpose(c);
	return c;
}

glm::mat4 Camera::frustum(const GLfloat left, const GLfloat right,
	const GLfloat bottom, const GLfloat top,
	const GLfloat zNear, const GLfloat zFar)
{
	// 任意视锥体矩阵
	glm::mat4 c = glm::mat4(1.0f);
	c[0][0] = 2.0 * zNear / (right - left);
	c[0][2] = (right + left) / (right - left);
	c[1][1] = 2.0 * zNear / (top - bottom);
	c[1][2] = (top + bottom) / (top - bottom);
	c[2][2] = -(zFar + zNear) / (zFar - zNear);
	c[2][3] = -2.0 * zFar * zNear / (zFar - zNear);
	c[3][2] = -1.0;
	c[3][3] = 0.0;

	c = glm::transpose(c);
	return c;
}


void Camera::updateCamera()
{
	// 使用相对于at的角度控制相机的时候，注意在upAngle大于90的时候，相机坐标系的u向量会变成相反的方向，
	// 要将up的y轴改为负方向才不会发生这种问题

	// 也可以考虑直接控制相机自身的俯仰角，
	// 保存up，eye-at 这些向量，并修改这些向量方向来控制
	// 看到这里的有缘人可以试一试
	up = glm::vec4(0.0, 0.0, 1.0, 0.0);
	eye.x = radius * sin(glm::radians(1.0 * beita)) * cos(glm::radians(1.0 * aerfa));//cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	eye.y = radius * sin(glm::radians(1.0 * beita)) * sin(glm::radians(1.0 * aerfa));//sin(glm::radians(pitch));
	eye.z = radius * cos(glm::radians(1.0 * beita));//sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	at = glm::vec4(0.0, 0.0, 1.5, 1.0);
	
	at.x += move.x;
	at.y += move.y;
	at.z += move.z;
	tmp = 0;
}

void Camera::setCamera(glm::vec3 lightp)
{
	eye = glm::vec4(lightp, 1.0);
	at = glm::vec4(0.0, 0.0, 0.0, 1.0);
	up = glm::vec4(0.0, 0.0, 1.0, 0.0);
	tmp = 1;
}

void Camera::keyboard(int key, int action, int mode)
{
	
	
	if (key == GLFW_KEY_W && action == GLFW_REPEAT && mode == 0x0000)
	{
		free_at.x -= eye.x*0.05;
		free_at.y -= eye.y*0.05;
		free_at.z -= eye.z*0.05;
	}
	else if (key == GLFW_KEY_S && action == GLFW_REPEAT && mode == 0x0000)
	{
		free_at.x += eye.x * 0.05;
		free_at.y += eye.y * 0.05;
		free_at.z += eye.z * 0.05;
	}
	else if (key == GLFW_KEY_F1 && action == GLFW_PRESS && mode == 0x0000)
	{
		camera_type = 1;
	}
	else if (key == GLFW_KEY_F2 && action == GLFW_PRESS && mode == 0x0000)
	{
		camera_type = -1;
	}
	else if (key == GLFW_KEY_F3 && action == GLFW_PRESS && mode == 0x0000)
	{
		camera_type = 0;
		free_at = at;
		free_move = move;
	}
}


void Camera::mousemove(double xpos, double ypos)
{
	//记录初始位置
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to left
	lastX = xpos;
	lastY = ypos;
	//移动速度
	GLfloat sensitivity = 0.03;	// Change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	
	aerfa -= xoffset;
	beita += yoffset;
	//限制上下角度
	if (beita > 135)
		beita = 135;
	if (beita < 1)
		beita = 1;

}
