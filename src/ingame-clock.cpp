#include "ingame-clock.h"
#include "RE/Skyrim.h"

void IngameClock::ClockOverlay::Draw()
{
    if (!visible)
        return;

    ImGui::SetNextWindowPos(ImVec2(Settings::Values::clock_position_x.GetValue(), Settings::Values::clock_position_y.GetValue()), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(Settings::Values::clock_alpha.GetValue()); 

    ImGui::Begin("Ingame Clock", nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs |
        ImGuiWindowFlags_NoBackground);

    ImGui::SetWindowFontScale(Settings::Values::clock_scale.GetValue());
    std::string text;
    SetClockText(text);
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(textColor.r, textColor.g, textColor.b, textColor.a));
    ImGui::Text(text.c_str());
    ImGui::PopStyleColor();

    ImGui::End();
}

void IngameClock::ClockOverlay::SetVisible(bool v)
{
    visible = v;
}

bool IngameClock::ClockOverlay::IsVisible()
{
    return visible;
}

Settings::Values::RGBA IngameClock::ClockOverlay::GetClockColor()
{
    textColor = Settings::Values::hexToRGBA(Settings::Values::clock_text_color.GetValue());
    return textColor;
}

inline void IngameClock::ClockOverlay::DrawWithImage()
{

}

void IngameClock::ClockOverlay::SetClockText(std::string &input)
{
    auto calendar = RE::Calendar::GetSingleton();
    
    if (calendar) {
        int hour = static_cast<int>(calendar->GetCurrentGameTime() * 24) % 24;
        int minute = static_cast<int>((calendar->GetCurrentGameTime() * 1440)) % 60;
        std::string day = calendar->GetDayName();
        input = std::format("{}: {:02}:{:02}",day, hour, minute);

    } else {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::tm localTime{};
        localtime_s(&localTime, &time);
        input = std::format("Real Time: {:02}:{:02}:{:02}", localTime.tm_hour, localTime.tm_min, localTime.tm_sec);
    }
}
