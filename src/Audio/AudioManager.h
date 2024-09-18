#pragma once
#include <SDL_mixer.h>
#include <map>
#include <string>

class AudioManager
{
public:
    static void Init();
    static bool LoadMusic(const std::string& id, const std::string& path);
    static bool LoadSound(const std::string& name);

    static void PlayMusic(const std::string& id, int loops = -1);
    static void PlaySound(const std::string& name, int loops = 0);

    static void StopMusic();
    static void StopSound(int channel = -1);
    static void CleanUp();
private:
    inline static bool initialized = false;
    inline static std::map<std::string, Mix_Music*> musicTracks;
    inline static std::map<std::string, Mix_Chunk*> soundEffects;
};
