#ifndef AKR_HPP
#define AKR_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <string>
#include <vector>
#include <iostream>
#include <cmath>
#include <random>
#include "Gun.hpp"

class AKR : public Gun
{
public:
    explicit AKR(void) = default;
    explicit AKR(float h, float w, float r);
    static bool loadResources(void);

private:
    static sf::Texture gun_tex, bullet_tex, fire_anim_tex, recharge_anim_tex;
    static sf::Sound shoot_sound, recharge_sound;
    static sf::SoundBuffer shoot_sound_buffer, recharge_sound_buffer;
};
#endif // AKR_HPP
