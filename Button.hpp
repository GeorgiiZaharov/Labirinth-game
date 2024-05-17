#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "includes.hpp"

class Button
{
public:
	/*
     * ===================================================
     * Конструкторы/Деструкторы
     * ===================================================
     */
	explicit Button(void) = default;
	explicit Button(const sf::FloatRect& pos, std::string str, const sf::Color& color=sf::Color::White);
	/*
     * ===================================================
     * Методы
     * ===================================================
     */
	// устанавливает флаг нажатой кнопки в случае нажатия
	void try_to_press(const sf::Vector2f& mousePos);
	// возращает флаг и затирает его
	bool is_pressed(void);
	// загружает ресурсы
	static bool loadResources(void);
	void draw(sf::RenderWindow& window, const sf::Vector2f& mousePos);
	/*
     * ===================================================
     * Геттеры/сеттеры
     * ===================================================
     */
	// возращает hitbox клетки
	sf::FloatRect get_hitbox(void) const;
private:
	sf::FloatRect big_pos, small_pos;
	bool pressed;
	sf::Text text;
	sf::Sprite big_sprite, small_sprite;
	static sf::Texture button_bg_tex;
	static sf::Font font;
};

#endif // BUTTON_HPP