#include "GameManager.hpp"

GameManager::GameManager(sf::RenderWindow& window)
:window(window),
board_width(4), 
board_height(3),
cur_level(1),
num_models_of_enemy(2),
need_shoot(false),
move_direct_x(0.0f), move_direct_y(0.0f),
panel(sf::FloatRect(window.getSize().x - 250, 0, 250, window.getSize().y), window)
{
	// загружаем статическиe переменные для работы классов
	if (!AKR::loadResources())
		std::cout << "AKR load failed\n";

	if (!M416::loadResources())
		std::cout << "M416 load failed\n";

	if (!Desert_eagle::loadResources())
		std::cout << "Desert eagle load failed\n";

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
void GameManager::run(void)
{
	while (h.alive())
	{
		init();			// генерируем новый уровень
		while (h.alive() && alive_enemies != 0)
		{
			// ввод
			input();
			// обработка
			treatment();
			// вывод
			output();
		}
		new_level();		// усложняет уровень
	}
	// вывод результата игры todo
}
// обрабатывает ввод пользователя
void GameManager::input(void)
{
	// слушаем события
    sf::Event event;
    while (window.pollEvent(event))
    {
    	switch (event.type)
    	{
        // отслеживание закрытия окна
    	case sf::Event::Closed:
    		h.suicide();
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
	            h.planting(clock.getElapsedTime().asMilliseconds());
            	break;
            // отсележиваем нажатие пробела
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
    	// отслеживание нажатия кнопки мыши
    	case sf::Event::MouseButtonPressed:
    		switch (event.mouseButton.button)
    		{
    		// левой кнопи
    		case (sf::Mouse::Left):
    			if (!panel.try_press_button(h))
    			{
    				need_shoot = true;
    			}
    			break;
    		default: 
    			break;
    		}
    		break;
    	// отслеживание отпускания кнопки мыши
    	case sf::Event::MouseButtonReleased:
    		switch (event.mouseButton.button)
    		{
    		// левой кнопи
    		case (sf::Mouse::Left):
    			need_shoot = false;
    			break;
    		default: 
    			break;
    		}
    		break;
    	default:
    		break;
    	}
    }
}

// делает обработку ввода
void GameManager::treatment(void)
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

    // стреляем если была нажата кнопка мыши (не на кнопки)
    if (need_shoot)
    {
    	h.shooting(clock.getElapsedTime().asMilliseconds());
    }

    // обновляем патроны в оружии
    h.upgrade_gun(clock.getElapsedTime().asMilliseconds());

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
    	enemy.upgrade_gun(clock.getElapsedTime().asMilliseconds());
    }

    // делаем файтинг
    // проверяем столкновение enemy с пулями hero
    auto hero_bullets = h.get_bullets();
    for (auto i = hero_bullets.first; i != hero_bullets.second; ++i)
    {
    	// если есть пересечение c врагом
    	for (auto &enemy : enemies)
    	{
    		if (enemy.alive() && enemy.check_collision(*i, clock.getElapsedTime().asMilliseconds()))
    		{
    			// уменьшаем текущее здоровье врага на урон hero
    			enemy.change_cur_health(- h.get_damage());
    			if (!enemy.alive()) // если после попадания enemy умер
    			{
    				h.change_cash(enemy.get_cost());		// увеличиваем cash персонажа на его цену
					--alive_enemies;					// количестов живых врагов уменьшили
    			}
    		}
    	}
    }

    // проверяем столкновение hero c пулями enemy
  	for (auto &enemy : enemies)
  	{
  		auto enemy_bullets = enemy.get_bullets();
  		for (auto i = enemy_bullets.first; i != enemy_bullets.second; ++i)
  		{
  			__int64_t cur_time = clock.getElapsedTime().asMilliseconds();
  			// если прошло более 2 секунд сначала игры
  			if (cur_time - survival_start_time >= 2000)
  			{
      			if (h.check_collision(*i, cur_time))	// смотрим пересечение с пулей
      			{
      				h.change_cur_health(- enemy.get_damage());
      			}
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

// выводит изменения
void GameManager::output(void)
{
	window.clear();
	b.draw(window, clock.getElapsedTime().asMilliseconds());		// рисуем игровое поле

	// вычисляем координаты мыши
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f globalMousePos = window.mapPixelToCoords(mousePos);

	if (is_hero(globalMousePos))	// если мышь наведена на hero
	{
		panel.draw_people_info(h, clock.getElapsedTime().asMilliseconds());// выводим информацию о персонаже
	}
	else if (is_enemy(globalMousePos))	// если мышь наведена на enemy
	{
		panel.draw_people_info(get_enemy(globalMousePos), clock.getElapsedTime().asMilliseconds());
	}
	else 		// если мышь ни на что не наведина
	{
		panel.draw_shop(h, cur_level);// иначе, выводим магазин
	}
	for (auto &enemy : enemies)
	{
		enemy.draw(window, clock.getElapsedTime().asMilliseconds());
	}
	h.draw(window, clock.getElapsedTime().asMilliseconds());
	// Отображаем содержимое окна на экране
    window.display();
}

// изменяет параметры
void GameManager::new_level(void)
{
	// увеличиваем счетчик текущего уровня
	++cur_level;
	// увеличиваем размер поля
	++board_width; ++board_height;
	// чистим список врагов
	enemies.clear();
}

// по параметрам генерируем уровень
void GameManager::init(void)
{
	// запоминаем время начала текщуего уровня
	survival_start_time = clock.getElapsedTime().asMilliseconds();

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
	for (std::size_t i = 0; i < std::min(board_width, board_height); ++i)
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
		case 1:
			enemies[i].set_weapon(Desert_eagle(people_size * 1 / 4, people_size * 1 / 2, people_size * 1 / 2));
			break;
		}
	}

	alive_enemies = enemies.size(); 	// считаем количество живых врагов, изначально макс.

	// устанавливаем hero 
	h.set_position(sf::FloatRect(
		cell_w / 2 - people_size / 2,
		cell_h / 2 - people_size / 2,
		people_size, 
		people_size)
	);

	h.set_weapon(M416(people_size / 4, people_size / 2, people_size / 2));
	h.boom();	// взрываем все установленные бомбы
	h.add_bomb();
}	

// отвечает за взрыв поля
void GameManager::make_board_boom(void)
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
				if (cor.first == enemy_cor.x && cor.second == enemy_cor.y && enemy.alive())
				{
					enemy.change_cur_health(-Bomb::get_damage());// уменьшаем здоровье на урон бомбы
					if (!enemy.alive())	// если враг умер
					{
						h.change_cash(enemy.get_cost());	// увеличиваем cash на цену enemy
						--alive_enemies;					// количестов живых врагов уменьшили
					}
				}
			}
			// делаем тоже самое для hero
			if (cor.first == hero_cor.x && cor.second == hero_cor.y)
			{
				h.change_cur_health(-Bomb::get_damage());
			}
			// запускаем у текущей ячейки анимацию взрыва
			b.boom_cell(cor.first, cor.second, clock.getElapsedTime().asMilliseconds());
		}
		// взорвем ближайшую стену
		b.wall_destroyer(bomb_rect);
	}
}

// проверяет наведена ли в данный момент мышка на hero
bool GameManager::is_hero(const sf::Vector2f& globalMousePos)
{
    if (h.intersection(globalMousePos))
	{
		return true;
	}
	// если такого не нашли возращаем false
    return false;
}

// проверяет наведена ли в данный момент мышка на enemy
bool GameManager::is_enemy(const sf::Vector2f& globalMousePos)
{
    // ищем персонажа на которого наведена мышь
    for (auto &enemy : enemies)
    {
    	// произошло пересечение
    	if (enemy.intersection(globalMousePos))
    	{
    		return true;
    	}
    }
	// если такого не нашли возращаем false
    return false;
}

Enemy& GameManager::get_enemy(const sf::Vector2f& globalMousePos)
{
    // ищем персонажа на которого наведена мышь
    for (auto &enemy : enemies)
    {
    	// произошло пересечение
    	if (enemy.intersection(globalMousePos))
    	{
    		return enemy;
    	}
    }
    return enemies[0];
}