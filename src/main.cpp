#include "hooks.h"
#include "inputHandler.h"



void ListenerCallback(SKSE::MessagingInterface::Message* a_message)
{
	if (a_message->type == SKSE::MessagingInterface::kDataLoaded) {
		auto inputManager = InputHandler::InputManager::GetSingleton();
		inputManager->Register();
		inputManager->SetEditorKey();
	}
}

SKSEPluginLoad(const SKSE::LoadInterface* a_skse)
{
	SKSE::Init(a_skse);
	SKSE::AllocTrampoline(8 * 14);
	SKSE::GetMessagingInterface()->RegisterListener(ListenerCallback);
	const auto settings = Settings::Manager::GetSingleton();
	settings->Load();
	Hooks::InstallAllHooks();
	logs::debug("test toml + debug logging");
	return true;
}
