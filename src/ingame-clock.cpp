#include "ingame-clock.h"
#include "settings.h"

void IngameClock::ClockOverlay::Draw()
{
    if (!visible)
        return;

    const auto settings = Settings::Manager::GetSingleton();
    //if (!settings->show_real_time.GetValue() && !settings->show_game_time.GetValue()) {
    //    if (!editor_active) {
    //        visible = false;
    //    }        
    //    return;
    //}
    InitSettings();

    auto& io = ImGui::GetIO();
    io.ConfigWindowsMoveFromTitleBarOnly = !editor_active;
    // Apply window position based on mode
    if (editor_active) {		
        InitEditor();
    } else {
        io.MouseDrawCursor = false;
        ImGui::SetNextWindowPos(ImVec2(settings->clock_position_x.GetValue(), settings->clock_position_y.GetValue()), ImGuiCond_Always);
    }

    ImGui::SetNextWindowBgAlpha(settings->clock_alpha.GetValue());

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar;
    if (!editor_active) {
        windowFlags |= ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar;
    } else {
        windowFlags |=  ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize;
    }

    ImGui::Begin("Ingame Clock", nullptr, windowFlags);

    currentWindowPos = ImGui::GetWindowPos();
    DrawClock();

    if (editor_active) {
        DrawEditor();        
    }

    ImGui::End();
}
void IngameClock::ClockOverlay::DrawEditor()
{
    auto& io = ImGui::GetIO();
    const auto settings = Settings::Manager::GetSingleton();
    
    ImGui::Separator();
    ImGui::Text("Editor Mode Active");
    
    if (ImGui::SliderFloat("Scale", &scale, 0.5f, 5.0f)) {
        settings->clock_scale.SetValue(scale);
        io.FontGlobalScale = scale; // live update
    }

	if (ImGui::Checkbox("Toggle 24h Format", &use24hFormat)) {
		settings->use_24_hour_format.SetValue(use24hFormat);
        settings->Save();
	}
	if (ImGui::Checkbox("Show Real Time", &show_real_time)) {
		settings->show_real_time.SetValue(show_real_time);
		settings->Save();
	}
	if (ImGui::Checkbox("Show Game Time", &show_game_time)) {
		settings->show_game_time.SetValue(show_game_time);
		settings->Save();
	}

    if (ImGui::ColorEdit4("Text Color", (float*)&textColor)) {
        settings->clock_text_color.SetValue(settings->ImColorToHex(textColor));
    }
}

void IngameClock::ClockOverlay::InitEditor() {
    auto& io = ImGui::GetIO();

    ImGui::SetNextWindowFocus();
    io.MouseDrawCursor = true;
    ImGui::SetNextFrameWantCaptureMouse(true);
    ImGui::SetNextFrameWantCaptureKeyboard(true);
    io.AddMouseButtonEvent(0, (GetKeyState(VK_LBUTTON) & 0x80) != 0);

    // Move window manually if dragging
    if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
        ImVec2 delta = io.MouseDelta;            
        ImVec2 pos = ImGui::GetWindowPos();
        ImGui::SetNextWindowPos(ImVec2(pos.x + delta.x, pos.y + delta.y));
    }
}

void IngameClock::ClockOverlay::DrawClock()
{
    if (show_game_time) {
        ImGui::PushStyleColor(ImGuiCol_Text, textColor.Value);
        ImGui::Text("%s", GetGameTimeText().c_str());
        ImGui::PopStyleColor();
    }
	if (show_real_time) {
		ImGui::PushStyleColor(ImGuiCol_Text, textColor.Value);
		ImGui::Text("%s", GetRealTimeText().c_str());
		ImGui::PopStyleColor();
	}

    /*std::string text;
    SetClockText(text);
    ImGui::PushStyleColor(ImGuiCol_Text, textColor.Value);
    ImGui::Text("%s", text.c_str());
    ImGui::PopStyleColor();*/
}

void IngameClock::ClockOverlay::InitSettings()
{
	if (settings_initialised) {
		return;
	}
	settings_initialised = true;
	const auto settings = Settings::Manager::GetSingleton();
	scale = settings->clock_scale.GetValue();
	use24hFormat = settings->use_24_hour_format.GetValue();
	show_game_time = settings->show_game_time.GetValue();
	show_real_time = settings->show_real_time.GetValue();
	textColor = settings->RGBAtoImColor(settings->hexToRGBA(settings->clock_text_color.GetValue(), settings->clock_alpha.GetValue()));
}

std::string IngameClock::ClockOverlay::GetGameTimeText() const
{
	std::string text;
	auto calendar = RE::Calendar::GetSingleton();
    if (calendar) {
        int hour = static_cast<int>(calendar->GetCurrentGameTime() * 24) % 24;
        int minute = static_cast<int>((calendar->GetCurrentGameTime() * 1440)) % 60;
        std::string day = calendar->GetDayName();
        if (use24hFormat) {
            text = std::format("{}: {:02}:{:02}", day, hour, minute);
        } else {
            std::string ampm = (hour >= 12) ? "PM" : "AM";
            hour = (hour == 0) ? 12 : (hour > 12 ? hour - 12 : hour);
            text = std::format("{}: {:02}:{:02} {}", day, hour, minute, ampm);
        }
    }
    else {
        if (use24hFormat) {
			text = "Invalid Time: 12:34";
        }
        else {
			text = "Invalid Time: 12:34 AM";
        }
    }
    return text;
}

