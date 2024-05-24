#include "Bomb.hpp"

Bomb::Bomb(int w, int h)
: w(w), h(h)
{
    activate = false;
    planting_time = 2000;  // 2 секунд время установки
    start_plant_time = 0;
    plant_duration = 0;
    last_bomb_pos = sf::Vector2f(-100.0f, -100.0f);

    wait_anim = Animation(wait_tex, 100, 100, 2, 500, true);
    wait_anim.resize_animation(sf::FloatRect(0, 0, w, h));
}

void Bomb::planting(const sf::FloatRect& pos_obj, __int64_t cur_time)
{
    if (BombLogic::planting(pos_obj, cur_time)) // елсли получилось установить бомбу
    {
        wait_anim.start_animation(cur_time);    // запускаем анимацию
    }
}

void Bomb::drop_planting(void)
{
    BombLogic::drop_planting();
}

bool Bomb::loadResources(void)
{       
    if (!wait_tex.loadFromFile("src/bomb.png"))
        return false;
    return true;
}

void Bomb::draw(sf::RenderWindow& window, __int64_t cur_time)
{
    sf::Sprite to_draw = wait_anim.get_sprite(cur_time);
    to_draw.setPosition(last_bomb_pos);
    window.draw(to_draw);
}

std::pair<__int64_t, __int64_t> Bomb::get_plant_info(void) const
{
    return BombLogic::get_plant_info();
}

bool Bomb::is_activate(void) const
{
    return BombLogic::is_activate();
}

sf::Vector2f Bomb::get_pos(void) const
{
    return BombLogic::get_pos();
}

void Bomb::deactivate(void)
{
    BombLogic::deactivate();
}

int Bomb::get_damage(void)
{
    return damage;
}

sf::Texture Bomb::wait_tex;
int Bomb::damage = 95;