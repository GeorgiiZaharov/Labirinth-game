#ifndef WEAPON_HPP
#define WEAPON_HPP

#include "includes.hpp"

#include "Gun.hpp"

class AKR : public Gun
{
public:
    explicit AKR(void) = default;
    explicit AKR(float h, float w, float r);
    static bool loadResources(void);

private:
    static sf::Texture gun_tex, bullet_tex, fire_anim_tex, recharge_anim_tex, presentation_tex;
    static sf::Sound shoot_sound, recharge_sound;
    static sf::SoundBuffer shoot_sound_buffer, recharge_sound_buffer;
};

class M416 : public Gun
{
public:
    explicit M416(void) = default;
    explicit M416(float h, float w, float r);
    static bool loadResources(void);

private:
    static sf::Texture gun_tex, bullet_tex, fire_anim_tex, recharge_anim_tex, presentation_tex;
    static sf::Sound shoot_sound, recharge_sound;
    static sf::SoundBuffer shoot_sound_buffer, recharge_sound_buffer;
};

class Desert_eagle : public Gun
{
public:
    explicit Desert_eagle(void) = default;
    explicit Desert_eagle(float h, float w, float r);
    static bool loadResources(void);

private:
    static sf::Texture gun_tex, bullet_tex, fire_anim_tex, recharge_anim_tex, presentation_tex;
    static sf::Sound shoot_sound, recharge_sound;
    static sf::SoundBuffer shoot_sound_buffer, recharge_sound_buffer;
};

#endif


