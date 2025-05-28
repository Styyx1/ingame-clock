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
    textColor = Settings::Values::hexToRGBA(Settings::Values::clock_text_color.GetValue(), Settings::Values::clock_alpha.GetValue());
    return textColor;
}

void IngameClock::ClockOverlay::SetClockText(std::string &input)
{
    auto calendar = RE::Calendar::GetSingleton();
    
    if (calendar) {
        int hour = static_cast<int>(calendar->GetCurrentGameTime() * 24) % 24;
        int minute = static_cast<int>((calendar->GetCurrentGameTime() * 1440)) % 60;
        std::string day = calendar->GetDayName();
        if (Settings::Values::use_24_hour_format.GetValue()) {
            input = std::format("{}: {:02}:{:02}",day, hour, minute);
        }
        else {
			if (hour >= 12) {
				hour -= 12;
				if (hour == 0) hour = 12; // Handle midnight case
				input = std::format("{}: {:02}:{:02} PM", day, hour, minute);
			}
            else {
                if (hour == 0) hour = 12; // Handle noon case
                input = std::format("{}: {:02}:{:02} AM", day, hour, minute);
            }
        }
    } 
    else {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::tm localTime{};
        localtime_s(&localTime, &time);
		if (Settings::Values::use_24_hour_format.GetValue())
			input = std::format("Real Time: {:02}:{:02}:{:02}", localTime.tm_hour, localTime.tm_min, localTime.tm_sec);
        else {			
            if (localTime.tm_hour >= 12) {
                int hour = localTime.tm_hour - 12;
                if (hour == 0) hour = 12; // Handle midnight case
                input = std::format("Real Time: {:02}:{:02}:{:02} PM", hour, localTime.tm_min, localTime.tm_sec);
            }
			else {
				if (localTime.tm_hour == 0) localTime.tm_hour = 12; // Handle noon case
				input = std::format("Real Time: {:02}:{:02}:{:02} AM", localTime.tm_hour, localTime.tm_min, localTime.tm_sec);
			}
        }
    }
}
