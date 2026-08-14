#include "visuals.hpp"

void Visuals::ESP::Run()
{
	int max_clients = engine->GetMaxClients();
	for (int i = 1; i <= max_clients; i++)
	{
		cl_entity_s* entity = engine->GetEntityByIndex(i);
		if (!entity || !entity->player || entity->index == localplayer->index)
			continue;

		//if (entity->curstate.team == localplayer->curstate.team) // le teamcheck
		//	continue;

		//if (entity->curstate.health > 0) // only alive check I could come up with (doesnt work apparently)
		//	continue;

		if (entity->origin.x == 0.0f && entity->origin.y == 0.0f && entity->origin.z == 0.0f)
			continue;

		float origin[3] = {entity->origin.x, entity->origin.y, entity->origin.z - 36.0f};
		float top[3] = {entity->origin.x, entity->origin.y, entity->origin.z + 36.0f};

		float scr_bottom[2] = { 0.0f, 0.0f };
		float scr_top[2] = { 0.0f, 0.0f };

		if (WorldToScreen(origin, scr_bottom) && WorldToScreen(top, scr_top))
		{
			int height = (int)fabs(scr_bottom[1] - scr_top[1]);
			int width = height / 2;

			int x = (int)scr_top[0] - (width / 2);
			int y = (int)scr_top[1];

			DrawBox(x, y, width, height, 255, 0, 0);

			hud_player_info_t playerinfo;
			engine->pfnGetPlayerInfo(entity->index, &playerinfo);

			if (playerinfo.name)
			{
				engine->pfnDrawSetTextColor(255 / 255, 0, 0);
				engine->pfnDrawConsoleString(CalcTextSize(playerinfo.name, x, width), y - 15, (char*)playerinfo.name);
			}
		}
	}
}