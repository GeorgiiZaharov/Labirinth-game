// Заголовочный файл класса Bullet, который представляет объект пули в игре.
// Этот класс отвечает за управление пулей, включая их движение и отрисовку.

#ifndef BULLET_HPP
#define BULLET_HPP

#include <SFML/Graphics.hpp>

class Bullet
{
public:
    /*
     * ===================================================
     * Конструкторы
     * ===================================================
     */
    explicit Bullet() = default;
    explicit Bullet(const sf::Sprite& bullet_sprite, const sf::FloatRect& poz, 
        float dx, float dy, float speed, __int64_t cur_time);

    /*
     * ===================================================
     * Методы
     * ===================================================
     */
    // Возвращает границы пули в виде sf::FloatRect.
    sf::FloatRect get_rect(void) const;

    // Обновляет позицию пули на основе текущего времени.
    void move(__int64_t cur_time);

    // Отрисовывает пулю на заданном окне.
    void draw(sf::RenderWindow& window);

    // возращает was_collision (было столкновение или нет)
    bool get_collision(void) const;

    // возращает текущие координаты пули
    sf::FloatRect getGlobalBounds(void) const;

    // возращает направление пули (dx, dy)
    sf::Vector2f get_direct(void);

    // устанавливает collision = true
    void collision(void);

private: 
    // Флаг того, была ли коллизия с объектами или нет
    bool was_collision;

    // Скорость движения пули по оси X.
    float dx;

    // Скорость движения пули по оси Y.
    float dy;

    // Скорость пули.
    int speed;

    // Время последнего обновления позиции пули.
    __int64_t last_time;

    // Спрайт пули.
    sf::Sprite sprite;
};

#endif // BULLET_HPP
