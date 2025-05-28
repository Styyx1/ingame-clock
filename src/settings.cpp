#include "settings.h"

namespace Settings {
	void Values::Update() {
        logs::info("*****************SETTINGS*****************");
        logs::info("Loading settings from: {} and {}", Constants::toml_file_path1, Constants::toml_file_path2);
        const auto toml = REX::TOML::SettingStore::GetSingleton();
        toml->Init(Constants::toml_file_path1, Constants::toml_file_path2);
        toml->Load();

        if (enable_debug_log.GetValue())
        {
            spdlog::set_level(spdlog::level::debug);
            logs::debug("Debug logging enabled");
        }
        logs::info("...Settings loaded");
        clock_alpha.SetValue(std::clamp(clock_alpha.GetValue(), 0.0f, 1.0f)); // Ensure alpha is between 0 and 1

        LogSettings();
    }
    void Values::LogSettings()
    {
        logs::debug("***************** SETTINGS *****************");

        logs::debug("Setting Debug Log: {}", enable_debug_log.GetValue() ? "true" : "false");
        logs::debug("Show Clock: {}", should_show_clock.GetValue() ? "true" : "false");
        logs::debug("Use 24-Hour Format: {}", use_24_hour_format.GetValue() ? "true" : "false");

        logs::debug("Clock Position X: {}", clock_position_x.GetValue());
        logs::debug("Clock Position Y: {}", clock_position_y.GetValue());

        logs::debug("Clock Scale: {}", clock_scale.GetValue());
        logs::debug("Clock Alpha: {}", clock_alpha.GetValue());

        logs::debug("Clock Text Color: {}", clock_text_color.GetValue());

		logs::debug("Font Name: {}", font_name.GetValue());
		logs::debug("Font Size: {}", font_size.GetValue());


        logs::debug("**********************************************");
    }
    Values::RGBA Values::hexToRGBA(std::string hex, float fallback_alpha){
        
        size_t len = hex.size();

        // Check for valid length and prefix
        if (!((len == 7 && hex[0] == '#') ||
            (len == 9 && hex[0] == '#') ||
            (len == 8 && hex.substr(0, 2) == "0x") ||
            (len == 10 && hex.substr(0, 2) == "0x"))) {
			logs::error("Invalid hex color format: {}", hex);
            return {255, 255, 255, 255};
        }

        std::string hexValue = (hex[0] == '#') ? hex.substr(1) : hex.substr(2);

        unsigned int value;
        std::stringstream ss;
        ss << std::hex << hexValue;

        if (!(ss >> value)) {
            logs::error("Failed to parse hex value");
            return {0xFF, 0xFF, 0xFF, 0xFF};
        }

        RGBA rgba;
        if (hexValue.size() == 8) {
            // RRGGBBAA format
            rgba.r = (value >> 24) & 0xFF;
            rgba.g = (value >> 16) & 0xFF;
            rgba.b = (value >> 8) & 0xFF;
            rgba.a = value & 0xFF;
        } else if (hexValue.size() == 6) {
            // RRGGBB format
            rgba.r = (value >> 16) & 0xFF;
            rgba.g = (value >> 8) & 0xFF;
            rgba.b = value & 0xFF;
            rgba.a = static_cast<uint8_t>(fallback_alpha * 255);
        } else {
            // Invalid length after prefix removal
			logs::error("Invalid hex color length: {}", hex);
            return {255, 255, 255, 255};
        }

        return rgba;

    }
}