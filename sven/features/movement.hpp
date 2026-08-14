#pragma once
#include "../include.hpp"

namespace Movement
{
	namespace Bunnyhop
	{
		inline static bool Enabled = true;
		void Run(usercmd_s* cmd);
	}
	namespace Autostrafe
	{
		inline static bool Enabled = true;
		void Run(usercmd_s* cmd);
	}
}