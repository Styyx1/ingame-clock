#include "clock-overlay-menu.h"
#include <ingame-clock.h>

ClockOverlayMenu::ClockOverlayMenu() : RE::IMenu() {}

RE::IMenu* ClockOverlayMenu::Creator()
{
    return new ClockOverlayMenu();
}

RE::UI_MESSAGE_RESULTS ClockOverlayMenu::ProcessMessage(RE::UIMessage& a_message)
{
    switch (*a_message.type) {
    case RE::UI_MESSAGE_TYPE::kShow:
        if (auto overlay = IngameClock::ClockOverlay::GetSingleton()) {
            overlay->SetVisible(true);
        }
        break;

    case RE::UI_MESSAGE_TYPE::kHide:
        if (auto overlay = IngameClock::ClockOverlay::GetSingleton()) {
            overlay->SetVisible(false);
        }
        break;

    default:
        break;
    }
    return IMenu::ProcessMessage(a_message);	
}
