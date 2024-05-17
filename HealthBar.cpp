// HealthBar.cpp
#include "HealthBar.hpp"
#include <cmath>

// Constructor with parameters for position and initial health
HealthBar::HealthBar(const sf::FloatRect& cur_pos, int health, int max_health)
    : cur_pos(cur_pos), max_health(max_health)
{
    // Initialize health bar and border sizes and positions
    healthBar.setSize(sf::Vector2f(cur_pos.width, cur_pos.height));
    healthBar.setPosition(sf::Vector2f(cur_pos.left, cur_pos.top));
    border.setSize(sf::Vector2f(cur_pos.width, cur_pos.height));
    border.setPosition(sf::Vector2f(cur_pos.left, cur_pos.top));
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineThickness(-2.f);       // толщина рамки 2 пикселя
    border.setOutlineColor(sf::Color::Black);   // цвет рамки Желтый todo

    // Set initial health
    set_health(health);
}

// Method to set the health of the health bar
void HealthBar::set_health(int cur_health)
{
    double attitude = static_cast<double>(cur_health) / max_health;
    if (attitude >= 0.75)
        healthBar.setFillColor(sf::Color::Green);
    else if (attitude >= 0.5)
        healthBar.setFillColor(sf::Color::Yellow);
    else
        healthBar.setFillColor(sf::Color::Red);
    healthBar.setSize(sf::Vector2f(attitude * cur_pos.width, cur_pos.height));
}

void HealthBar::set_max_health(int new_max_health)
{
    max_health = new_max_health;
}

// метод перемещает border и health бар на bias_x по оси х, bias_y по оси y
void HealthBar::move(float bias_x, float bias_y)
{
    // Move both health bar and border
    border.move(bias_x, bias_y);
    healthBar.move(bias_x, bias_y);
}

// метод отображает элементы HealthBar на экран
void HealthBar::draw(sf::RenderWindow& window)
{
    window.draw(healthBar);
    window.draw(border);
}
