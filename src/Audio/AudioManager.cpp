#include "AudioManager.h"

#include "../Logger/Logger.h"

AudioManager& AudioManager::GetInstance()
{
    static AudioManager instance;
    return instance;
}

bool AudioManager::init()
{
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        Logger::Err("Error initializing SDL:" + std::string(SDL_GetError()));

        return false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        Logger::Err("Error initializing SDL_mixer:" + std::string(Mix_GetError()));
        return false;
    }
    return true;
}

bool AudioManager::loadMusic(const std::string& id, const std::string& path)
{
    Mix_Music* music = Mix_LoadMUS(path.c_str());

    if (music == nullptr)
    {
        Logger::Err("Error loading music:: " + std::string(Mix_GetError()));

        return false; 
    }
    musicTracks[id] = music;
    return true;
}

bool AudioManager::loadSound(const std::string& id, const std::string& path)
{
    Mix_Chunk* sound = Mix_LoadWAV(path.c_str());
    if (sound == nullptr)
    {
        std::cerr << "Error loading sound: " << Mix_GetError() << std::endl;
        return false; 
    }
    soundEffects[id] = sound;
    return true;
}

void AudioManager::playMusic(const std::string& id, int loops)
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

void AudioManager::playSound(const std::string& id, int loops)
{
    if (soundEffects.find(id) != soundEffects.end())
    {
        Mix_PlayChannel(-1, soundEffects[id], loops);
    }  
    else 
    {
        Logger::Err("Sound not found: " + id);
    }
}

void AudioManager::stopMusic()
{
    Mix_HaltMusic();
}

void AudioManager::stopSound(int channel)
{
    Mix_HaltChannel(channel);
}

void AudioManager::cleanUp()
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
