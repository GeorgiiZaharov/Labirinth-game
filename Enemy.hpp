#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "People.hpp"
#include "Gun.hpp"
#include "Weapon.hpp"
#include <vector>
#include <SFML/Graphics.hpp>

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

private:
    // считает координаты сережины объекта
    inline sf::Vector2f get_mid_dot(const sf::FloatRect& rec);
    // проверяет путь до hero, возращает true если enemy его видит и false иначе
    bool is_open_pozition(void);

    sf::Vector2i cur_cor;
    std::vector<std::pair<int, int>> path_to_hero;
    sf::Vector2f hero_pos;
    bool saw_the_player;
    int distance;
    static std::vector<std::vector<sf::Texture>> textures; // для каждого вида врага 4 текстуры 
    // обычная, в движении, брызги крови, умер
    static std::vector<int> distances;
};

#endif // ENEMY_HPP
