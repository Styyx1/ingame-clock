#include "settings.h"

void Settings::Manager::Load() {
	auto toml = REX::TOML::SettingStore::GetSingleton();
	toml->Init(Constants::toml_file_path1.c_str(), Constants::toml_file_path2.c_str());
	logs::info("Loading settings from: {} and {}", Constants::toml_file_path1, Constants::toml_file_path2);
	toml->Load();

	if (enable_debug_log.GetValue()) {
		spdlog::set_level(spdlog::level::debug);
		logs::debug("Debug logging enabled");
	}

	clock_alpha.SetValue(std::clamp(clock_alpha.GetValue(), 0.0f, 1.0f));
	Log();
}

void Settings::Manager::Save() {
	auto toml = REX::TOML::SettingStore::GetSingleton();
	toml->Init(Constants::toml_file_path1.c_str(), Constants::toml_file_path2.c_str());
	logs::info("Saving settings to: {}", Constants::toml_file_path1);
	toml->Save();
}

void Settings::Manager::Log() {
	logs::debug("***************** SETTINGS *****************");
	logs::debug("Use 24-Hour Format: {}", use_24_hour_format.GetValue() ? "true" : "false");
	logs::debug("Position X: {}", clock_position_x.GetValue());
	logs::debug("Position Y: {}", clock_position_y.GetValue());
	logs::debug("Scale: {}", clock_scale.GetValue());
	logs::debug("Alpha: {}", clock_alpha.GetValue());
	logs::debug("Text Color: {}", clock_text_color.GetValue().c_str());
	logs::debug("Font Name: {}", font_name.GetValue().c_str());
	logs::debug("Font Size: {}", font_size.GetValue());
	logs::debug("Editor Toggle Key: {}", editor_toggle_key.GetValue().c_str());
	logs::debug("Clock Toggle Key: {}", clock_toggle_key.GetValue().c_str());
	logs::debug("Show Real Time: {}", show_real_time.GetValue() ? "true" : "false");
	logs::debug("Show Game Time: {}", show_game_time.GetValue() ? "true" : "false");
	logs::debug("Debug Logging Enabled: {}", enable_debug_log.GetValue() ? "true" : "false");

	logs::debug("**********************************************");
}

Settings::RGBA Settings::Manager::hexToRGBA(const std::string& hex, float fallback_alpha) {
	size_t len = hex.size();

	if (!((len == 7 && hex[0] == '#') || (len == 9 && hex[0] == '#') ||
		(len == 8 && hex.substr(0, 2) == "0x") || (len == 10 && hex.substr(0, 2) == "0x"))) {
		logs::error("Invalid hex color format: {}", hex);
		return { 255, 255, 255, 255 };
	}
	std::string hexValue = (hex[0] == '#') ? hex.substr(1) : hex.substr(2);
	unsigned int value;
	std::stringstream ss;
	ss << std::hex << hexValue;
	if (!(ss >> value)) {
		logs::error("Failed to parse hex value");
		return { 255, 255, 255, 255 };
	}

	RGBA rgba;
	if (hexValue.size() == 8) {
		rgba.r = (value >> 24) & 0xFF;
		rgba.g = (value >> 16) & 0xFF;
		rgba.b = (value >> 8) & 0xFF;
		rgba.a = value & 0xFF;
	}
	else if (hexValue.size() == 6) {
		rgba.r = (value >> 16) & 0xFF;
		rgba.g = (value >> 8) & 0xFF;
		rgba.b = value & 0xFF;
		rgba.a = static_cast<uint8_t>(std::clamp(fallback_alpha, 0.0f, 1.0f) * 255);
	}
	else {
		logs::error("Invalid hex color length: {}", hex);
		return { 255, 255, 255, 255 };
	}
	return rgba;
}

ImColor Settings::Manager::RGBAtoImColor(const RGBA& rgba)
{
	return ImColor(rgba.r / 255.0f,	rgba.g / 255.0f, rgba.b / 255.0f, rgba.a / 255.0f);
}

std::string Settings::Manager::ImColorToHex(const ImColor& color)
{
	ImVec4 col = color.Value;
	uint8_t r = static_cast<uint8_t>(col.x * 255.0f);
	uint8_t g = static_cast<uint8_t>(col.y * 255.0f);
	uint8_t b = static_cast<uint8_t>(col.z * 255.0f);
	uint8_t a = static_cast<uint8_t>(col.w * 255.0f);

	std::stringstream ss;
	ss << "#" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(r)
		<< std::setw(2) << std::setfill('0') << static_cast<int>(g)
		<< std::setw(2) << std::setfill('0') << static_cast<int>(b)
		<< std::setw(2) << std::setfill('0') << static_cast<int>(a);

	return ss.str();
}
