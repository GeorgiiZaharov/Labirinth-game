
#include "Bomb.hpp"

Bomb::Bomb(int w, int h)
    : activate(false),
      w(w), h(h),
      planting_time(2000), // 2 секунд время установки
      start_plant_time(0), 
      plant_duration(0),
      last_bomb_pos(-100, -100) // изначальная координата бомбы
{
    wait_anim = Animation(wait_tex, 100, 100, 2, 500, true);
    wait_anim.resize_animation(sf::FloatRect(0, 0, w, h));
}

void Bomb::planting(const sf::FloatRect& pos_obj, __int64_t cur_time)
{
    if (activate) return; // если бомба уже активирована то и ставить ее не нужно
    sf::Vector2f cur_bomb_pos(pos_obj.left + pos_obj.width / 2,
                              pos_obj.top + pos_obj.height / 2);
    // сравниваем текущее положение бомбы с предыдущим
    if (cur_bomb_pos.x == last_bomb_pos.x && cur_bomb_pos.y == last_bomb_pos.y)
    { // если они равны (бомба не передвигалась)
        if (cur_time - start_plant_time >= planting_time)
        {
            activate = true; // устанавливаем бомбу
            wait_anim.start_animation(cur_time);
            plant_duration = 0;
        }
        else
        {
            plant_duration = cur_time - start_plant_time;
        }
    }
    else // если же передвигалась
    { // изменяем стартовое время и позицию
        start_plant_time = cur_time;
        last_bomb_pos = cur_bomb_pos;
        plant_duration = 0;
    }
}

void Bomb::drop_planting(void)
{
    if (!activate)
    {
        last_bomb_pos = sf::Vector2f(-100, -100);
        plant_duration = 0;
    }
}

bool Bomb::loadResources(void)
{       
    if (!wait_tex.loadFromFile("bomb.png"))
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
    return std::make_pair(plant_duration, planting_time);
}

bool Bomb::is_activate(void) const
{
    return activate;
}

sf::Vector2f Bomb::get_pos(void) const
{
    return last_bomb_pos;
}

void Bomb::deactivate(void)
{
    activate = false;
}

int Bomb::get_damage(void)
{
    return damage;
}

sf::Texture Bomb::wait_tex;
int Bomb::damage = 75;