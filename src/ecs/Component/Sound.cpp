#include "Sound.hpp"
#include <stdexcept>

/**
 * @brief Construct a new Sound object
 *
 * @param file_path Path to the sound file
 * @param volume Initial volume (0-100)
 * @param is_looping Looping flag
 */
Sound::Sound(const std::string& file_path, float volume, bool is_looping)
    : _volume(volume), _is_looping(is_looping)
{
    if (!_buffer.loadFromFile(file_path))
        throw std::runtime_error("Failed to load sound: " + file_path);

    _sound.setBuffer(_buffer);
    _sound.setVolume(_volume);
    _sound.setLoop(_is_looping);

    _type = ComponentType::SOUND;
}

/**
 * @brief Get the SFML sound
 *
 * @return sf::Sound& Reference to the sound
 */
sf::Sound& Sound::GetSound()
{
    return _sound;
}

/**
 * @brief Set a new sound buffer from file
 *
 * @param file_path Path to the new sound file
 */
void Sound::SetBuffer(const std::string& file_path)
{
    if (!_buffer.loadFromFile(file_path))
        throw std::runtime_error("Failed to load sound: " + file_path);

    _sound.setBuffer(_buffer);
}

/**
 * @brief Return the current volume
 *
 * @return float Volume (0-100)
 */
float Sound::GetVolume() const
{
    return _volume;
}

/**
 * @brief Set the volume
 *
 * @param volume New volume (0-100)
 */
void Sound::SetVolume(float volume)
{
    _volume = volume;
    _sound.setVolume(_volume);
}

/**
 * @brief Return if the sound is looping
 *
 * @return true if looping
 * @return false if not looping
 */
bool Sound::GetIsLooping() const
{
    return _is_looping;
}

/**
 * @brief Set the looping state
 *
 * @param is_looping New looping flag
 */
void Sound::SetIsLooping(bool is_looping)
{
    _is_looping = is_looping;
    _sound.setLoop(_is_looping);
}

/**
 * @brief Enable looping
 */
void Sound::Loop()
{
    _is_looping = true;
    _sound.setLoop(true);
}

/**
 * @brief Disable looping
 */
void Sound::DontLoop()
{
    _is_looping = false;
    _sound.setLoop(false);
}

/**
 * @brief Play the sound
 */
void Sound::Play()
{
    _sound.play();
}

/**
 * @brief Stop the sound
 */
void Sound::Stop()
{
    _sound.stop();
}
