#include "hooks.h"
#include "settings.h"

void ListenerCallback(SKSE::MessagingInterface::Message* a_message)
{
	if (a_message->type == SKSE::MessagingInterface::kDataLoaded) {

	}
}


SKSEPluginLoad(const SKSE::LoadInterface* a_skse)
{
	SKSE::Init(a_skse);
	SKSE::AllocTrampoline(4 * 14);
	SKSE::GetMessagingInterface()->RegisterListener(ListenerCallback);
	Settings::Values::Update();
	Hooks::InstallAllHooks();
	logs::debug("test toml + debug logging");
	return true;
}
