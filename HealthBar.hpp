// HealthBar.hpp
#ifndef HEALTHBAR_HPP
#define HEALTHBAR_HPP

#include "includes.hpp"

class HealthBar
{
public:
    /*
    * ===================================================
    * Конструкторы
    * ===================================================
    */
    // Default constructor
    explicit HealthBar(void) = default;
    // Constructor with parameters for position and initial health
    explicit HealthBar(const sf::FloatRect& cur_pos, int health, int max_health);

    /*
    * ===================================================
    * Методы
    * ===================================================
    */
    // Method to set the health of the health bar
    void set_health(int cur_health);

    // Изменяет максимальное значение здоровья
    void set_max_health(int new_max_health);
    
    // Method to move the health bar and its border
    void move(float bias_x, float bias_y);

    // Method to draw the health bar and its border on a window
    void draw(sf::RenderWindow& window);
private:
    // Health bar and border shapes
    sf::RectangleShape healthBar, border;

    // Current position and maximum health
    sf::FloatRect cur_pos;
    int max_health;
};

#endif // HEALTHBAR_HPP
