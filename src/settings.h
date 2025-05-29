#pragma once

namespace Settings {

	struct RGBA {
		uint8_t r, g, b, a;
	};

	class Manager : public REX::Singleton<Manager> {
	public:
		// Settings values
		inline static REX::TOML::Bool enable_debug_log{ "Settings.Debugging", "bEnableDebugLogs", false };
		inline static REX::TOML::Bool should_show_clock{ "Settings.Toggles", "bShowClock", true };
		inline static REX::TOML::Bool use_24_hour_format{ "Settings.Toggles", "bUse24HourFormat", true };
		inline static REX::TOML::Bool show_real_time{ "Settings.Toggles", "bShowRealTime", false };
		inline static REX::TOML::Bool show_game_time{ "Settings.Toggles", "bShowGameTime", true };

		inline static REX::TOML::F32 clock_position_x{ "Settings.Positions", "fPositionX", 200.0f };
		inline static REX::TOML::F32 clock_position_y{ "Settings.Positions", "fPositionY", 100.0f };
		inline static REX::TOML::F32 clock_scale{ "Settings.Scale", "fClockScale", 1.5f };
		inline static REX::TOML::F32 clock_alpha{ "Settings.Misc", "fClockAlpha", 0.0f };

		inline static REX::TOML::Str clock_text_color{ "Settings.Misc", "uClockColor", (std::string)"#FFFFFFFF" };
		inline static REX::TOML::Str font_name{ "Settings.Fonts", "sFont", (std::string)"Jost-Regular.ttf" };
		inline static REX::TOML::I32 font_size{ "Settings.Fonts", "iFontSize", 27 };

		inline static REX::TOML::Str editor_toggle_key{ "Settings.Hotkeys", "sEditorToggleKey", (std::string)"F11" };

		// Functions
		void Load();
		void Save();
		void Log();
		RGBA hexToRGBA(const std::string& hex, float fallback_alpha);
		ImColor RGBAtoImColor(const RGBA& rgba);
		std::string ImColorToHex(const ImColor& color);
	};

	namespace Constants {
		inline const std::string toml_file_path1 = "Data/SKSE/Plugins/ingame-clock.toml";
		inline const std::string toml_file_path2 = "Data/SKSE/Plugins/ingame-clock_Custom.toml";
		constexpr const char* png_file_path = R"(.\Data\SKSE\Plugins\ClockPicture\clock-widget.png)";
		constexpr const char* font_file_path = R"(.\Data\Interface\ImGuiIcons\Fonts\)";
	}

}  // namespace Settings
