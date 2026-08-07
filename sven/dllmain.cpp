#include <windows.h>
#include <cstdio>

#include "safetyhook/safetyhook.hpp"
#include "sdk.hpp"
#include <string>
#include <format>

cl_enginefunc_t* engine;
playermove_t* pmove;

cl_entity_s* localplayer;

inline SafetyHookInline createmove_hook{};
void CreateMove(float frametime, struct usercmd_s* cmd, int active)
{
	createmove_hook.call<void>(frametime, cmd, active);

	localplayer = engine->GetLocalPlayer();

	if (localplayer != nullptr)
	{
		if (cmd->buttons & IN_JUMP)
		{
			if (!(pmove->flags & FL_ONGROUND))
				cmd->buttons &= ~IN_JUMP;
		}
	}
}

inline SafetyHookInline redraw_hook{};
int Redraw(__int64 a1)
{
	int result = redraw_hook.call<int>(a1);

	int monitorWidth = GetSystemMetrics(SM_CXSCREEN);
	int monitorHeight = GetSystemMetrics(SM_CYSCREEN);

	if (engine != nullptr)
	{
		engine->pfnDrawSetTextColor(75, 0, 130);
		engine->pfnDrawConsoleString(5, 5, (char*)"sven hook");
		if (localplayer != nullptr && pmove != nullptr)
		{
			std::string velocity = std::to_string((int)pmove->velocity.Length()); // forward vel
			engine->pfnDrawConsoleString(monitorWidth / 2, monitorHeight / 2 + 10, (char*)velocity.c_str());
		}
	}

	return result;
}

DWORD WINAPI CheatThread(LPVOID lpParam) 
{
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    SetConsoleTitleA("sven hook");

	while (GetModuleHandleA("client.dll") == nullptr)
		Sleep(100);

    printf("init!\n");

	uintptr_t createmove_addr = (uintptr_t)GetProcAddress(GetModuleHandleA("client.dll"), "CL_CreateMove");
	printf("createmove: %p\n", (void*)createmove_addr);
	createmove_hook = safetyhook::create_inline(reinterpret_cast<void*>(createmove_addr), reinterpret_cast<void*>(CreateMove));

	uintptr_t redraw_addr = (uintptr_t)GetProcAddress(GetModuleHandleA("client.dll"), "HUD_Redraw");
	printf("redraw: %p\n", (void*)redraw_addr);
	redraw_hook = safetyhook::create_inline(reinterpret_cast<void*>(redraw_addr), reinterpret_cast<void*>(Redraw));

	engine = GetEngineFuncs();
	pmove = GetPlayerMove();

    return TRUE;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        CreateThread(NULL, 0, CheatThread, NULL, 0, NULL);
    }
    return TRUE;
}
