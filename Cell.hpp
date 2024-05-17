#ifndef CELL_HPP
#define CELL_HPP

#include "includes.hpp"

#include "Animation.hpp"

class Board;

class Cell {
	friend class Board;
public:
    /*
    * ===================================================
    * Конструкторы
    * ===================================================
    */
	explicit Cell() = default;
    explicit Cell(const sf::Texture& bg_tex, const sf::Texture& border_tex, float x, float y, float cell_w, float cell_h, float border_width);
    /*
    * ===================================================
    * Методы
    * ===================================================
    */
    // загружает игровые ресурсы
    static bool loadResources(void);
    //запускает анимацию вызрыва ячейки
    void make_boom(__int64_t cur_time);
    // рисует ячейку на экран
    void draw(sf::RenderWindow& window, __int64_t cur_time);
    // проверяет пересечение ячейки с переданным объектом
    bool check_collision(const sf::FloatRect& r) const;
protected:
    bool is_up, is_right, is_down, is_left;

private:
    void pruning(sf::Sprite& sprite, const sf::Texture& tex, const sf::FloatRect& pos);
    sf::Sprite rec, line_up, line_right, line_down, line_left;
    Animation boom_anim;
    static sf::Texture boom_tex;
    static sf::SoundBuffer boom_buf;
    static sf::Sound boom_sound;
};

#endif
