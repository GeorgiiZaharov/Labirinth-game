#include "BombLogic.hpp"
bool BombLogic::planting(const sf::FloatRect& pos_obj, __int64_t cur_time)
{
    if (activate) return false; // если бомба уже активирована то и ставить ее не нужно
    sf::Vector2f cur_bomb_pos(pos_obj.left + pos_obj.width / 2,
                              pos_obj.top + pos_obj.height / 2);
    // сравниваем текущее положение бомбы с предыдущим
    if (cur_bomb_pos == last_bomb_pos)
    { // если они равны (бомба не передвигалась)
        plant_duration = cur_time - start_plant_time;   // пересчитываем сколько мы устанавливали бомбу
        if (plant_duration >= planting_time)       // прошло ли достаточно времени
        {
            activate = true;                        // устанавливаем бомбу
        }
    }
    else // если же передвигалась
    { // изменяем стартовое время и позицию
        start_plant_time = cur_time;
        last_bomb_pos = cur_bomb_pos;
        plant_duration = 0;
    }
    return activate;
}

void BombLogic::drop_planting(void)
{
    if (!activate)
    {
        last_bomb_pos = sf::Vector2f(-100, -100);
        plant_duration = 0;
    }
}

void BombLogic::deactivate(void)
{
    activate = false;    
}

bool BombLogic::is_activate(void) const
{
    return activate;
}

std::pair<__int64_t, __int64_t> BombLogic::get_plant_info(void) const
{
    return std::make_pair(plant_duration, planting_time);
}

sf::Vector2f BombLogic::get_pos(void) const
{
    return last_bomb_pos;
}
