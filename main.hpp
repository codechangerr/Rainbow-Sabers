#pragma once

// Include the modloader header, which allows us to tell the modloader which mod this is, and the version etc.
#include "modloader/shared/modloader.hpp"

// beatsaber-hook is a modding framework that lets us call functions and fetch field values from in the game
// It also allows creating objects, configuration, and importantly, hooking methods to modify their values
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"

#include "GlobalNamespace/MainMenuViewController.hpp"
#include "UnityEngine/UI/Button.hpp"
#include "UnityEngine/GameObject.hpp"
#include "HMUI/CurvedTextMeshPro.hpp"
#include "UnityEngine/Transform.hpp"
// Include the necessary headers
#include "GlobalNamespace/SaberModelController.hpp"
#include "UnityEngine/Color.hpp"
#include <chrono>
#include "GlobalNamespace/SaberType.hpp"
#include "GlobalNamespace/SetSaberGlowColor.hpp"
#include "GlobalNamespace/ColorManager.hpp"
#include "GlobalNamespace/SetSaberGlowColor.hpp"
#include "GlobalNamespace/SetSaberGlowColor.hpp"
#include "GlobalNamespace/SaberManager.hpp"
#include "GlobalNamespace/SaberModelController.hpp"
#include "GlobalNamespace/Saber.hpp"
#include "UnityEngine/Vector3.hpp"

// Define variables to track color fading
std::chrono::high_resolution_clock::time_point colorFadeStartTime;
bool colorFadingEnabled = false;
UnityEngine::Color startColor;
UnityEngine::Color endColor;
float fadeDuration = 5.0f; // Duration of the color fading (in seconds)

void SaberManager_Update(GlobalNamespace::SaberManager* self) {
    // Call the original Update function
    self->Update(); 
}

MAKE_HOOK_MATCH(setSaberTypeHook, &GlobalNamespace::SaberManager::Update, void, GlobalNamespace::SaberManager* self) {
    // Call the hook for SaberManager's Update function
    SaberManager_Update(self);
}


// Function to update saber blade color based on time
void UpdateSaberBladeColor(GlobalNamespace::SaberManager* saberManager, UnityEngine::Color newEndColor) {
    if (colorFadingEnabled) {
        // Calculate the elapsed time since color fading started
        std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> timeElapsed = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - colorFadeStartTime);

        // Check if the color fading is complete
        if (timeElapsed.count() <= fadeDuration) {
            // Calculate the color interpolation factor
            float interpolationFactor = timeElapsed.count() / fadeDuration;

            // Calculate the new color based on the interpolation factor
            UnityEngine::Color newColor = UnityEngine::Color::Lerp(startColor, endColor, interpolationFactor);
        } else {
            // Color fading is complete, disable color fading
            colorFadingEnabled = false;
        }
    }
}

// Define these functions here so that we can easily read configuration and log information from other files
Configuration& getConfig();
Logger& getLogger();