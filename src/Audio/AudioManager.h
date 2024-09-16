#pragma once

#include <SDL.h>
#include <SDL_mixer.h>
#include <iostream>
#include <map>
#include <string>

class AudioManager
{
public:
    static AudioManager& GetInstance();

    bool init();
    bool loadMusic(const std::string& id, const std::string& path);
    bool loadSound(const std::string& id, const std::string& path);
    
    void playMusic(const std::string& id, int loops = -1);
    void playSound(const std::string& id, int loops = 0);
    
    void stopMusic();
    void stopSound(int channel = -1);
    void cleanUp();

private:
    std::map<std::string, Mix_Music*> musicTracks;
    std::map<std::string, Mix_Chunk*> soundEffects;
};
