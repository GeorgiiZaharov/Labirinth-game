#include "Weapon.hpp"

AKR::AKR(float h, float w, float r)
    :Gun(gun_tex, bullet_tex, h, w, h / 2, h / 2, 715, 100, 2000, 30, 3, r, 30, 25)
{
    Gun::set_animation(Animation(fire_anim_tex, shoot_sound, h, w, 2, 100, true), Animation(recharge_anim_tex, recharge_sound, h, w, 1, 2000, false));
}

bool AKR::loadResources(void)
{
    bool success = true;

    if (!gun_tex.loadFromFile("AKR.png"))
        success = false;

    if (!bullet_tex.loadFromFile("bullet.png"))
        success = false;

    if (!fire_anim_tex.loadFromFile("gun_anim.png"))
        success = false;

    if (!recharge_anim_tex.loadFromFile("recharge_anim.png"))
        success = false;

    if (!shoot_sound_buffer.loadFromFile("shootSound.wav"))
        success = false;

    if (!recharge_sound_buffer.loadFromFile("rechargeSound.wav"))
        success = false;

    shoot_sound.setBuffer(shoot_sound_buffer);
    recharge_sound.setBuffer(recharge_sound_buffer);
    return success;
}
sf::Texture AKR::gun_tex, AKR::bullet_tex, AKR::fire_anim_tex, AKR::recharge_anim_tex;
sf::Sound AKR::shoot_sound, AKR::recharge_sound;
sf::SoundBuffer AKR::shoot_sound_buffer, AKR::recharge_sound_buffer;
