#include "settings.h"

namespace Settings {
	void Values::Update() {
        logs::info("*****************SETTINGS*****************");
        logs::info("Loading settings...");
        const auto toml = REX::TOML::SettingStore::GetSingleton();
        toml->Init(Constants::toml_file_path1, Constants::toml_file_path2);
        toml->Load();

        if (enable_debug_log.GetValue())
        {
            spdlog::set_level(spdlog::level::debug);
            logs::debug("Debug logging enabled");
        }
        logs::info("...Settings loaded");

        logs::debug("rgba value is: {}", clock_text_color.GetValue());

    }
    Values::RGBA Values::hexToRGBA(std::string hex){
        
        if (hex.size() != 9 || hex[0] != '#') {
            logs::error("invalid hex format");
            RGBA rgba{0xFF, 0xFF, 0xFF, 0xFF};
            return rgba;
        }

        unsigned int value;
        std::stringstream ss;
        ss << std::hex << hex.substr(1);

        if (!(ss >> value)) {
            logs::error("Failed to parse hex value");
            RGBA rgba{0xFF, 0xFF, 0xFF, 0xFF};
            return rgba;
        }

        RGBA rgba;
        rgba.r = (value >> 24) & 0xFF;
        rgba.g = (value >> 16) & 0xFF;
        rgba.b = (value >> 8) & 0xFF;
        rgba.a = (value) & 0xFF;

        return rgba;

    }
}