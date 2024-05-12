// Реализация класса Bullet, отвечающего за управление пулей в игре.

#include "Bullet.hpp"
#include <cmath>

// Конструктор, инициализирует пулю с заданными свойствами.
Bullet::Bullet(const sf::Sprite& bullet_sprite, const sf::FloatRect& poz, 
    float dx, float dy, float speed, __int64_t cur_time)
    : was_collision(false), dx(dx), dy(dy), speed(speed), last_time(cur_time), sprite(bullet_sprite)
{
    // Устанавливает начальную позицию пули.
    sprite.setPosition(poz.left, poz.top);

    // Вычисляет угол направления движения пули.
    float angle = std::atan2(dy, dx) * 180 / M_PI;
    sprite.setRotation(angle);

    // Нормализует скорости dx и dy, чтобы они были единичными.
    float length = std::hypot(dx, dy);
    dx /= length;
    dy /= length;
}

// Возвращает границы пули.
sf::FloatRect Bullet::get_rect(void) const
{
    return sprite.getGlobalBounds();
}

// Обновляет позицию пули на основе текущего времени.
void Bullet::move(__int64_t cur_time)
{
    // если пуля еще не с чем не сталкивалась двигаем ее
    if (!was_collision)
    {
        float delta_time = static_cast<float>(cur_time - last_time) / 1000;
        float bias_x = dx * (delta_time * speed);
        float bias_y = dy * (delta_time * speed);
        sprite.move(bias_x, bias_y);
        last_time = cur_time;
    }
}

// Отрисовывает пулю на заданном окне.
void Bullet::draw(sf::RenderWindow& window)
{
    // если пуля еще не с чем не сталкивалась отрисовываем ее
    if (!was_collision)
    {
        window.draw(sprite);
    }
}

bool Bullet::get_collision(void) const
{
    return was_collision;
}

sf::FloatRect Bullet::getGlobalBounds(void) const
{
    return sprite.getGlobalBounds();
}

sf::Vector2f Bullet::get_direct(void)
{
    return sf::Vector2f(dx, dy);
}

void Bullet::collision(void)
{
    was_collision = true;
}
