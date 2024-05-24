#ifndef BOMB_HPP
#define BOMB_HPP

#include "includes.hpp"
#include "BombLogic.hpp"
#include "Animation.hpp"

class Bomb : private BombLogic // отношение "Реализовано посредством"
{
public:
    /*
    * ===================================================
    * Конструкторы
    * ===================================================
    */
    explicit Bomb(void) = default;
    explicit Bomb(int w, int h);
    /*
    * ===================================================
    * Методы
    * ===================================================
    */
    // загружает ресурсы
    static bool loadResources(void);
    // устанавливает бомбу
    void planting(const sf::FloatRect& pos_obj, __int64_t cur_time);
    // сбрасывает установку бомбы
    void drop_planting(void);

    // рисует бомбу, если нужно
    void draw(sf::RenderWindow& window, __int64_t cur_time);
    // деактивиркет бомбу
    void deactivate(void);
    /*
    * ===================================================
    * Геттеры/сеттеры
    * ===================================================
    */
    // возращает "сколько плэнтим"/"сколько нужно"
    std::pair<__int64_t, __int64_t> get_plant_info(void) const;

    // возращает ответ на "бомба активирована?"
    bool is_activate(void) const;

    // возращает позицию бомбы
    sf::Vector2f get_pos(void) const;

    static int get_damage(void);


private:
    int w, h; // Ширина и высота бомбы и ее урон
    Animation wait_anim;
    static sf::Texture wait_tex; // Анимации ожидания
    static int damage;
};

#endif // BOMB_HPP
