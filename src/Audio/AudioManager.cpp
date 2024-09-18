#include "AudioManager.h"

#include <algorithm>
#include <iostream>
#include <SDL.h>

#include "../Logger/Logger.h"

// Convert a normalized volume (0.0 to 1.0) to SDL_mixer's range (0 to 128)
int AudioManager::NormalizeVolume(float volume)
{
    return static_cast<int>(std::clamp(volume * MIX_MAX_VOLUME, 0.0f, static_cast<float>(MIX_MAX_VOLUME)));
}

void AudioManager::Init()
{
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        Logger::Err("Error initializing SDL: " + std::string(SDL_GetError()));
        return;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        Logger::Err("Error initializing SDL_mixer: " + std::string(Mix_GetError()));
        SDL_Quit();
        return;
    }

    // Set default volume levels
    Mix_VolumeMusic(NormalizeVolume(globalMusicVolume));
    Mix_Volume(-1, NormalizeVolume(globalSoundVolume));

    Logger::Log("AudioManager Loaded");
    initialized = true;
}

bool AudioManager::LoadMusic(const std::string& id, const std::string& path)
{
    if (!initialized) Init();

    if (musicTracks.find(id) != musicTracks.end())
    {
        // Music already loaded
        return true;
    }

    Mix_Music* music = Mix_LoadMUS(path.c_str());
    if (!music)
    {
        Logger::Wrn("Error loading music: " + std::string(Mix_GetError()));
        return false;
    }

    musicTracks[id] = music;
    Logger::Log("Music loaded: " + id);
    return true;
}

bool AudioManager::LoadSound(const std::string& name)
{
    if (!initialized) Init();

    if (soundEffects.find(name) != soundEffects.end())
    {
        // Sound already loaded
        return true;
    }

    const std::string path = "assets/audios/" + name;
    Mix_Chunk* sound = Mix_LoadWAV(path.c_str());

    if (!sound)
    {
        Logger::Wrn("Error loading sound: " + std::string(Mix_GetError()));
        return false;
    }

    soundEffects[name] = sound;
    Logger::Log("Sound loaded: " + name);
    return true;
}

void AudioManager::PlayMusic(const std::string& id, std::optional<float> volume, std::optional<int> loops)
{
    // Check if the music is already loaded, if not, attempt to load it
    if (musicTracks.find(id) == musicTracks.end())
    {
        Logger::Wrn("Music not found: " + id + ". Attempting to load it.");

        // You might want to determine a valid path or handle this differently
        std::string path = "assets/music/" + id + ".mp3"; // Example path
        if (!LoadMusic(id, path))
        {
            Logger::Wrn("Failed to load music: " + id);
            return;
        }
    }

    // Play the music
    if (volume.has_value())
    {
        Mix_VolumeMusic(NormalizeVolume(volume.value()));
    }

    int loopCount = loops.value_or(-1); // Default to infinite loop if not specified
    if (Mix_PlayMusic(musicTracks[id], loopCount) == -1)
    {
        Logger::Err("Error playing music: " + std::string(Mix_GetError()));
    }
}

void AudioManager::PlaySound(const std::string& name, std::optional<float> volume, std::optional<int> loops)
{
    // Check if the sound is already loaded, if not, attempt to load it
    if (soundEffects.find(name) == soundEffects.end())
    {
        Logger::Wrn("Sound not found: " + name + ". Attempting to load it.");

        if (!LoadSound(name))
        {
            Logger::Wrn("Failed to load sound: " + name);
            return;
        }
    }

    // Play the sound
    Mix_Chunk* sound = soundEffects[name];
    int actualVolume = NormalizeVolume(volume.value_or(globalSoundVolume));
    Mix_VolumeChunk(sound, actualVolume);

    int loopCount = loops.value_or(0); // Default to no loop if not specified
    if (Mix_PlayChannel(-1, sound, loopCount) == -1)
    {
        Logger::Err("Error playing sound: " + std::string(Mix_GetError()));
    }
}

void AudioManager::StopMusic()
{
    Mix_HaltMusic();
}

void AudioManager::StopSound(int channel)
{
    if (channel == -1)
    {
        Mix_HaltChannel(-1);
    }
    else
    {
        Mix_HaltChannel(channel);
    }
}

void AudioManager::CleanUp()
{
    if (!initialized) return;

    for (auto& music : musicTracks)
    {
        Mix_FreeMusic(music.second);
    }
    musicTracks.clear();

    for (auto& sound : soundEffects)
    {
        Mix_FreeChunk(sound.second);
    }
    soundEffects.clear();

    Mix_CloseAudio();
    SDL_Quit();
    initialized = false;
    Logger::Log("AudioManager cleaned up");
}

void AudioManager::SetMusicVolume(float volume)
{
    globalMusicVolume = std::clamp(volume, 0.0f, 1.0f);
    Mix_VolumeMusic(NormalizeVolume(globalMusicVolume));
}

void AudioManager::SetSoundVolume(float volume)
{
    globalSoundVolume = std::clamp(volume, 0.0f, 1.0f);
    Mix_Volume(-1, NormalizeVolume(globalSoundVolume));
}

float AudioManager::GetMusicVolume()
{
    return globalMusicVolume;
}

float AudioManager::GetGlobalSoundVolume()
{
    return globalSoundVolume;
}
