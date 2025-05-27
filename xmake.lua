-- set minimum xmake version
set_xmakever("2.8.2")

-- includes
includes("lib/commonlibsse")

-- set project
set_project("ingame-clock")
set_version("1.0.0")
set_license("GPL-3.0")

-- set defaults
set_languages("c++23")
set_warnings("allextra")

-- add rules
add_rules("mode.debug", "mode.releasedbg")
add_rules("plugin.vsxmake.autoupdate")

-- set policies
set_policy("package.requires_lock", true)

-- set configs
set_config("skyrim_ae", true)

set_config("rex_toml", true)

add_extrafiles("release/**.toml")
-- targets

-- ImGui static lib target
target("imgui")
    set_kind("static")
    add_files("lib/imgui/*.cpp")
    add_includedirs("lib/imgui", {public = true})

target("ingame-clock")    

    add_deps("commonlibsse")
    add_deps("imgui")

    -- add commonlibsse plugin
    add_rules("commonlibsse.plugin", {
        name = "ingame-clock",
        author = "styyx",
        description = "SKSE64 plugin template using CommonLibSSE"
    })

    -- add src files
    add_files("src/**.cpp")
    add_headerfiles("src/**.h")
    add_includedirs("src")
    add_includedirs("lib/imgui")
    set_pcxxheader("src/pch.h")
    add_includedirs("lib/imgui")
    add_includedirs("lib/imgui/backends")
    add_files("lib/imgui/imgui*.cpp")
    add_files("lib/imgui/backends/imgui_impl_dx11.cpp")
    add_files("lib/imgui/backends/imgui_impl_win32.cpp")
    add_headerfiles("src/extern/**.h")

after_build(function(target)
    local copy = function(env, ext)
        for _, env in pairs(env:split(";")) do
            if os.exists(env) then
                local plugins = path.join(env, ext, "SKSE/Plugins")
                os.mkdir(plugins)
                os.trycp(target:targetfile(), plugins)
                os.trycp(target:symbolfile(), plugins)
                -- Copy config files or other extras
                os.trycp("$(projectdir)/release/**", plugins)
            end
        end
    end
    if os.getenv("XSE_TES5_MODS_PATH") then
        copy(os.getenv("XSE_TES5_MODS_PATH"), target:name())
    elseif os.getenv("XSE_TES5_GAME_PATH") then
        copy(os.getenv("XSE_TES5_GAME_PATH"), "Data")
    end    
end)
