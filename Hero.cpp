#include "Hero.hpp"

Hero::Hero(void)
:People(100, 100),
cash(10)
{
    People::set_animation(
        Animation(hero_tex, 100, 100, 1, 0, true),
        Animation(walking_tex, 100, 100, 2, 500, false),
        Animation(blood_spatter_tex, 100 / 2, 100 / 2, 2, 1000, false),
        Animation(dead_tex, 100, 100, 1, 0, true), 
        Animation(present_tex, 250, 250, 2, 300, false)
    );
    // заполняем plantBar цветом
    plantBar.setFillColor(sf::Color::White);

}

bool Hero::loadResources(void)
{
    bool success = true;

    if (!hero_tex.loadFromFile("src/hero.png"))
        success = false;

    if (!walking_tex.loadFromFile("src/hero_walking.png"))
        success = false;

    if (!blood_spatter_tex.loadFromFile("src/blood_spatter.png"))
        success = false;

    if (!dead_tex.loadFromFile("src/dead.png"))
        success = false;

    if (!present_tex.loadFromFile("src/hero_presentation.png"))
        success = false;

    return success;
}

void Hero::add_bomb(void)
{
    bombs.emplace_back(pos.width / 2, pos.height / 2);
}

int Hero::get_bomb_cnt(void) const
{
    int cnt = 0;
    for (auto &bomb : bombs)
    {
        // считаем все неактивированные бомбы
        if (!bomb.is_activate())
        {
            ++cnt;
        }
    }
    return cnt;
}

void Hero::planting(__int64_t cur_time)
{
    if (alive())
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
    if (alive())
    {
        while (bombs.size() != 0 && bombs[0].is_activate() != false)
        {
            result.push_back(bombs[0].get_pos());
            bombs.erase(bombs.begin());
        }
    }    
    return result;
}

void Hero::suicide(void)
{
    cur_health = 0;
}

void Hero::draw(sf::RenderWindow& window, __int64_t cur_time)
{
    // ищем первую неактивированную бомбу и считаем для нее статистику по установке
    for (auto &bomb : bombs)
    {
        if (!bomb.is_activate())
        {
            std::pair<__int64_t, __int64_t> plant_info = bomb.get_plant_info();     // возращает информацию по установке бомбы
            if (plant_info.first == 0)  // если мы ее нисколько не плэнтили
            {
                break;
            }
            // находим процент установки бомбы
            float attitude = std::max(static_cast<float>(plant_info.first) / 
                static_cast<float>(plant_info.second), 0.0f);
            // рссчитываем позицию для индикатора установки бомбы
            plantBar.setSize(sf::Vector2f(15.0f, attitude * pos.height));
            plantBar.setPosition(pos.left - 15.0f, pos.top + (1 - attitude) * pos.height);
            // рисуем
            window.draw(plantBar);
            break;
        }
    }
    // отрисовывает все бомбы hero если они установлены
    for (auto &bomb : bombs)
    {
        if (bomb.is_activate())
        {
            bomb.draw(window, cur_time);
        }
        else
        {
            break;      // дошли до части неактивированных бомб => дальше смотреть смысла нет
        }
    }
    People::draw(window, cur_time);
}

void Hero::change_cash(int delta_cash)
{
    cash += delta_cash;
}

int Hero::get_cash(void) const
{
    return cash;
}
////////////////////////////////////////////////////

void Hero::update_cur_health(void)
{
    cur_health = max_health;
}


void Hero::change_max_health(int delta_health)
{
    max_health += delta_health;
    health_bar.set_max_health(max_health);
}

void Hero::change_damage(int delta_damage)
{
    gun.change_damage(delta_damage);
}

void Hero::change_speed(int delta_speed)
{
    speed += delta_speed;
}

void Hero::change_recharge_time(__int64_t delta_time)
{
    gun.change_recharge_time(delta_time);
}


void Hero::change_spread(float delta_spread)
{
    gun.change_spread(delta_spread);
}

void Hero::add_magazine(void)
{
    gun.add_magazine();
}

sf::Texture Hero::hero_tex, Hero::walking_tex, Hero::blood_spatter_tex, Hero::dead_tex, Hero::present_tex;
