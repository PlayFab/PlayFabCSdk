---
page_type: sample
languages:
- cpp
products:
- gdk
urlFragment: "PlayFabGameSaveSample-XboxConsole"
description: "Blank PF Game Saves GDK sample shell with Achievements removed."
---

# PF Game Saves GDK Sample (Blank Shell)

This is a minimal shell prepared for future Game Saves work. All Achievements-related code, projects, and docs have been removed. The app signs in a user, shows the ATG Live HUD, and renders a neutral UI from `Assets/SampleUI.csv` with Log and Results consoles.

Build targets supported: Gaming.Desktop.x64 and Gaming.Xbox.Scarlett.x64 (Debug/Profile/Release). Gaming.Xbox.XboxOne.x64 configs are present but typically unused.

Next steps:
- Add Game Saves flows and UI actions.
- Customize `Assets/SampleUI.csv` and assets in `Assets/`.
- Update `MicrosoftGameConfig.mgc` identity as needed for your sandbox.
