# Papyrus Functions:

## Overview:
Provides native functions to change the Ingame Clock's appearance/positions
Note: All changes you make with it will be saved to the mod's .toml file so they persist. this is completely separate from the game's saving, so if you change something and load a save, the change will be there regardless. This is a limitation by design. 

## Functions:
```papyrus
int Function GetVersion() Global native
```
returns the version of the mod's papyrus functions. Currently that's 1
```papyrus
Function ToggleClock() Global native
Function HideClock() Global native
Function ShowClock() Global native
bool Function IsClockVisible() Global native
```
for clock visibility
```papyrus
Function SetUse24hFormat(bool abUse24hFormat) Global native
```
changes whether to use 24h or 12h time format
```papyrus
Function SetShowRealTime(bool abShowRealTime) Global native
Function SetShowGameTime(bool abShowGameTime) Global native
```
shows/hides real/game time

```papyrus
float Function GetScale() Global native
Function SetScale(float afScale) Global native
```
Scales the clock. Scale can be between 0.5 and 10.0
```papyrus
Function SetColor(string asColor) Global native
```
Set the color of the clock. format needs to be #RRGGBBAA

```papyrus
Function SetPosition(float afX, float afY) Global native
float Function GetPositionX() Global native
float Function GetPositionY() Global native
```
change/get the position of the clock. X = 0 is on the left, Y = 0 is on the top

```papyurs
string Function GetGameTimeText() Global native
string Function GetRealTimeText() Global native
```
returns the time the clock shows in case you need it for a notification or whatever

## Example Usage:
```papyrus
import Styyx_IngameClock

Actor Property PlayerRef Auto

Event OnStoryChangeLocation(ObjectReference akActor, Location akOldLocation, Location akNewLocation)
    If (akNewLocation && PlayerRef.IsInInterior())
        if IsClockVisible()
            SetColor("#00BFFFEE")
            SetPosition(820, 100)
            SetScale(1.2)
            debug.Notification("it is " + GetRealTimeText())
            If (PlayerRef.IsInCombat())
                HideClock()
            else
                ShowClock()
                ; code
            EndIf
        endif
    ElseIf (!PlayerRef.IsInInterior())
        If (IsClockVisible())
            SetColor("#FFA500FF")
            SetPosition(1150, 10)
            SetScale(1.3)
            debug.Notification("it is " + GetGameTimeText() + " in the game")
        EndIf
    EndIf
    parent.Reset() 
EndEvent
```
this changes the color whether the player goes into an interior or exterior location and hides the clock when in combat while changing locations. It also prints the Clock text onto the screen as notification