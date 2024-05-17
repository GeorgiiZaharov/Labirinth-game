#ifndef GAME_MANAGER_HPP
#define GAME_MANAGER_HPP

#include "includes.hpp"

#include "Board.hpp"
#include "Hero.hpp"
#include "Enemy.hpp"
#include "Button.hpp"
#include "Panel.hpp"

class GameManager
{
public:
	explicit GameManager(void) = default;
	explicit GameManager(sf::RenderWindow& window);
	void run(void);
private:
	// обрабатывает ввод пользователя
	void input(void);
	// делает обработку ввода
	void treatment(void);
	// выводит изменения
	void output(void);

	// изменяет параметры
	void new_level(void);

	// по параметрам генерируем уровень
	void init(void);

	// отвечает за взрыв поля
	void make_board_boom(void);

	// проверяет наведена ли в данный момент мышка на hero
	bool is_hero(const sf::Vector2f& globalMousePos);

	// проверяет наведена ли в данный момент мышка на enemy
	bool is_enemy(const sf::Vector2f& globalMousePos);

	// возращает enemy на которого наведена мышь
	Enemy& get_enemy(const sf::Vector2f& globalMousePos);

	sf::RenderWindow &window;	// окно в котором будем это отображать
	std::size_t board_width, board_height, cur_level, num_models_of_enemy;	// переменные для игры

	float cell_w, cell_h, people_size;	// информация о доске
	bool need_shoot;       				// зажата ли клавиша мыши используется для стрельбы (удержание => очередь)
	float move_direct_x, move_direct_y; // направление ходьбы персонажа
	int alive_enemies;
	Board b;							// объект доски
	Hero h;								// объект персонажа
	__int64_t last_hero_moving_time;	// нужно чтобы откатываться назад при столкновении
	__int64_t survival_start_time;		// время начала уровня, первые 2 секунды неуязвим
	std::vector<Enemy> enemies;			// массив с объектами врагов
	sf::Clock clock;					// таймер
	Panel panel;
};


#endif