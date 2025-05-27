#pragma once

namespace Settings {
	namespace Values {

		struct RGBA {
			uint8_t r, g, b, a;
		};

		//bool
		static REX::TOML::Bool enable_debug_log{ "Settings.Debugging", "bEnableDebugLogs", false };
		static REX::TOML::Bool should_show_clock{ "Settings.Toggles", "bShowClock", true };
		//value
		static REX::TOML::F32 clock_position_x{ "Settings.Positions", "fPositionX", 200.0f };
		static REX::TOML::F32 clock_position_y{ "Settings.Positions", "fPositionY", 100.0f };
		static REX::TOML::F32 clock_scale{ "Settings.Scale", "fClockScale", 1.5f };
		static REX::TOML::F32 clock_alpha{ "Settings.Misc", "fClockAlpha", 0.0f }; 
		//string
		static REX::TOML::Str clock_text_color{ "Settings.Misc", "uClockColor", (std::string)"#FFFFFFFF" };

		void Update();

		RGBA hexToRGBA(std::string hex);

	}
	namespace Constants {
		constexpr const char* toml_file_path1 = R"(.\Data\SKSE\Plugins\ingame-clock.toml)";
		constexpr const char* toml_file_path2 = R"(.\Data\SKSE\Plugins\ingame-clock.toml)";
		constexpr const char* png_file_path = R"(.\Data\SKSE\Plugins\ClockPicture\clock-widget.png)";
	}

    
}
