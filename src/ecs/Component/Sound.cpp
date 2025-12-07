#include "Sound.hpp"

/**
 * @brief Construct a new Sound:: Sound object
 *
 * @param link Set _link
 * @param volume Set _volume
 * @param is_looping Set _is_looping
 */
Sound::Sound(std::string link, float volume, bool is_looping) : _link(link), _volume(volume), _is_looping(is_looping)
{
    _type = ComponentType::SOUND;
}

/**
 * @brief Get the sound link
 *
 * @return std::string return the sound link
 */
std::string Sound::GetLink()
{
    return _link;
}

/**
 * @brief Replace _link by a new link
 *
 * @param link new link
 */
void Sound::SetLink(std::string link)
{
    _link = link;
}

/**
 * @brief Return the _volume
 *
 * @return float volume
 */
float Sound::GetVolume()
{
    return _volume;
}

/**
 * @brief Set the _volume
 *
 * @param volume new volume
 */
void Sound::SetVolume(float volume)
{
    _volume = volume;
}

/**
 * @brief Return if the sound is looping
 *
 * @return true the sound is looping
 * @return false the sound is not looping
 */
bool Sound::GetIsLooping()
{
    return _is_looping;
}

/**
 * @brief Set if the sound is looping
 *
 * @param is_looping new value
 */
void Sound::SetIsLooping(bool is_looping)
{
    _is_looping = is_looping;
}

/**
 * @brief Make the sound loop
 */
void Sound::Loop()
{
    _is_looping = true;
}

/**
 * @brief Make the sound dont loop
 */
void Sound::DontLoop()
{
    _is_looping = false;
}

