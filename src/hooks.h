#pragma once
#include "ingame-clock.h"

namespace Hooks {

	void InstallAllHooks();

	struct FrameUpdate {
		static void Install();
	private:
		static std::int32_t FrameUpdateHook(float a_delta);
		static inline REL::Relocation<decltype(FrameUpdateHook)> frameUpdateHook;
	};

	struct Renderer {
		static void Install();
	private:
		static void Present(uint32_t a1);
		static inline REL::Relocation<decltype(Present)> func;
	};

	struct SwapChainHook {
		static void Install();
	private:
		static void RendererInit();
		static void ApplyClockStyle();		
		static inline REL::Relocation<decltype(RendererInit)> func;
		
		static inline bool bInitialized = false;
		static inline bool bStyleApplied = false;
		static inline bool bColorApplied = false;
	};

}

