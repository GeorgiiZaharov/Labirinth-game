#include <SFML/Graphics.hpp>
#include <iostream>
#include "Board.hpp"
#include "Hero.hpp"
#include "Enemy.hpp"


class GameManager
{
public:
	explicit GameManager(void) = default;
	explicit GameManager(sf::RenderWindow& window)
	:window(window),
	board_width(8), 
	board_height(6),
	cur_level(1),
	num_models_of_enemy(1),
	cash(0),
	mouse_button_is_pressed(false),
	move_direct_x(0.0f), move_direct_y(0.0f)
	{
		// загружаем статическиe переменные для работы классов
		if (!AKR::loadResources())
			std::cout << "AKR load failed\n";

		if (!Board::loadResources())
			std::cout << "BOARD load failed\n";

		if (!Hero::loadResources())
			std::cout << "HERO load failed\n";

		if (!Enemy::loadResources())
			std::cout << "ENEMY load failed\n";

		if (!Bomb::loadResources())
			std::cout << "Bomb load failed\n";

		// определение переменных
		// определяем игровое поле (изначально оно на весь экран)todo
		sf::Vector2u window_size = window.getSize();
		sf::FloatRect board_pos(0, 0, window_size.x - 250, window_size.y);
		b = Board(board_pos);
		h = Hero();
	}
	void run(void)
	{
		// пока мы не проиграли
		bool game_is_over = false;
		while (!game_is_over)
		{
			init();	// генерируем новый уровень
			while (true)
			{
				// ввод
				input();
				// обработка
				treatment();
				// вывод
				output();
			}
			new_level();
		}
		// вывод результата игры todo
	}
private:
	// обрабатывает ввод
	void input(void)
	{
		// слушаем события
	    sf::Event event;
	    while (window.pollEvent(event))
	    {
	    	switch (event.type)
	    	{
	        // отслеживание закрытия окна
	    	case sf::Event::Closed:
	    		window.close();
	    		break;
	        // отслеживание нажатия кнопок
	    	case sf::Event::KeyPressed:
	    		switch (event.key.code)
	    		{
	            // нажатие кнопок перемещения персонажа (WASD)
	    		case sf::Keyboard::W:
	                move_direct_y = -1.0f;
	                break;
	            case sf::Keyboard::D:
	            	move_direct_x = 1.0f;
	            	break;
	            case sf::Keyboard::S:
	            	move_direct_y = 1.0f;
	            	break;
	            case sf::Keyboard::A:
	            	move_direct_x = -1.0f;
	            	break;
	        	// отслеживаем нажатие кнопки 'B'
	            case sf::Keyboard::B:
	            	//std::cout << "B pressed" << std::endl;
		            h.planting(clock.getElapsedTime().asMilliseconds());
	            	break;
	            case sf::Keyboard::Space:
	            	make_board_boom();
	            	break;
	            default:
	            	break;
	    		}
	    		break;
	        // отслеживание отпускания клавиш
	    	case sf::Event::KeyReleased:
	    		switch (event.key.code)
	    		{
		    		case sf::Keyboard::W:
		                move_direct_y = 0.0f;
		                break;
		            case sf::Keyboard::D:
		            	move_direct_x = 0.0f;
		            	break;
		            case sf::Keyboard::S:
		            	move_direct_y = 0.0f;
		            	break;
		            case sf::Keyboard::A:
		            	move_direct_x = 0.0f;
		            	break;
	        		// отслеживание отпускания кнопки 'B'
		            case sf::Keyboard::B:
		            	h.drop_planting();
		            	break;
		            default:
		            	break;
	    		}
	    		break;

	    	default:
	    		break;
	    	}
	        // отслеживание нажатий мыши
	        if (event.type == sf::Event::MouseButtonPressed && 
	        	event.mouseButton.button == sf::Mouse::Left)
	        {
	        	mouse_button_is_pressed = true;
	        }
	        // отслеживание отпускания мыши
	        else if (event.type == sf::Event::MouseButtonReleased && 
	        	event.mouseButton.button == sf::Mouse::Left)
	        {
	        	mouse_button_is_pressed = false;
	        }
	    }
	}

