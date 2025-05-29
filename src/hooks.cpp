#include "hooks.h"
#include "settings.h"

using namespace RE;

             // Use ImGui::GetCurrentContext()

namespace Hooks {
    void Hooks::Renderer::Present(uint32_t a1)
    {
        if (ImGui::GetCurrentContext()) {
            ImGui_ImplDX11_NewFrame();
            ImGui_ImplWin32_NewFrame();
            ImGui::NewFrame();

			const auto ingameClock = IngameClock::ClockOverlay::GetSingleton();
            ingameClock->Draw();

            ImGui::Render();
            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        }
        else {
            logs::debug("no imgui context found");
        }
        func(a1);
    }

    void Renderer::Install() {
        const REL::Relocation<uintptr_t> presentHook{ RELOCATION_ID(75461, 77246), OFFSET(0x9, 0x9) };
        auto& trampoline = SKSE::GetTrampoline();
        func = trampoline.write_call<5>(presentHook.address(), Present);
    }
    std::int32_t FrameUpdate::FrameUpdateHook(float a_delta)
    {
        if (frameUpdateCount > 150) {
            frameUpdateCount = 0.0f;

        }
        frameUpdateCount++;
        return frameUpdateHook(a_delta);
    }
    void FrameUpdate::Install() {
        REL::Relocation<std::uintptr_t> main_update{ RELOCATION_ID(35565, 36564), OFFSET(0x748, 0xc26) };
        auto& trampoline = SKSE::GetTrampoline();
        frameUpdateHook = trampoline.write_call<5>(main_update.address(), &FrameUpdateHook);
    }
    void SwapChainHook::RendererInit()
    {
        func();  // call original
        if (!bInitialized) {
			const auto settings = Settings::Manager::GetSingleton();
            auto renderer = RE::BSGraphics::Renderer::GetSingleton();
			const auto ingameClock = IngameClock::ClockOverlay::GetSingleton();
            if (!renderer) {
                logs::error("Renderer not found!");
                return;
            }

            auto swapChain = (IDXGISwapChain*)renderer->data.renderWindows[0].swapChain;
            if (!swapChain) {
                logs::error("SwapChain not found!");
                return;
            }

            DXGI_SWAP_CHAIN_DESC desc{};
            if (FAILED(swapChain->GetDesc(&desc))) {
                logs::error("SwapChain::GetDesc failed");
                return;
            }

            const auto device = reinterpret_cast<ID3D11Device*>(renderer->data.forwarder);
            const auto context = reinterpret_cast<ID3D11DeviceContext*>(renderer->data.context);

            ImGui::CreateContext();

            if (!bStyleApplied) {
                ApplyClockStyle();
                logs::info("applied clock style");
                bStyleApplied = true;
            }

            if (!bColorApplied) {
				logs::debug("Applying clock color");
                ingameClock->textColor = ingameClock->GetClockColor();
                bColorApplied = true;
            }


            auto& io = ImGui::GetIO();
            io.ConfigFlags = ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad | ImGuiConfigFlags_NavEnableSetMousePos;
            io.IniFilename = nullptr;
            io.FontGlobalScale = settings->clock_scale.GetValue();

            std::string fontPath = Settings::Constants::font_file_path + settings->font_name.GetValue();

            ImFont* font = io.Fonts->AddFontFromFileTTF(fontPath.c_str(), settings->font_size.GetValue());
            if (!font) {
                // fallback to default font if loading failed

                ImFontConfig config;
                config.SizePixels = (float)settings->font_size.GetValue();  // or whatever size you want

                font = io.Fonts->AddFontDefault(&config);
                // Optionally log this failure somewhere
                logs::warn("Failed to load font at {}, falling back to default font. ", fontPath);
            }

            if (!ImGui_ImplWin32_Init(desc.OutputWindow)) {
                logs::error("ImGui Win32 init failed");
                return;
            }

            if (!ImGui_ImplDX11_Init(device, context)) {
                logs::error("ImGui DX11 init failed");
                return;
            }

            logs::info("ImGui initialized.");
            bInitialized = true;

            WndProc::func = reinterpret_cast<WNDPROC>(
                SetWindowLongPtrA(
                    desc.OutputWindow,
                    GWLP_WNDPROC,
                    reinterpret_cast<LONG_PTR>(WndProc::thunk)));
            if (!WndProc::func) {
                logs::error("SetWindowLongPtrA failed!");
            }

        }

    }
    void SwapChainHook::ApplyClockStyle()
    {
        ImGuiStyle& style = ImGui::GetStyle();

        // Use dark colors but keep it semi-transparent
        style.WindowRounding = 5.0f;
        style.WindowBorderSize = 1.0f;

        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.3f);  // translucent black bg
        style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 0.9f, 0.7f, 1.0f);       // warm off-white text

        style.Colors[ImGuiCol_Border] = ImVec4(1.0f, 0.9f, 0.7f, 0.0f);

        // No title bar, no resize grip, minimal padding
        style.WindowPadding = ImVec2(8, 6);
        style.FramePadding = ImVec2(4, 2);

        style.ItemSpacing = ImVec2(6, 4);

        style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
    }

    ImFont* SwapChainHook::LoadClockFont(float a_fontSize)
    {
		const auto settings = Settings::Manager::GetSingleton();
        const auto& io = ImGui::GetIO();
        std::string fontPath = Settings::Constants::font_file_path + settings->font_name.GetValue();
        const auto font = io.Fonts->AddFontFromFileTTF(fontPath.c_str(), a_fontSize);

        ImFontConfig icon_config;
        icon_config.MergeMode = true;
        icon_config.PixelSnapH = true;
        icon_config.OversampleH = icon_config.OversampleV = 1;

        io.Fonts->AddFontDefault();

        return font;
    }

    void SwapChainHook::Install() {
        REL::Relocation<std::uintptr_t> target{ RELOCATION_ID(75595, 77226), OFFSET(0x9, 0x275) };  // BSGraphics::InitD3D
        auto& trampoline = SKSE::GetTrampoline();
        func = trampoline.write_call<5>(target.address(), &RendererInit);
    }
    void InstallAllHooks()
    {
        Renderer::Install();
        SwapChainHook::Install();
    }
}