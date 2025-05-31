
# 📖 Ingame Clock API Documentation

## Overview
The **Ingame Clock API** provides two ways to interact with the in-game clock overlay:

- A **native C++ API** for SKSE plugins
- A **Papyrus scripting API** for Skyrim mods

---

## Native C++ API — `IngameClockAPI`

This API allows other SKSE plugins to control the clock overlay.

### Accessing the API

```cpp
#include "IngameClockAPI.h"

if (auto api = IngameClockAPI::GetAPI()) {
    api->ShowClock();
}
```

### 📖 Available Functions

| Function               | Return Type | Description |
|:----------------------|:------------|:------------------------------------------------|
| `ShowClock()`           | `void`       | Forces the clock overlay to become visible |
| `HideClock()`           | `void`       | Forces the clock overlay to hide |
| `ToggleClock()`         | `void`       | Toggles the clock overlay visibility |
| `IsClockVisible()`      | `bool`       | Returns `true` if the clock overlay is currently visible |

### API Versioning

Check the API version to ensure compatibility:

```cpp
int version = IngameClockAPI::GetAPIVersion();
```

- Current version: `1`

### API Declaration

```cpp
namespace IngameClockAPI
{
    constexpr inline int API_VERSION = 1;

    class ClockAPI
    {
    public:
        virtual void ShowClock() {};
        virtual void HideClock() {};
        virtual void ToggleClock() {};
        [[nodiscard]]
        virtual bool IsClockVisible() const { return true; };

    protected:
        virtual ~ClockAPI() = default;
    };

    ClockAPI* GetAPI();
    int GetAPIVersion();
}
```

---

## Papyrus API — `Styyx_IngameClock`

This API allows mod authors to control the clock overlay through Papyrus scripts.

### Example Usage

```papyrus
if Styyx_IngameClock.IsClockVisible()
    Styyx_IngameClock.HideClock()
else
    Styyx_IngameClock.ShowClock()
endif
```

### Available Functions

| Function                     | Return Type | Description |
|:----------------------------|:-------------|:------------------------------------------------|
| `GetVersion()`                | `int`        | Returns API version number |
| `ToggleClock()`               | `None`       | Toggles clock overlay visibility |
| `HideClock()`                 | `None`       | Hides the clock overlay |
| `ShowClock()`                 | `None`       | Shows the clock overlay |
| `IsClockVisible()`            | `bool`       | Returns whether the overlay is visible or not |
| `SetUse24hFormat(bool)`       | `None`       | Enables/disables 24-hour time format |
| `SetShowRealTime(bool)`       | `None`       | Enables/disables real-time clock display |
| `SetShowGameTime(bool)`       | `None`       | Enables/disables game-time clock display |
| `GetScale()`                  | `float`      | Returns the current clock scale |
| `SetScale(float)`             | `None`       | Sets the clock’s scale |
| `SetColor(string)`            | `None`       | Sets the clock color HEX RRGGBBAA format |
| `SetPosition(float, float)`   | `None`       | Sets clock position (X, Y) |
| `GetPositionX()`              | `float`      | Gets current X position |
| `GetPositionY()`              | `float`      | Gets current Y position |
| `GetGameTimeText()`           | `string`     | Returns game-time string |
| `GetRealTimeText()`           | `string`     | Returns real-world time string |

### API Versioning

Use `GetVersion()` to retrieve the current Papyrus API version.

- Current version: `1`

---

## Notes

- **Overlay visibility functions** persist until changed again.
- **SetColor** expects a valid hexadecimal color string (`"#RRGGBB"`).
- **Positions** are relative to screen resolution (0.0 — 1.0 normalized).

---

## Version History

| Version | Changes |
|:---------|:-------------------------------------------------|
| `1`      | Initial release with C++ and Papyrus APIs |
