#ifndef HERO_HPP
#define HERO_HPP

#include <SFML/Graphics.hpp>
#include <vector>
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
    // устанавливает бомбу
    void planting(__int64_t cur_time);
    // сбрасывает установку бомбы
    void drop_planting(void);
    // взрывает все установленные бомбы и возращает их позиции
    std::vector<sf::Vector2f> boom(void);

private:
    sf::RectangleShape plantBar, plantBarBorder;
    std::vector<Bomb> bombs;
    static sf::Texture hero_tex, walking_tex, blood_spatter_tex, dead_tex;
};

#endif // HERO_HPP
