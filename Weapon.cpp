#include "Weapon.hpp"

AKR::AKR(float h, float w, float r)
    :Gun(gun_tex, bullet_tex, h, w, h / 4, h / 4, 715, 100, 2000, 30, 3, r, 30, 25)
{
    Gun::set_animation(Animation(fire_anim_tex, shoot_sound, h, w, 2, 100, true), 
                        Animation(recharge_anim_tex, recharge_sound, h, w, 1, 2000, false),
                        Animation(presentation_tex, 125, 250, 2, 500, false));
}

bool AKR::loadResources(void)
{
    bool success = true;

    if (!gun_tex.loadFromFile("src/AKR.png"))
        success = false;

    if (!bullet_tex.loadFromFile("src/bullet.png"))
        success = false;

    if (!fire_anim_tex.loadFromFile("src/AKR_shoot.png"))
        success = false;

    if (!recharge_anim_tex.loadFromFile("src/AKR_recharge.png"))
        success = false;

    if (!shoot_sound_buffer.loadFromFile("src/AKR_shoot_sound.wav"))
        success = false;

    if (!recharge_sound_buffer.loadFromFile("src/AKR_recharge_sound.wav"))
        success = false;

    if (!presentation_tex.loadFromFile("src/AKR_presentation.png"))
        success = false;

    shoot_sound.setBuffer(shoot_sound_buffer);
    recharge_sound.setBuffer(recharge_sound_buffer);
    return success;
}
sf::Texture AKR::gun_tex, AKR::bullet_tex, AKR::fire_anim_tex, AKR::recharge_anim_tex, AKR::presentation_tex;
sf::Sound AKR::shoot_sound, AKR::recharge_sound;
sf::SoundBuffer AKR::shoot_sound_buffer, AKR::recharge_sound_buffer;




M416::M416(float h, float w, float r)
    :Gun(gun_tex, bullet_tex, h, w, h / 4, h / 4, 715, 75, 2000, 30, 3, r, 30, 20)
{
    Gun::set_animation(Animation(fire_anim_tex, shoot_sound, h, w, 2, 75, true), 
                        Animation(recharge_anim_tex, recharge_sound, h, w, 1, 2000, false),
                        Animation(presentation_tex, 125, 250, 2, 500, false));
}

bool M416::loadResources(void)
{
    bool success = true;

    if (!gun_tex.loadFromFile("src/M416.png"))
        success = false;

    if (!bullet_tex.loadFromFile("src/bullet.png"))
        success = false;

    if (!fire_anim_tex.loadFromFile("src/M416_shoot_anim.png"))
        success = false;

    if (!recharge_anim_tex.loadFromFile("src/M416_recharge.png"))
        success = false;

    if (!shoot_sound_buffer.loadFromFile("src/M416_shoot_sound.wav"))
        success = false;

    if (!recharge_sound_buffer.loadFromFile("src/M416_recharge_sound.wav"))
        success = false;

    if (!presentation_tex.loadFromFile("src/M416_presentation.png"))
        success = false;

    shoot_sound.setBuffer(shoot_sound_buffer);
    recharge_sound.setBuffer(recharge_sound_buffer);
    return success;
}
sf::Texture M416::gun_tex, M416::bullet_tex, M416::fire_anim_tex, M416::recharge_anim_tex, M416::presentation_tex;
sf::Sound M416::shoot_sound, M416::recharge_sound;
sf::SoundBuffer M416::shoot_sound_buffer, M416::recharge_sound_buffer;


Desert_eagle::Desert_eagle(float h, float w, float r)
    :Gun(gun_tex, bullet_tex, h, w, h / 4, h / 4, 715, 400, 1000, 7, 3, r, 45, 50)
{
    Gun::set_animation(Animation(fire_anim_tex, shoot_sound, h, w, 2, 75, true), 
                        Animation(recharge_anim_tex, recharge_sound, h, w, 1, 2000, false),
                        Animation(presentation_tex, 125, 250, 2, 500, false));
}

bool Desert_eagle::loadResources(void)
{
    bool success = true;

    if (!gun_tex.loadFromFile("src/desert_eagle.png"))
        success = false;

    if (!bullet_tex.loadFromFile("src/bullet.png"))
        success = false;

    if (!fire_anim_tex.loadFromFile("src/desert_eagle_shoot.png"))
        success = false;

    if (!recharge_anim_tex.loadFromFile("src/desert_eagle_recharge.png"))
        success = false;

    if (!shoot_sound_buffer.loadFromFile("src/desert_eagle_shoot_sound.wav"))
        success = false;

    if (!recharge_sound_buffer.loadFromFile("src/desert_eagle_recharge_sound.wav"))
        success = false;

    if (!presentation_tex.loadFromFile("src/desert_eagle_presentation.png"))
        success = false;

    shoot_sound.setBuffer(shoot_sound_buffer);
    recharge_sound.setBuffer(recharge_sound_buffer);
    return success;
}
sf::Texture Desert_eagle::gun_tex, Desert_eagle::bullet_tex, Desert_eagle::fire_anim_tex, Desert_eagle::recharge_anim_tex, Desert_eagle::presentation_tex;
sf::Sound Desert_eagle::shoot_sound, Desert_eagle::recharge_sound;
sf::SoundBuffer Desert_eagle::shoot_sound_buffer, Desert_eagle::recharge_sound_buffer;