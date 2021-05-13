#pragma once

// OpenGL Extension Wrangler
#include <GL/glew.h> 
#include <GL/wglew.h> //WGLEW = Windows GL Extension Wrangler (change for different platform) 

// GLFW toolkit
#include <GLFW/glfw3.h>

// OpenCV 
#include <opencv2\opencv.hpp>

typedef struct  Avatar {   // camera (player) info
	float       posX;
	float       posY;      //height
	float       posZ;
	float       move_h_angle;   // Yaw
	float		move_v_angle;   // Pitch
	   					       //(no Roll)
	float       cam_h_angle;   // Yaw
	float		cam_v_angle;   // Pitch
							   //(no Roll)

	float		mouse_sensitivity;
	bool		lock_cam_move_angles;
	float		movement_speed;
} Avatar;


struct s_globals {
	GLFWwindow* window;
	bool fullScreen;
	int height;
	int width;
	double app_start_time;

	Avatar * camera;

	cv::VideoCapture capture;
};

extern s_globals globals;

Avatar avatarMoveForward(Avatar& avatar);
Avatar avatarMoveBackward(Avatar& avatar);
Avatar avatarMoveLeft(Avatar& avatar);
Avatar avatarMoveRight(Avatar& avatar);
Avatar avatarMoveUp(Avatar& avatar);
Avatar avatarMoveDown(Avatar& avatar);

Avatar avatarRotate(Avatar& avatar, const float yaw, const float pitch, const float roll);
