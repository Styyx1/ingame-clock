# IngameClockAPI C++ Documentation

---

## Overview

The **IngameClockAPI** allows other SKSE plugin mods to interact with the IngameClock mod at runtime.  
It provides functions to show, hide, reposition, recolor, and rescale the clock, as well as manage the toggle hotkey.

Here is an example mod that uses most of the functions: [Chaotic Clock](https://github.com/Styyx1/ChaoticClock/blob/main/src/main.cpp)

## API Initialization

Before using the API, your mod must acquire it at runtime during an initialization phase, such as during the `SKSE::MessagingInterface::kDataLoaded` message.  
**You should store the API pointer globally or persistently for the remainder of the game's runtime.**

---

### Recommended Pattern (one-time lookup and persistent storage)

Declare a global pointer:

```cpp
IngameClockAPI* g_clockAPI = nullptr;
```
and then:
```cpp
typedef IngameClockAPI* (*_RequestIngameClockAPI)();

void RegisterClockAPI()
{
    auto handle = GetModuleHandleW(L"ingame-clock.dll");
    if (!handle) {
        // Clock mod not loaded
        return;
    }

    auto requestAPI = (_RequestIngameClockAPI)GetProcAddress(handle, "RequestIngameClockAPI");
    if (!requestAPI)
        return;

    g_clockAPI = requestAPI();

    if (!g_clockAPI || g_clockAPI->version != INGAMECLOCK_API_VERSION) {
        // Version mismatch or invalid API pointer
        g_clockAPI = nullptr;
    }
}
```
## API Functions
### API Data

```cpp
uint32_t version;
Description: The API version number (currently 1).
Usage: Ensure compatibility by checking api->version == INGAMECLOCK_API_VERSION.
```
### Clock Visibility
```cpp
void (*ShowClock)();
Show the clock overlay.
```

```cpp
void (*HideClock)();
Hide the clock overlay.
```
```cpp
void (*ToggleClock)();
Toggle the clock overlay visibility.
```
```cpp
bool (*IsClockVisible)();
Returns true if the clock overlay is currently visible.
```
### Clock Appearance
```cpp
void (*SetClockPosition)(float x, float y, bool save);
Parameters:
x, y — screen position in pixels.
save — whether to persist this position in the mod’s config.
```

```cpp
void (*SetClockColor)(const char* color, bool save);
Parameters:
color — a hex string in "#RRGGBBAA" format.
save — whether to persist this color in the mod’s config.
```

```cpp
float (*GetClockScale)();
Returns the current scale factor of the clock.
```

```cpp
void (*SetClockScale)(float scale, bool save);
Parameters:
scale — a scale multiplier (e.g. 1.0f = 100%).
save — whether to persist this scale in the mod’s config.
```

### Settings
```cpp
void (*RefreshSettings)();
Re-read the mod’s configuration settings and apply them.
```

### External Control State
```cpp
bool (*HasControlsDisableExternally)();
Returns true if any mod disables the show/hide toggle key of the Ingame Clock mod
```

```cpp
void (*SetControlDisabler)(const char* modName, bool disable);
Parameters:
modName — your plugin’s name (used to track who disabled controls).
disable — true to disable the ability to show/hide the clock with the hotkey, false to re-enable.
//example usage: Hide Clock in combat mod or similar
```

```cpp
const char* (*GetControllingModName)();
Returns the name of the mod currently disabling controls (if any).
Returns an empty string if no external mod has disabled them.
```

### Example Usage
```cpp
api->ShowClock();
api->SetClockPosition(800.0f, 500.0f, false);
api->SetClockColor("#FFA500FF", false);
api->SetClockScale(1.5f, false);
api->SetControlDisabler("MyChaosMod", true);
```
## Version Compatibility
Always check api->version == INGAMECLOCK_API_VERSION before calling any functions to ensure compatibility.

### Exported Symbol
Exported function:

```cpp
extern "C" __declspec(dllexport) IngameClockAPI* RequestIngameClockAPI();
```
DLL Export Name: RequestIngameClockAPI
Type Signature:

```cpp
typedef IngameClockAPI* (*_RequestIngameClockAPI)();
```