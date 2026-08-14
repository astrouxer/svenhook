#pragma once
#include "../include.hpp"

namespace Visuals
{
	namespace ESP
	{
        inline static bool Enabled = true;
		void Run();

        inline void DrawBox(int x, int y, int w, int h, int r, int g, int b, int a = 255, int lineThickness = 1) 
        {
            engine->pfnFillRGBA(x, y, w, lineThickness, r, g, b, a);
            engine->pfnFillRGBA(x, y + h - lineThickness, w, lineThickness, r, g, b, a);
            engine->pfnFillRGBA(x, y, lineThickness, h, r, g, b, a);
            engine->pfnFillRGBA(x + w - lineThickness, y, lineThickness, h, r, g, b, a);
        }

        inline bool WorldToScreen(float* vec, float* screen)
        {
            if (!vec || !screen || (vec[0] == 0.0f && vec[1] == 0.0f && vec[2] == 0.0f))
                return false;

            float clip_output[3] = { 0.0f, 0.0f, 0.0f };
            int w2s = engine->pTriAPI->WorldToScreen(vec, clip_output);
            if (w2s)
                return false;

            if (clip_output[0] < -1.0f || clip_output[0] > 1.0f ||
                clip_output[1] < -1.0f || clip_output[1] > 1.0f)
            {
                return false;
            }

            SCREENINFO_s scr_inf;
            scr_inf.iSize = sizeof(SCREENINFO_s);
            engine->pfnGetScreenInfo(&scr_inf);

            screen[0] = clip_output[0] * (scr_inf.iWidth / 2.0f) + (scr_inf.iWidth / 2.0f);
            screen[1] = -clip_output[1] * (scr_inf.iHeight / 2.0f) + (scr_inf.iHeight / 2.0f);

            return true;
        }

        inline int CalcTextSize(const char* string, int box_x, int box_width)
        {
            int w = 0, h = 0;
            engine->pfnDrawConsoleStringLen(string, &w, &h);
            return (box_x + (box_width / 2)) - (w / 2);
        }
	}
}