#include "hooks.h"
#include "inputHandler.h"
#include "API/IngameClockAPI.h"
#include "papyrus.h"

extern "C" __declspec(dllexport) IngameClockAPI* RequestIngameClockAPI()
{
	static IngameClockAPI g_clockAPI;
	static std::string controllingModNameCache;
	g_clockAPI.version = INGAMECLOCK_API_VERSION;
	g_clockAPI.ShowClock = []() { IngameClock::ClockOverlay::GetSingleton()->SetVisible(true); };
	g_clockAPI.HideClock = []() { IngameClock::ClockOverlay::GetSingleton()->SetVisible(false); };
	g_clockAPI.ToggleClock = []() {IngameClock::ClockOverlay::GetSingleton()->SetVisible(!IngameClock::ClockOverlay::GetSingleton()->IsVisible()); };
	g_clockAPI.RefreshSettings = []() {IngameClock::ClockOverlay::GetSingleton()->RequestRefresh(); };
	g_clockAPI.SetClockPosition = [](float x, float y, bool save) {IngameClock::ClockOverlay::GetSingleton()->SetWindowPosition(x, y, save); };
	g_clockAPI.SetClockColor = [](const char* color, bool save) {IngameClock::ClockOverlay::GetSingleton()->SetColor(color, save); };
    g_clockAPI.HasControlsDisableExternally = []() { return IngameClock::ClockOverlay::GetSingleton()->IsExternallyControlled(); };
	g_clockAPI.SetControlDisabler = [](const char* mod_name, bool disable) {IngameClock::ClockOverlay::GetSingleton()->SetControlDisabler(mod_name, disable); };
	g_clockAPI.GetControllingModName = []() {
		controllingModNameCache = IngameClock::ClockOverlay::GetSingleton()->GetExternallyControllingModName();
		return controllingModNameCache.c_str(); 
		};
	g_clockAPI.IsClockVisible = []() {return IngameClock::ClockOverlay::GetSingleton()->IsVisible(); };
	g_clockAPI.GetClockScale = []() {return IngameClock::ClockOverlay::GetSingleton()->GetScale(); };
	g_clockAPI.SetClockScale = [](float scale, bool save) {IngameClock::ClockOverlay::GetSingleton()->SetScale(scale, save); };
	return &g_clockAPI;
}

void ListenerCallback(SKSE::MessagingInterface::Message* a_message)
{
	if (a_message->type == SKSE::MessagingInterface::kInputLoaded) {
		auto inputManager = InputHandler::InputManager::GetSingleton();
		inputManager->Register();
		inputManager->SetEditorKey();
	}

	if (a_message->type == SKSE::MessagingInterface::kDataLoaded) {
		
		IngameClock::ClockOverlay::GetSingleton();
	}
}

SKSEPluginLoad(const SKSE::LoadInterface* a_skse)
{
	SKSE::Init(a_skse);
	SKSE::AllocTrampoline(8 * 14);
	SKSE::GetMessagingInterface()->RegisterListener(ListenerCallback);
	SKSE::GetPapyrusInterface()->Register(PapyrusFunctions::Bind);
	const auto settings = Settings::Manager::GetSingleton();
	settings->Load();
	Hooks::InstallAllHooks();
	logs::debug("test toml + debug logging");
	return true;
}
