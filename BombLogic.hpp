#ifndef BOMB_LOGIC_HPP
#define BOMB_LOGIC_HPP value

#include "includes.hpp"

class BombLogic
{
public:
	/*
    * ===================================================
    * Методы
    * ===================================================
    */
	// устанавливает бомбу возращает статус установки (получилось или нет)
    bool planting(const sf::FloatRect& pos_obj, __int64_t cur_time);
    
    // сбрасывает установку бомбы
    void drop_planting(void);

    // деактивиркет бомбу
    void deactivate(void);

    // возращает статус уставновки бомбы
	bool is_activate(void) const;

    /*
    * ===================================================
    * Геттеры/сеттеры
    * ===================================================
    */
    // возращает "сколько плэнтим"/"сколько нужно"
    std::pair<__int64_t, __int64_t> get_plant_info(void) const;

    // возращает позицию бомбы
    sf::Vector2f get_pos(void) const;

    bool activate; // Флаги для управления состоянием бомбы
    __int64_t planting_time, start_plant_time, plant_duration; // Время установки и время начала установки
    sf::Vector2f last_bomb_pos; // Последняя известная позиция бомбы

};

#endif