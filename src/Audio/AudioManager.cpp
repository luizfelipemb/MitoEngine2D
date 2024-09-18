#include "AudioManager.h"

#include <iostream>
#include <SDL.h>

#include "../Logger/Logger.h"

void AudioManager::Init()
{
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        Logger::Err("Error initializing SDL:" + std::string(SDL_GetError()));
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        Logger::Err("Error initializing SDL_mixer:" + std::string(Mix_GetError()));
    }
    Logger::Log("AudioManager Loaded");
    initialized = true;
}

bool AudioManager::LoadMusic(const std::string& id, const std::string& path)
{
    Mix_Music* music = Mix_LoadMUS(path.c_str());

    if (music == nullptr)
    {
        std::cerr << "Error loading sound: " << Mix_GetError() << std::endl;
        return false;
    }
    musicTracks[id] = music;
    return true;
}

bool AudioManager::LoadSound(const std::string& name)
{
    if(!initialized)
    {
        Init();
    }
    const std::string path = "assets/audios/"+name;
    Mix_Chunk* sound = Mix_LoadWAV(path.c_str());
    if (sound == nullptr)
    {
        std::cerr << "Error loading sound: " << Mix_GetError() << '\n';
        return false;
    }
    soundEffects[name] = sound;
    return true;
}

void AudioManager::PlayMusic(const std::string& id, int loops)
{
    if (musicTracks.find(id) != musicTracks.end())
    {
        Mix_PlayMusic(musicTracks[id], loops);
    }
    else
    {
        Logger::Err("Music not found: " + id);
    }
}

void AudioManager::PlaySound(const std::string& name, int loops)
{
    if (soundEffects.find(name) != soundEffects.end())
    {
        Mix_PlayChannel(-1, soundEffects[name], loops);
    }
    else
    {
        Logger::Err("Sound not found: " + name);
    }
}

void AudioManager::StopMusic()
{
    Mix_HaltMusic();
}

void AudioManager::StopSound(int channel)
{
    Mix_HaltChannel(channel);
}

void AudioManager::CleanUp()
{
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
}
