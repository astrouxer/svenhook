#pragma once

#include <windows.h>
#include <string>
#include <format>
#include <cstdio>

#include "sdk.hpp"
#include "safetyhook/safetyhook.hpp"

extern cl_enginefunc_t* engine;
extern playermove_t* pmove;
extern engine_studio_api_t* e_studio;
extern r_studio_interface_t* i_studio;
extern cl_entity_s* localplayer;

#include "features/movement.hpp"
#include "features/visuals.hpp"