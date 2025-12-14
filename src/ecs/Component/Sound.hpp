#pragma once

#include "../IComponent.hpp"
#include <SFML/Audio.hpp>
#include <string>

class Sound : public IComponent
{
private:
    sf::SoundBuffer _buffer;
    sf::Sound _sound;
    float _volume;
    bool _is_looping = false;

public:
    Sound(const std::string& file_path, float volume, bool is_looping);
    ~Sound() override = default;

    // SOUND
    sf::Sound& GetSound();
    void SetBuffer(const std::string& file_path);

    // VOLUME
    float GetVolume() const;
    void SetVolume(float volume);

    // LOOP
    bool GetIsLooping() const;
    void SetIsLooping(bool is_looping);
    void Loop();
    void DontLoop();

    // PLAY / STOP
    void Play();
    void Stop();
};
