#ifndef PANEL_HPP

#include "includes.hpp"

#include "Button.hpp"
#include "Hero.hpp"
#include "Enemy.hpp"


class Panel
{
public:
	explicit Panel(void) = default;
	explicit Panel(const sf::FloatRect& pos, sf::RenderWindow& window);

	// загружает ресурсы
	bool loadResources(void);

	// отображает на window боковую панель с информацией о герое
	void draw_people_info(Hero& h, __int64_t cur_time);

	// отображает на window боковую панель с информацией о враге
	void draw_people_info(Enemy& enemy, __int64_t cur_time);

	// отображает меню магазина
	void draw_shop(const Hero& h, int cur_level);

	// изменяет характеристики персонажа в соответсвии с выбранным улучшением возращает true в случае нажатия
	bool try_press_button(Hero& h);
private:
	// инициализирует info panel с кнопами
	void button_init(void);
	// настраивает sf::Text под единый стиль
	void settings_text(sf::Text& text, const sf::Color& color=sf::Color::White, int size=20);

	enum costs					// цены на товары
	{
		BOMB = 5,				// добавляем бомбу
		MAGAZINE = 1,			// добавляет магазин для оружия
		RECHARGE = 1,			// ускоряет перезарядку
		FLAME_ARRESTER = 1,		// уменьшает разброс
		FIRST_AID_KIT = 3,		// лечит
		MAX_HEALTH = 5,			// увеличивает максимальное здоровье
		SPEED = 8 				// увеличивает скорость
	};
	sf::FloatRect pos; 			// позиция боковой панели на экране
	sf::RenderWindow &window;
	Button bomb_btn, magazine_btn, 		// кнопки для управления игрой
		recharge_btn, flame_arrester_btn, 
		first_aid_kit_btn, max_health_btn, speed_btn, 
		menu_btn; 			
	sf::Text people_info_text, gun_info_text, 	// sf::Text для вывода информации об игре
		title1_text, title2_text, cash_text,
		cur_level_text;
	sf::Sprite panel_bg_sprite;
	sf::Font font;						// шрифт для вывод текста
	sf::Texture panel_bg_tex;
};

#define PANEL_HPP value
#endif