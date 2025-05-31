#pragma once
#include "ingame-clock.h"
#include "inputHandler.h"

// huge thanks to po3 (photo mode: https://github.com/powerof3/PhotoMode) and Ersh (open animation replacer: https://github.com/ersh1/OpenAnimationReplacer).
// for the hooks i found in their repos

namespace Hooks {

	void InstallAllHooks();

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
		static ImFont* LoadClockFont(float a_fontSize);
		static inline REL::Relocation<decltype(RendererInit)> func;		
		static inline bool bInitialized = false;
		static inline bool bStyleApplied = false;
		static inline bool bColorApplied = false;		
	};
	struct WndProc
	{
		static LRESULT thunk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			const auto ingameClock = IngameClock::ClockOverlay::GetSingleton();
			if (ingameClock->IsEditorModeActive()) {
				ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);				
				return true;				

			}
			if (uMsg == WM_SETFOCUS) {
				ingameClock->FocusRegained();
				const auto inputHandler = InputHandler::InputManager::GetSingleton();
				inputHandler->SetEditorKey();
			}

			if (uMsg == WM_KILLFOCUS) {
				ingameClock->SetEditorMode(false);
				ingameClock->FocusLost();
			}

			return func(hWnd, uMsg, wParam, lParam);
		}
		static inline WNDPROC func;
	};
}