	void treatment(void)
	{
		// эксплуатируем hero
		// разворачиваемся в нужном направлении
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f globalMousePos = window.mapPixelToCoords(mousePos);
        h.aim(globalMousePos.x, globalMousePos.y);

        // передвигаемся в соответсвии с нажатыми клавишами
        __int64_t cur_time = clock.getElapsedTime().asMilliseconds();
        h.moving(
        	cur_time,
        	move_direct_x,
        	move_direct_y
        );
        // если происходит пересечение хитбокса с картой => откатываемся
        if (b.check_collision(h.get_hitbox()))
        {
        	h.moving(
	        	cur_time + (cur_time - last_hero_moving_time),
	        	-move_direct_x,
	        	-move_direct_y
        	);
        }
        last_hero_moving_time = cur_time;

        // стреляем если была нажата кнопка мыши
        if (mouse_button_is_pressed)
        {
        	h.shooting(clock.getElapsedTime().asMilliseconds());
        }

        // эксплуатируем персонажей игры
        // находим позицию hero на board
        sf::Vector2i hero_cor = b.calc_pos_on_board(h.get_hitbox());
        // даем enemy из enemies представление об окружающей ситуации
        for (auto &enemy : enemies)
        {
        	sf::Vector2i enemy_cor = b.calc_pos_on_board(enemy.get_hitbox(), enemy.get_direct());
        	enemy.set_path_to_hero(b.navigator(enemy_cor, hero_cor));
        	enemy.set_cooridinate(enemy_cor);
        	enemy.set_hero_pos(h.get_hitbox());
        }
        // целемся стреляем двигаемся при необходимости
        for (auto &enemy : enemies)
        {
        	enemy.aim();
        	enemy.shooting(clock.getElapsedTime().asMilliseconds());
        	enemy.moving(clock.getElapsedTime().asMilliseconds());
        }

        // делаем файтинг
        // проверяем столкновение enemy с пулями hero
        auto hero_bullets = h.get_bullets();
        for (auto i = hero_bullets.first; i != hero_bullets.second; ++i)
        {
        	// если есть пересечение c врагом
        	for (auto &enemy : enemies)
        	{
        		if (enemy.check_collision(*i, clock.getElapsedTime().asMilliseconds()))
        		{
        			// уменьшаем текущее здоровье врага на урон hero
        			enemy.change_cur_health(- h.get_damage());
        		}
        	}
        }

        // проверяем столкновение hero c пулями enemy
      	for (auto &enemy : enemies)
      	{
      		auto enemy_bullets = enemy.get_bullets();
      		for (auto i = enemy_bullets.first; i != enemy_bullets.second; ++i)
      		{
      			if (h.check_collision(*i, clock.getElapsedTime().asMilliseconds()))
      			{
      				h.change_cur_health(- enemy.get_damage());
      			}
      		}
      	}
      	// ПРОВЕРКА СТОЛКНОВЕНИЙ ПУЛЬ СО СТЕНАМИ ЛАБИРИНТА
      	// проверяем столкновение пуль hero со стенами лабиринта
      	for (auto i = hero_bullets.first; i != hero_bullets.second; ++i)
      	{
      		b.check_collision(*i);
      	}
      	// проверка столкновения пуль enemies со стенами лабиринта
      	for (auto &enemy : enemies)
      	{
      		auto enemy_bullets = enemy.get_bullets();
      		for (auto i = enemy_bullets.first; i != enemy_bullets.second; ++i)
      		{
      			b.check_collision(*i);
      		}
      	}
	}

	void output(void)
	{
		window.clear();
		b.draw(window, clock.getElapsedTime().asMilliseconds());
		h.draw(window, clock.getElapsedTime().asMilliseconds());

		for (auto &enemy : enemies)
		{
			enemy.draw(window, clock.getElapsedTime().asMilliseconds());
		}
		// Отображаем содержимое окна на экране
        window.display();
	}

	// изменяет параметры
	void new_level(void)
	{
		// увеличиваем счетчик текущего уровня
		++cur_level;
		// увеличиваем размер поля
		++board_width; ++board_height;
		// чистим список врагов
		enemies.clear();
	}

