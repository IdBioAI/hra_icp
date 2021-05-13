#include <glm/glm.hpp>
#include "globals.h"

// global variable - encapsulates all
s_globals globals;

Avatar avatarMoveForward(Avatar& avatar)
{
	Avatar a = avatar;
	
	a.posX += avatar.movement_speed * sin(glm::radians(avatar.move_h_angle));
	a.posY += avatar.movement_speed * cos(glm::radians(avatar.move_h_angle));

	return a;
}

Avatar avatarMoveBackward(Avatar& avatar)
{
	Avatar a = avatar;
	a.posX = avatar.posX - avatar.movement_speed * sin(glm::radians(avatar.move_h_angle));
	a.posY = avatar.posY - avatar.movement_speed * cos(glm::radians(avatar.move_h_angle));
	return a;
}

Avatar avatarMoveLeft(Avatar& avatar)
{
	Avatar a = avatar;
	a.posX = avatar.posX + avatar.movement_speed * cos(glm::radians(avatar.move_h_angle));
	a.posY = avatar.posY - avatar.movement_speed * sin(glm::radians(avatar.move_h_angle));
	return a;
}

Avatar avatarMoveRight(Avatar& avatar)
{
	Avatar a = avatar;
	a.posX = avatar.posX - avatar.movement_speed * cos(glm::radians(avatar.move_h_angle));
	a.posY = avatar.posY + avatar.movement_speed * sin(glm::radians(avatar.move_h_angle));
	return a;
}

Avatar avatarMoveUp(Avatar& avatar)
{
	Avatar a = avatar;
	a.posZ = avatar.posZ + avatar.movement_speed;
	return a;
}

Avatar avatarMoveDown(Avatar& avatar)
{
	Avatar a = avatar;
	a.posZ = avatar.posZ - avatar.movement_speed;
	return a;
}

Avatar avatarRotate(Avatar& avatar, const float yaw, const float pitch, const float roll)
{
	Avatar a = avatar;

	a.cam_h_angle = yaw * a.mouse_sensitivity;
	a.cam_v_angle = pitch * a.mouse_sensitivity;

	if (a.lock_cam_move_angles)
	{
		a.move_h_angle = a.cam_h_angle;
		a.move_v_angle = a.cam_v_angle;
	}

	return a;
}