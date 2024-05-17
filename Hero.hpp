#ifndef HERO_HPP
#define HERO_HPP

#include "includes.hpp"

#include "People.hpp"
#include "Gun.hpp"
#include "Bomb.hpp"

class Hero : public People
{
public:
    /*
    * ===================================================
    * Конструкторы
    * ===================================================
    */
    explicit Hero (void);
    /*
    * ===================================================
    * Методы
    * ===================================================
    */
    void draw(sf::RenderWindow& window, __int64_t cur_time);
    // загружает ресурсы
    static bool loadResources(void);
    // добавляет момбу которую можно установить
    void add_bomb(void);   
    // возращает количество неустановленных бомб
    int get_bomb_cnt(void) const;

    // устанавливает бомбу
    void planting(__int64_t cur_time);
    // сбрасывает установку бомбы
    void drop_planting(void);
    // взрывает все установленные бомбы и возращает их позиции
    std::vector<sf::Vector2f> boom(void);

    // убивает hero делает cur_health = 0
    void suicide(void);
    /*
    * ===================================================
    * Геттеры/сеттеры
    * ===================================================
    */
    // изменяет значение cash на delta_cash
    void change_cash(int delta_cash);

    // возращает значение cash
    int get_cash(void) const;

    /*
    *  Запросы на изменение / установку
    */
    // устанавливает текущее здоровье равное максимально доступному (max_health)
    void update_cur_health(void);

    // изменяет максимальное здоровье
    void change_max_health(int delta_health);

    // // устанавливает максимальное здоровье
    // void set_max_health(int new_max_health);

    // изменяет урон
    void change_damage(int delta_damage); 

    // устанавливает скорость персонажа
    void change_speed(int delta_speed);

    // Добавляет рожок с патронами
    void add_magazine(void);

    // Изменяет скорость перезарядки
    void change_recharge_time(__int64_t delta_time);

    // Изменяет разброс
    void change_spread(float delta_spread);

private:
    sf::RectangleShape plantBar;
    std::vector<Bomb> bombs;
    static sf::Texture hero_tex, walking_tex, blood_spatter_tex, dead_tex, present_tex;
    int cash;
};

#endif // HERO_HPP
