#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "includes.hpp"

#include "People.hpp"
#include "Gun.hpp"
#include "Weapon.hpp"

class Enemy : public People
{
public:
    /*
    * ===================================================
    * Конструкторы
    * ===================================================
    */
    explicit Enemy(void) = default;
    explicit Enemy(std::size_t num_model);

    static bool loadResources(void);
    /*
    * ===================================================
    * Геттеры/сеттеры
    * ===================================================
    */
    // загружает путь до hero
    void set_path_to_hero(std::vector<std::pair<int, int>> path_to_hero);
    // загружает текущюю позицию на карте
    void set_cooridinate(const sf::Vector2i& cor);
    // устанавливает координаты героя
    void set_hero_pos(const sf::FloatRect& hero_pos);
    /*
    * ===================================================
    * Методы
    * ===================================================
    */
    // К ВЫЗОВУ ЭТИХ МЕТОДОВ ДОЛЖНЫ БЫТЬ УСТАНОВЛЕНЫ АКТУАЛЬНЫЕ ЗНАЧЕНИЯ!!!
    void aim(void);
    void shooting(__int64_t cur_time);
    void moving(__int64_t cur_time);
    /*
    * ===================================================
    * Геттеры/сеттеры
    * ===================================================
    */
    // возращает цену за убийство данного персонажа
    int get_cost(void) const;
    // возращает дальность видимости персонажа
    int get_distance(void) const;

private:
    // считает координаты сережины объекта
    inline sf::Vector2f get_mid_dot(const sf::FloatRect& rec);
    // проверяет путь до hero, возращает true если enemy его видит и false иначе
    bool is_open_pozition(void);

    sf::Vector2i cur_cor;           // текущии координаты на доске
    std::vector<std::pair<int, int>> path_to_hero;  // вектор с полным путем к hero
    sf::Vector2f hero_pos;          // позиция hero на экране
    bool saw_the_player;            // флаг того, видел ли self hero
    int distance;                   // дистанция обнаружения
    int cost;                       // цена за убийство данного врага
    static std::vector<std::vector<sf::Texture>> textures; // для каждого вида врага 4 текстуры 
    // обычная, в движении, брызги крови, умер
    static std::vector<int> distances;                      // для каждого типа своя дальность видимости
    static std::vector<int> costs;                          // для каждого типа своя награда за убийство
};

#endif // ENEMY_HPP