	// по параметрам генерируем уровень
	void init(void)
	{
		// устанавливаем board_width клеток в ширину, board_height в высоту
		b.set_board_size(board_width, board_height);
		// генерируем лабиринт
		b.build();

		// обновляем размеры ячейки и получаем рекомендованный размер персонажа
		people_size = b.get_people_size();

		sf::Vector2f cell_size = b.get_cell_size();
		cell_w = cell_size.x; cell_h = cell_size.y;

		// расставляем врагов, так чтобы они находились в уникальных позициях

		// ИНИЦИАЛИЗИРУЕМ ГЕНЕРАТОРО СЛУЧАЙНЫХ ЧИСЕЛ
		// Создание объекта случайного устройства для получения истинно случайного начального значения
	    std::random_device rd;
	    // Создание объекта генератора случайных чисел с начальным значением от случайного устройства
	    std::mt19937 gen(rd());
	    // Создание объекта равномерного распределения от 0 до 2^16
	    std::uniform_int_distribution<> dis(0, 1 << 16);
	    // ЗАКОНЧИЛИ ИНИЦИАЛИЗИРОВАТЬ ГЕНЕРАТОР

	    // расставляем врагов
	    int cur_row, cur_col;
		std::set<std::pair<int, int>> enemies_positions; // сет со стартовыми позициями enemy
		// количество врагов будет равно max(board_width, board_height)
		for (std::size_t i = 0; i < std::max(board_width, board_height); ++i)
		{
			cur_row = dis(gen) % board_height;
			cur_col = dis(gen) % board_width;
			// до тех пор пока координата не уникальна продолжаем генерировать
			// также нельзя чтобы враг стоял в тойже ячейке что и hero
			while (0 != enemies_positions.count(std::make_pair(cur_row, cur_col)) || cur_row + cur_col == 0)
			{
				cur_row = dis(gen) % board_height;
				cur_col = dis(gen) % board_width;
			}
			// запоминаем новую уникальную позицию
			enemies_positions.insert(std::make_pair(cur_row, cur_col));
			// генерируем тип enemy
			int cur_num_model = dis(gen) % num_models_of_enemy;
			// вычисляем позицию на карте
			sf::FloatRect cur_enemy_pos(
				cur_col * cell_w + cell_w / 2 - people_size / 2,
				cur_row * cell_h + cell_h / 2 - people_size / 2,
				people_size,
				people_size
				);

			// заносим enemy в enemies
			enemies.emplace_back(cur_num_model);
			// устанавливаем в нужную позицию
			enemies[i].set_position(cur_enemy_pos);

			// в зависимости от типа enemy устанавливаем оружие и спрайт
			switch (cur_num_model)
			{
			case 0:
				enemies[i].set_weapon(AKR(people_size * 1 / 4, people_size * 1 / 2, people_size * 1 / 2));
				break;
			}
		}

		// устанавливаем hero 
		h.set_position(sf::FloatRect(
			cell_w / 2 - people_size / 2,
			cell_h / 2 - people_size / 2,
			people_size, 
			people_size)
		);

		h.set_weapon(AKR(people_size / 4, people_size / 2, people_size / 2));
		h.add_bomb();
	}

	void make_board_boom(void)
	{
		// получаем позиции бомбы на карте
		std::vector<sf::Vector2f> bombs_pos = h.boom();
		// для каждой позиции
		for (auto bomb_pos : bombs_pos)
		{
			// запишем это в виде sf::FloatRect
			sf::FloatRect bomb_rect(bomb_pos.x, bomb_pos.y, 0, 0);
			// получим список клеток, которые подпадают под взрыв
			std::vector<std::pair<int, int>> cell_cors = b.neighbour(bomb_rect);
			//считаем ячейку в которой находится hero
			sf::Vector2i hero_cor = b.calc_pos_on_board(h.get_hitbox());
			// просматриваем всех персонажей и проверяем принадлежность этим ячейкам
			for (auto cor : cell_cors)
			{
				for (auto &enemy : enemies)
				{
					// вычисляем координату enemy на доске
					sf::Vector2i enemy_cor = b.calc_pos_on_board(enemy.get_hitbox());
					if (cor.first == enemy_cor.x && cor.second == enemy_cor.y)
					{
						enemy.change_cur_health(-Bomb::get_damage());// уменьшаем здоровье на урон бомбы
					}
				}
				// делаем тоже самое для hero
				if (cor.first == hero_cor.x && cor.second == hero_cor.y)
				{
					h.change_cur_health(-Bomb::get_damage());
				}
				// запускаем у текцщей ячейки анимацию взрыва
				b.boom_cell(cor.first, cor.second, clock.getElapsedTime().asMilliseconds());
			}
			// взорвем ближайшую стену
			b.wall_destroyer(bomb_rect);
		}
		h.add_bomb();
	}

	sf::RenderWindow &window;
	std::size_t board_width, board_height, cur_level, num_models_of_enemy;
	float cell_w, cell_h, people_size;
	std::size_t cash;
	bool mouse_button_is_pressed;
	float move_direct_x, move_direct_y; // направление ходьбы персонажа
	Board b;
	Hero h;
	__int64_t last_hero_moving_time;
	std::vector<Enemy> enemies;
	sf::Clock clock;
};

int main()
{
	    // Создаем объект окна
    sf::RenderWindow window(sf::VideoMode(1000, 600), "My SFML Window");
    window.setFramerateLimit(60);

	GameManager g = GameManager(window);
	g.run();
}