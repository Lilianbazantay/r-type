#include "Hitbox.hpp"

#include <algorithm>

/**
 * @brief Construct a new Hitbox object and set its type to HITBOX
 *
 * @param sizeX set _sizeX with a float value
 * @param sizeY set _sizeY with a float value
 */
Hitbox::Hitbox(float sizeX, float sizeY, int damage) : _sizeX(sizeX), _sizeY(sizeY), _damage(damage)
{
    _type = ComponentType::HITBOX;
}

/**
 * @brief Construct a new Hitbox object and set its type to HITBOX
 *
 * @param sizeX set _sizeX with a float value
 * @param sizeY set _sizeY with a float value
 * @param layers set all layers
 * @param masks set all masks
 */
Hitbox::Hitbox(float sizeX, float sizeY, int damage, std::vector<int> layers, std::vector<int> masks) : _sizeX(sizeX), _sizeY(sizeY), _layers(layers), _masks(masks), _damage(damage)
{
    _type = ComponentType::HITBOX;
}

/**
 * @brief Get the Hitbox Size object
 *
 * @return std::pair<float, float> return the size of the Hitbox
 */
std::pair<float, float> Hitbox::GetHitboxSize()
{
    return std::make_pair(_sizeX, _sizeY);
}

/**
 * @brief Set the Hitbox Size object
 *
 * @param sizeX set _sizeX with a float value
 * @param sizeY set _sizeY with a float value
 */
void Hitbox::SetHitboxSize(float sizeX, float sizeY)
{
    _sizeX = sizeX;
    _sizeY = sizeY;
}

/**
 * @brief Set the Hitbox Size object
 *
 * @param size set _sizeY and _sizeX with a pair of float values
 */
void Hitbox::SetHitboxSize(std::pair<float, float> size)
{
    _sizeX = size.first;
    _sizeY = size.second;
}

/**
 * @brief Add Layer to the list
 *
 * @param layer Add a layer to the layers vector
 */
void Hitbox::AddLayer(int layer)
{
    _layers.push_back(layer);
}

/**
 * @brief Sub Layer from the list
 *
 * @param layer remove a layer from the layers vector
 */
void Hitbox::SubLayer(int layer)
{
    _layers.erase(std::remove(_layers.begin(), _layers.end(), layer), _layers.end());
}

/**
 * @brief Get the Layers object
 *
 * @return std::vector<int> return the layers vector
 */
std::vector<int> Hitbox::GetLayers()
{
    return _layers;
}

/**
 * @brief Set the Layers object
 *
 * @param layers set the layers vector
 */
void Hitbox::SetLayers(std::vector<int> layers)
{
    _layers = layers;
}

/**
 * @brief Add Mask to the list
 *
 * @param mask Add a mask to the masks vector
 */
void Hitbox::AddMask(int mask)
{
    _masks.push_back(mask);
}

/**
 * @brief Sub Mask from the list
 *
 * @param mask remove a mask from the masks vector
 */
void Hitbox::SubMask(int mask)
{
    _masks.erase(std::remove(_masks.begin(), _masks.end(), mask), _masks.end());
}

/**
 * @brief Get the Mask object
 *
 * @return std::vector<int> return the masks vector
 */
std::vector<int> Hitbox::GetMask()
{
    return _masks;
}

/**
 * @brief Set the Mask object
 *
 * @param mask set the masks vector
 */
void Hitbox::SetMask(std::vector<int> mask)
{
    _masks = mask;
}

/**
 * @brief Return the damage of the Hitbox
 *
 * @return int Damage to return
 */
int Hitbox::GetDamage()
{
    return _damage;
}

/**
 * @brief Set the damage
 *
 * @param new_damage Set _damage
 */
void Hitbox::SetDamage(int new_damage)
{
    _damage = new_damage;
}

/**
 * @brief Multiply the damage with multiplyer
 *
 * @param multiplyer Multiply the damage
 */
void Hitbox::MultiplyDamage(int multiplyer)
{
    _damage *= multiplyer;
}

/**
 * @brief Divise the damage if the diviser is != 0
 *
 * @param diviser Divise the damage
 */
void Hitbox::DiviseDamage(int diviser)
{
    if (diviser != 0)
        _damage /= diviser;
}
