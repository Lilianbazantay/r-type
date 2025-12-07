#pragma once

#include "../IComponent.hpp"

#include <string>

class Sound : public IComponent
{
private:
    std::string _link;
    float _volume;
    bool _is_looping = false;

public:
    Sound(std::string link, float volume, bool is_looping);
    ~Sound();

    // GET / SET
    std::string GetLink();
    void SetLink(std::string link);

    float GetVolume();
    void SetVolume(float volume);

    bool GetIsLooping();
    void SetIsLooping(bool is_looping);
    void Loop();
    void DontLoop();
};