std::string IngameClock::ClockOverlay::GetRealTimeText() const
{
    std::string text;
    const char* week_days[7] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::tm localTime{};
    localtime_s(&localTime, &time);
	std::string day = week_days[localTime.tm_wday];
    if (use24hFormat) {
        text = std::format("{}: {:02}:{:02}", day, localTime.tm_hour, localTime.tm_min);
    } else {
        std::string ampm = (localTime.tm_hour >= 12) ? "PM" : "AM";
        int hour = (localTime.tm_hour == 0) ? 12 : (localTime.tm_hour > 12 ? localTime.tm_hour - 12 : localTime.tm_hour);
        text = std::format("{}: {:02}:{:02} {}", day, hour,  localTime.tm_min, ampm);
    }
    return text;
}

void IngameClock::ClockOverlay::SetVisible(bool v)
{
    visible = v;
}

bool IngameClock::ClockOverlay::IsVisible() const
{
    return visible;
}

void IngameClock::ClockOverlay::SetEditorMode(bool active)
{
	const auto settings = Settings::Manager::GetSingleton();


    if (active && !editor_active) {
        // Entering editor mode — load current settings
        scale = settings->clock_scale.GetValue();
		use24hFormat = settings->use_24_hour_format.GetValue();
		show_game_time = settings->show_game_time.GetValue();
		show_real_time = settings->show_real_time.GetValue();
    }
    else if (!active && editor_active) {
        logs::info("Saving settings on editor exit");
        // Exiting editor mode — save position and values
        settings->clock_position_x.SetValue(currentWindowPos.x);
        settings->clock_position_y.SetValue(currentWindowPos.y);
        settings->clock_scale.SetValue(scale);
    }
    settings->Save();
    editor_active = active;
    RefreshVisibility();
}

bool IngameClock::ClockOverlay::IsEditorModeActive() const
{
    return editor_active;
}

ImColor IngameClock::ClockOverlay::GetClockColor()
{
	const auto settings = Settings::Manager::GetSingleton();
	ImColor new_text_color = settings->RGBAtoImColor(settings->hexToRGBA(settings->clock_text_color.GetValue(), settings->clock_alpha.GetValue()));
    textColor = new_text_color;
    return textColor;
}



void IngameClock::ClockOverlay::FocusLost()
{
	_lostFocus = true;
	ImGui::GetIO().ClearInputKeys();
	ImGui::GetIO().MouseDrawCursor = false; // Hide cursor when focus is lost
    SetEditorMode(false); // Exit editor mode if focus is lost
	logs::info("Focus lost, clearing input keys and hiding cursor");
}

void IngameClock::ClockOverlay::FocusRegained()
{
    _lostFocus = false;
    ImGui::GetIO().MouseDrawCursor = true; // Hide cursor when focus is lost
    logs::info("Focus regained — state restored.");
}

void IngameClock::ClockOverlay::RefreshVisibility()
{
    const auto settings = Settings::Manager::GetSingleton();
    visible = editor_active || settings->show_real_time.GetValue() || settings->show_game_time.GetValue();
}

void IngameClock::ClockOverlay::SetClockText(std::string& input)
{
    auto calendar = RE::Calendar::GetSingleton();
	const auto settings = Settings::Manager::GetSingleton();

    if (show_game_time) {

    }


    if (calendar) {
        int hour = static_cast<int>(calendar->GetCurrentGameTime() * 24) % 24;
        int minute = static_cast<int>((calendar->GetCurrentGameTime() * 1440)) % 60;
        std::string day = calendar->GetDayName();

        if (settings->use_24_hour_format.GetValue()) {
            input = std::format("{}: {:02}:{:02}", day, hour, minute);
        } else {
            std::string ampm = (hour >= 12) ? "PM" : "AM";
            hour = (hour == 0) ? 12 : (hour > 12 ? hour - 12 : hour);
            input = std::format("{}: {:02}:{:02} {}", day, hour, minute, ampm);
        }
    } else {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::tm localTime{};
        localtime_s(&localTime, &time);

        if (settings->use_24_hour_format.GetValue()) {
            input = std::format("Real Time: {:02}:{:02}:{:02}", localTime.tm_hour, localTime.tm_min, localTime.tm_sec);
        } else {
            std::string ampm = (localTime.tm_hour >= 12) ? "PM" : "AM";
            int hour = (localTime.tm_hour == 0) ? 12 : (localTime.tm_hour > 12 ? localTime.tm_hour - 12 : localTime.tm_hour);
            input = std::format("Real Time: {:02}:{:02}:{:02} {}", hour, localTime.tm_min, localTime.tm_sec, ampm);
        }
    }
}


