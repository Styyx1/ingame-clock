#pragma once

class ClockOverlayMenu : public RE::IMenu
{
public:
    ClockOverlayMenu();
    ~ClockOverlayMenu() override = default; 

    RE::UI_MESSAGE_RESULTS ProcessMessage(RE::UIMessage& a_message) override;

    static RE::IMenu* Creator();
    static constexpr std::string_view MENU_NAME = "IngameClock";
};
