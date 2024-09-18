#pragma once

#include <SDL_mixer.h>
#include <map>
#include <string>
#include <optional>

class AudioManager
{
public:
    // Initialization and Cleanup
    static void Init();
    static void CleanUp();

    // Load audio resources
    static bool LoadMusic(const std::string& id, const std::string& path);
    static bool LoadSound(const std::string& name);

    // Play and stop audio with optional parameters
    static void PlayMusic(const std::string& id,
        std::optional<float> volume,
        std::optional<int> loops);
    static void PlaySound(const std::string& name,
        std::optional<float> volume,
        std::optional<int> loops);
    static void StopMusic();
    static void StopSound(int channel = -1);

    // Volume control
    static void SetMusicVolume(float volume);  // Volume range: 0.0-1.0
    static void SetSoundVolume(float volume);  // Global volume range: 0.0-1.0
    static float GetMusicVolume();
    static float GetGlobalSoundVolume();  // Global volume getter

private:
    // Flag to check if the audio manager is initialized
    inline static bool initialized = false;

    // Maps for managing loaded music and sound effects
    inline static std::map<std::string, Mix_Music*> musicTracks;
    inline static std::map<std::string, Mix_Chunk*> soundEffects;

    // Volume settings
    inline static float globalMusicVolume = 1.0f; // Default volume (full volume)
    inline static float globalSoundVolume = 1.0f; // Default global volume (full volume)

    // Convert normalized volume to SDL_mixer's volume range
    static int NormalizeVolume(float volume);
};
