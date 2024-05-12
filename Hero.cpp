#include "Hero.hpp"

Hero::Hero(void)
:People(100, 100)
{
    People::set_animation(
        Animation(hero_tex, 100, 100, 1, 0, true),
        Animation(walking_tex, 100, 100, 2, 500, false),
        Animation(blood_spatter_tex, 100 / 2, 100 / 2, 1, 1000, false),
        Animation(dead_tex, 100, 100, 1, 0, true)
    );
}

bool Hero::loadResources(void)
{
    bool success = true;

    if (!hero_tex.loadFromFile("hero.png"))
        success = false;

    if (!walking_tex.loadFromFile("walking_anim.png"))
        success = false;

    if (!blood_spatter_tex.loadFromFile("blood_spatter_anim.png"))
        success = false;

    if (!dead_tex.loadFromFile("dead.png"))
        success = false;

    return success;
}

void Hero::add_bomb(void)
{
    bombs.emplace_back(pos.width / 2, pos.height / 2);
}

void Hero::planting(__int64_t cur_time)
{
    for (auto &bomb : bombs)
    {
        // начинаем плэнтить первую неактивированную бомбу
        if (!bomb.is_activate())
        {
            bomb.planting(pos, cur_time);
            break;
        }
    }
}

void Hero::drop_planting(void)
{
    for (auto &bomb : bombs)
    {
        // сбрасываем первую неактивированную бомбу
        if (!bomb.is_activate())
        {
            bomb.drop_planting();
            break;
        }
    }
}

std::vector<sf::Vector2f> Hero::boom(void)
{
    std::vector<sf::Vector2f> result;
    while (bombs.size() != 0 && bombs[0].is_activate() != false)
    {
        result.push_back(bombs[0].get_pos());
        bombs.erase(bombs.begin());
    }
    
    return result;
}

void Hero::draw(sf::RenderWindow& window, __int64_t cur_time)
{
    for (auto bomb : bombs)
    {
        if (bomb.is_activate())
        {
            bomb.draw(window, cur_time);
        }
        else
        {
            break;
        }
    }
    People::draw(window, cur_time);
}

sf::Texture Hero::hero_tex, Hero::walking_tex, Hero::blood_spatter_tex, Hero::dead_tex;
