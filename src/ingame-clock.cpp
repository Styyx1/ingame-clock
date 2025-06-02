#include "ingame-clock.h"
#include "settings.h"

void IngameClock::ClockOverlay::Draw()
{
    if (ShouldRefresh()) {
        RefreshSettings();
    }

    const auto settings = Settings::Manager::GetSingleton();
    if (!shouldDraw() ) {
#ifdef DO_DEBUG_LOGGING
        logs::debug("Not Visible, skip drawing entirely");
#endif
        return;
    }  
    InitSettings();

    auto& io = ImGui::GetIO();
    io.ConfigWindowsMoveFromTitleBarOnly = !editor_active;
    // Apply window position based on mode
    if (IsEditorModeActive()) {		
        InitEditor();
    } else {
        io.MouseDrawCursor = false;
        ImGui::SetNextWindowPos(ImVec2(settings->clock_position_x.GetValue(), settings->clock_position_y.GetValue()), ImGuiCond_Always);
    }

    io.FontGlobalScale = scale;

    ImGui::SetNextWindowBgAlpha(settings->clock_alpha.GetValue());

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar;
    if (!editor_active) {
        windowFlags |= ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar;
    } else {
        windowFlags |=  ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize;
    }

    ImGui::Begin("Ingame Clock", nullptr, windowFlags);

    currentWindowPos = ImGui::GetWindowPos();
	settings->clock_position_x.SetValue(currentWindowPos.x);
	settings->clock_position_y.SetValue(currentWindowPos.y);

    DrawClock();

    if (IsEditorModeActive()) {
        DrawEditor();        
    }

    ImGui::End();
}
void IngameClock::ClockOverlay::SetScale(float newScale, bool save)
{
    scale = newScale;
    if (save) {
        auto settings = Settings::Manager::GetSingleton();
        settings->clock_scale.SetValue(scale);
        settings->Save();
    }

}
void IngameClock::ClockOverlay::SetColor(const std::string& color, bool save)
{
	const auto settings = Settings::Manager::GetSingleton();
	textColor = settings->RGBAtoImColor(settings->hexToRGBA(color, settings->clock_alpha.GetValue()));
	settings->clock_text_color.SetValue(settings->ImColorToHex(textColor));
    if (save)
        settings->Save();
}
void IngameClock::ClockOverlay::SetWindowPosition(float x, float y, bool save)
{
    currentWindowPos.x = x; 
    currentWindowPos.y = y;
    const auto settings = Settings::Manager::GetSingleton();
    settings->clock_position_x.SetValue(currentWindowPos.x);
    settings->clock_position_y.SetValue(currentWindowPos.y);
    if (save) {        
        settings->Save();
    }
    
}
void IngameClock::ClockOverlay::SetControlDisabler(const char* a_modName, bool a_enable)
{
    std::lock_guard lock(mutex);
    std::string modName(a_modName);
    if (a_enable) {
        logs::info("{} is disabling controls", modName);
        controlDisablers.insert(modName);
    }        
    else
        controlDisablers.erase(modName);
}
bool IngameClock::ClockOverlay::IsExternallyControlled() const
{
    std::lock_guard lock(mutex);
    return !controlDisablers.empty();
}
std::string IngameClock::ClockOverlay::GetExternallyControllingModName() const
{
    std::lock_guard lock(mutex);
    if (!controlDisablers.empty())
        logs::debug("controling mod name {}", *controlDisablers.begin());
        return *controlDisablers.begin();
    return "";
}
std::string IngameClock::ClockOverlay::GetGameTimeString()
{
    return GetGameTimeText();
}
std::string IngameClock::ClockOverlay::GetRealTimeString()
{
	return GetRealTimeText();
}
void IngameClock::ClockOverlay::DrawEditor()
{
    auto& io = ImGui::GetIO();
    const auto settings = Settings::Manager::GetSingleton();
    
    ImGui::Separator();
    ImGui::Text("Editor Mode Active");
    
    if (ImGui::SliderFloat("Scale", &scale, 0.1f, 10.0f)) {
        settings->clock_scale.SetValue(scale);
        io.FontGlobalScale = scale;
    }

	if (ImGui::Checkbox("Toggle 24h Format", &use24hFormat)) {
		settings->use_24_hour_format.SetValue(use24hFormat);
	}
    ImGui::SameLine();
	if (ImGui::Checkbox("Show Real Time", &show_real_time)) {
		settings->show_real_time.SetValue(show_real_time);
	}
    ImGui::SameLine();
	if (ImGui::Checkbox("Show Game Time", &show_game_time)) {
		settings->show_game_time.SetValue(show_game_time);
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
}

void IngameClock::ClockOverlay::InitSettings()
{
	if (settings_initialised) {
		return;
	}
	settings_initialised = true;
    ManipulateSettingValues();
}

bool IngameClock::ClockOverlay::shouldDraw() const
{
	const auto settings = Settings::Manager::GetSingleton();
	if (editor_active) {
		return true; // Override visibility if editor is active
	}

	bool anyTimeShown = settings->show_real_time.GetValue() || settings->show_game_time.GetValue();
    if (!anyTimeShown) {
		return false; // If neither time is shown, don't draw
    }

    return force_visible;
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
    force_visible = v;
}

bool IngameClock::ClockOverlay::IsVisible() const
{
    return force_visible;
}

void IngameClock::ClockOverlay::SetEditorMode(bool active)
{
	const auto settings = Settings::Manager::GetSingleton();

    if (!active && editor_active) {
        settings->Save();
    }
    editor_active = active;
    
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
#ifdef DO_DEBUG_LOGGING
	logs::debug("Focus lost, clearing input keys and hiding cursor");
#endif
}

void IngameClock::ClockOverlay::FocusRegained()
{
    _lostFocus = false;
    ImGui::GetIO().MouseDrawCursor = true; // Hide cursor when focus is lost
#ifdef DO_DEBUG_LOGGING
    logs::debug("Focus regained — state restored.");
#endif
}

bool IngameClock::ClockOverlay::Uses24HourFormat() const
{
    return use24hFormat;
}

void IngameClock::ClockOverlay::ManipulateSettingValues()
{
    const auto settings = Settings::Manager::GetSingleton();

    currentWindowPos.x = settings->clock_position_x.GetValue();
    currentWindowPos.y = settings->clock_position_y.GetValue();
    alpha = std::clamp(settings->clock_alpha.GetValue(), 0.0f, 1.0f);
    scale = std::clamp(settings->clock_scale.GetValue(), 0.1f, 10.0f);
    use24hFormat = settings->use_24_hour_format.GetValue();
    show_game_time = settings->show_game_time.GetValue();
    show_real_time = settings->show_real_time.GetValue();
    SetColor(settings->clock_text_color.GetValue());   
}

void IngameClock::ClockOverlay::Set24HourFormat(bool activate)
{
	use24hFormat = activate;
}

void IngameClock::ClockOverlay::RefreshSettings() {
    const auto settings = Settings::Manager::GetSingleton();
    settings->Load();

    ManipulateSettingValues();

    should_refresh_settings = false;
}

void IngameClock::ClockOverlay::RequestRefresh()
{
    should_refresh_settings = true;
}

bool IngameClock::ClockOverlay::ShouldRefresh() const
{
    return should_refresh_settings;
}


