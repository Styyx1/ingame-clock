#include "hooks.h"
#include "inputHandler.h"
#include "API/IngameClockAPI.h"
#include "papyrus.h"

extern "C" __declspec(dllexport) IngameClockAPI::ClockAPI* IngameClockAPI_GetAPI()
{
	return IngameClockAPI::GetAPI();
}

void ListenerCallback(SKSE::MessagingInterface::Message* a_message)
{
	if (a_message->type == SKSE::MessagingInterface::kDataLoaded) {
		auto inputManager = InputHandler::InputManager::GetSingleton();
		inputManager->Register();
		inputManager->SetEditorKey();
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
