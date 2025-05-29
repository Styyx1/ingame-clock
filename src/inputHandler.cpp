#include "inputHandler.h"

void InputHandler::InputManager::Register()
{
    if (const auto scripts = RE::BSInputDeviceManager::GetSingleton()) {
        scripts->AddEventSink<RE::InputEvent*>(GetSingleton());
        logs::info("Registered for {}", typeid(RE::InputEvent).name());
    }
}

void InputHandler::InputManager::SetEditorKey()
{
	const auto settings = Settings::Manager::GetSingleton();
    if(!activate_editor_key.SetPattern(settings->editor_toggle_key.GetValue()))
        logs::error("Failed to set {} for editor acivation key", settings->editor_toggle_key.GetValue());
}

void InputHandler::InputManager::ToggleEditMode(const hotkeys::KeyCombination* key)
{
	const auto ingameClock = IngameClock::ClockOverlay::GetSingleton();
    ingameClock->SetEditorMode(!ingameClock->IsEditorModeActive());
}

RE::BSEventNotifyControl InputHandler::InputManager::ProcessEvent(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>* a_eventSource)
{
    using EventType = RE::INPUT_EVENT_TYPE;
    using Result = RE::BSEventNotifyControl;

    if (!a_event)
        return Result::kContinue;

    activate_editor_key.Process(a_event);

    return Result::kContinue;
}
