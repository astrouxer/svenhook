#include "include.hpp"

cl_enginefunc_t* engine = nullptr;
playermove_t* pmove = nullptr;
engine_studio_api_t* e_studio = nullptr;
r_studio_interface_t* i_studio = nullptr;
cl_entity_s* localplayer = nullptr;

bool toad_mode;

inline SafetyHookInline createmove_hook{};
void CreateMove(float frametime, struct usercmd_s* cmd, int active)
{
	createmove_hook.call<void>(frametime, cmd, active);
	localplayer = engine->GetLocalPlayer();

	if (Movement::Bunnyhop::Enabled)
		Movement::Bunnyhop::Run(cmd);

	if (Movement::Autostrafe::Enabled)
		Movement::Autostrafe::Run(cmd);
}

inline SafetyHookInline redraw_hook{};
int Redraw(__int64 a1)
{
	int result = redraw_hook.call<int>(a1);

	if (engine != nullptr)
	{
		SCREENINFO screenInfo;
		screenInfo.iSize = sizeof(SCREENINFO);
		engine->pfnGetScreenInfo(&screenInfo);

		engine->pfnDrawSetTextColor(75, 0, 130);
		engine->pfnDrawConsoleString(5, 5, (char*)"sven hook");

		char slop1[32], slop2[32], slop3[32];
		snprintf(slop1, sizeof(slop1), "BHOP [%s]", Movement::Bunnyhop::Enabled ? "ON" : "OFF");
		snprintf(slop2, sizeof(slop2), "AUTOSTRAFE [%s]", Movement::Autostrafe::Enabled ? "ON" : "OFF");
		snprintf(slop3, sizeof(slop3), "TOAD MODE [%s]", toad_mode ? "ON" : "OFF");

		engine->pfnDrawSetTextColor(75, 0, 130);
		engine->pfnDrawConsoleString(5, 20, slop1);

		engine->pfnDrawSetTextColor(75, 0, 130);
		engine->pfnDrawConsoleString(5, 35, slop2);

		engine->pfnDrawSetTextColor(75, 0, 130);
		engine->pfnDrawConsoleString(5, 50, slop3);

		if (localplayer != nullptr && pmove != nullptr)
		{
			std::string velocity = std::to_string((int)pmove->velocity.Length());
			engine->pfnDrawSetTextColor(75, 0, 130);
			engine->pfnDrawConsoleString(screenInfo.iWidth / 2, screenInfo.iHeight / 2 + 10, (char*)velocity.c_str());
		}
	}

	return result;
}

// epic model swapper
inline SafetyHookInline studio_draw_model_hook{};
int StudioDrawModel(int flags) 
{
	if (toad_mode && e_studio) 
	{
		cl_entity_t* ent = e_studio->GetCurrentEntity();
		if (ent && ent->model && ent->model->name) 
		{
			model_t* custommodel = e_studio->Mod_ForName("models/bshift/chumtoad.mdl", 1);
			if (custommodel)
			{
				model_t* originalmodel = ent->model;
				ent->model = custommodel;
				int result = studio_draw_model_hook.call<int>(flags);
				ent->model = originalmodel;
				return result;
			}
		}
	}

	return studio_draw_model_hook.call<int>(flags);
}

DWORD WINAPI CheatThread(LPVOID lpParam) 
{
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    SetConsoleTitleA("sven hook - unload key: end");

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
	e_studio = GetEngineStudio();
	i_studio = GetStudioInterface();

	printf("e_studio: %p\n", e_studio);
	printf("i_studio: %p\n", i_studio);

	printf("\n");
	printf("bhop: page up\n");
	printf("autostrafe: page down\n");
	printf("toad mode: f10\n");

	while (!i_studio)
		Sleep(100);

	studio_draw_model_hook = safetyhook::create_inline(reinterpret_cast<void*>(i_studio->StudioDrawModel), reinterpret_cast<void*>(StudioDrawModel));

	while (true)
	{
		if (GetAsyncKeyState(VK_END) & 0x8000)
			break;

		if (GetAsyncKeyState(VK_PRIOR) & 1)
			Movement::Bunnyhop::Enabled = !Movement::Bunnyhop::Enabled;

		if (GetAsyncKeyState(VK_NEXT) & 1)
			Movement::Autostrafe::Enabled = !Movement::Autostrafe::Enabled;

		if (GetAsyncKeyState(VK_F10) & 1)
			toad_mode = !toad_mode;
	}

	printf("unloading...\n");

	createmove_hook.reset();
	redraw_hook.reset();
	studio_draw_model_hook.reset();

	fclose(f);
	FreeConsole();
	FreeLibraryAndExitThread((HMODULE)lpParam, 0);
    return TRUE;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        CreateThread(NULL, 0, CheatThread, hModule, 0, NULL);
    }
    return TRUE;
}
