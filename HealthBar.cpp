// HealthBar.cpp
#include "HealthBar.hpp"
#include <cmath>

// Constructor with parameters for position and initial health
HealthBar::HealthBar(const sf::FloatRect& cur_pos, int health)
    : cur_pos(cur_pos), max_health(health)
{
    // Initialize health bar and border sizes and positions
    healthBar.setSize(sf::Vector2f(cur_pos.width, cur_pos.height));
    healthBar.setPosition(sf::Vector2f(cur_pos.left, cur_pos.top));
    border.setSize(sf::Vector2f(cur_pos.width, cur_pos.height));
    border.setPosition(sf::Vector2f(cur_pos.left, cur_pos.top));
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineThickness(-3.f);
    border.setOutlineColor(sf::Color::Yellow);   // Border fill color

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

// Method to move the health bar and its border
void HealthBar::move(float bias_x, float bias_y)
{
    // Move both health bar and border
    border.move(bias_x, bias_y);
    healthBar.move(bias_x, bias_y);
}

void HealthBar::set_position(float w, float h)
{
    healthBar.setPosition(w, h);
    border.setPosition(w, h);
}


// Method to draw the health bar and its border on a window
void HealthBar::draw(sf::RenderWindow& window)
{
    window.draw(healthBar);
    window.draw(border);
}
