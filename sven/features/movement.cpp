#pragma once
#include "movement.hpp"

void Movement::Bunnyhop::Run(usercmd_s* cmd)
{
	if (cmd->buttons & IN_JUMP)
	{
		if (!(pmove->flags & FL_ONGROUND))
			cmd->buttons &= ~IN_JUMP;
	}
}

void Movement::Autostrafe::Run(usercmd_s* cmd)
{
	if (pmove->flags & FL_ONGROUND)
		return;

	static float old_yaw = 0.0f;
	float cur_yaw = cmd->viewangles.y;

	if (cur_yaw > old_yaw)
		cmd->sidemove = -400.0f;

	if (cur_yaw < old_yaw)
		cmd->sidemove = 400.0f;

	old_yaw = cur_yaw;
}