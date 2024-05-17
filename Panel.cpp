#include "Panel.hpp"

Panel::Panel(const sf::FloatRect& pos, sf::RenderWindow& window)
: pos(pos), 
window(window)
{
	if (!loadResources())
		std::cout << "Panel load failed\n";

	// Подготовка sf::Text
	settings_text(people_info_text);
	settings_text(gun_info_text);
	settings_text(title1_text, sf::Color::Blue);
	settings_text(title2_text, sf::Color::Blue);
	settings_text(cash_text, sf::Color::Yellow);
	settings_text(cur_level_text, sf::Color::Red, 24);

	// обозначаем позицию боковой панели и устанавливаем изображение заднего фона
	panel_bg_sprite.setTexture(panel_bg_tex);

	sf::FloatRect img_bounds = panel_bg_sprite.getGlobalBounds();

	panel_bg_sprite.setScale(		// масштабируем картинку заднего фона
		pos.width / img_bounds.width, 
		pos.height / img_bounds.height
		);

	panel_bg_sprite.setPosition(pos.left, pos.top);
	// инициализируем кнопки
	button_init();
}

// загружает статические ресурсы
bool Panel::loadResources(void)
{
	bool success = true;
	// загружаем ресурсы
	if (!font.loadFromFile("src/Roboto-Black.ttf"))
		success = false;
	if (!Button::loadResources())
		success = false;
	if (!panel_bg_tex.loadFromFile("src/panel_bg.png"))
		success = false;
	return success;
}


// отображает на window боковую панель с информацией о герое
void Panel::draw_people_info(Hero& h, __int64_t cur_time)
{
	h.start_presentation_animation(cur_time);
	// заголовок
    title1_text.setString("Hero characteristics");
	title1_text.setPosition(pos.left, 
								pos.top);
	// вычисляем его границы, чтобы знать куда размещять следующий
	sf::FloatRect last_obj_pos = title1_text.getGlobalBounds();

	// получаем спрайт для презентации
	sf::Sprite present_sprite_people = h.get_presentation_sprite(cur_time);
	present_sprite_people.setOrigin(0.0f, 0.0f);
	// размещаем его
	// вычисляем позицию картинки на доске
	present_sprite_people.setPosition(last_obj_pos.left, 
								pos.top + last_obj_pos.height + 15.0f);
	last_obj_pos = present_sprite_people.getGlobalBounds();
	// обновляем позицию картинки на доске
	last_obj_pos = present_sprite_people.getGlobalBounds();
	// получаем информацию о h
	std::string people_info_string = "\tHealth:\t" + std::to_string(std::max(0, h.get_cur_health())) + "\n" +
						 			 "\tSpeed:\t"  + std::to_string(h.get_speed() / 10) + "\n" +
						 			 "\tBombs:\t"  + std::to_string(h.get_bomb_cnt()) + "\n" +
						 			 "\tBomb damage:\t" + std::to_string(Bomb::get_damage()) + "\n";
	// вставляем ее в sf::Text для представления
	people_info_text.setString(people_info_string);
	// позиционируем ее в info_panel
	people_info_text.setPosition(last_obj_pos.left,
								last_obj_pos.top + last_obj_pos.height);

	last_obj_pos = people_info_text.getGlobalBounds();
	// Заголовок (что мы смоотрим характеристики оружия)
	title2_text.setString("Gun characteristics");
	title2_text.setPosition(last_obj_pos.left,
								last_obj_pos.top + last_obj_pos.height);
	// вычисляем позицию предыдущего элемента
	last_obj_pos = title2_text.getGlobalBounds();

	// получаем спрайт презентации оружия
	sf::Sprite present_sprite_gun = h.get_gun_presentation_sprite(cur_time);
	present_sprite_gun.setOrigin(0, 0);
	// устанавливаем его в нужную позицию
	present_sprite_gun.setPosition(last_obj_pos.left,
									last_obj_pos.top + last_obj_pos.height);
	// обновляем позицию картинки на доске
	last_obj_pos = present_sprite_gun.getGlobalBounds();
	// получаем информацию о оружии
	std::string gun_info_str = "\tBullets:\t" + std::to_string(h.get_magazine_info().first) + "/" + std::to_string(h.get_magazine_info().second) + "\n" +
								"\tDamage:\t" + std::to_string(h.get_damage()) + "\n" +
								"\tSpread:\t" + std::to_string(static_cast<int>(h.get_spread())) + "deg\n" +
								"\tMagazines:\t" + std::to_string(h.get_gun_info()) + "\n" +
								"\tRecharge time:\t" + std::to_string(h.get_recharge_time() / 1000) + "sec.";    		gun_info_text.setString(gun_info_str);
	gun_info_text.setPosition(last_obj_pos.left,
									last_obj_pos.top + last_obj_pos.height);
	// рисуем задний фон боковой панели
	window.draw(panel_bg_sprite);

	window.draw(title1_text);
	window.draw(present_sprite_people);
	window.draw(people_info_text);

	window.draw(title2_text);
	window.draw(present_sprite_gun);
	window.draw(gun_info_text);
	return;
}

// отображает на window боковую панель с информацией о враге
void Panel::draw_people_info(Enemy& enemy, __int64_t cur_time)
{
	enemy.start_presentation_animation(cur_time);
	// Заголовок (что мы смоотрим характеристики персонажа)
	title1_text.setString("Character characteristics");
	title1_text.setPosition(pos.left, 
								pos.top);
	// вычисляем его границы, чтобы знать куда размещять следующий
	sf::FloatRect last_obj_pos = title1_text.getGlobalBounds();
	// получаем спрайт для презентации
	sf::Sprite present_sprite_people = enemy.get_presentation_sprite(cur_time);
	// размещаем его
	present_sprite_people.setOrigin(0, 0);
	present_sprite_people.setPosition(last_obj_pos.left,
								last_obj_pos.top + last_obj_pos.height + 15.0f);
	// вычисляем позицию картинки на доске
	last_obj_pos = present_sprite_people.getGlobalBounds();
	// получаем информацию о enemy
	std::string people_info_string = "\tHealth:\t" + std::to_string(std::max(0, enemy.get_cur_health())) + "\n" +
						 			 "\tSpeed:\t"  + std::to_string(enemy.get_speed() / 10) + "\n" +
									 "\tVisibility range:\t" + std::to_string(enemy.get_distance()) + " cells\n" +
									 "\tCost:\t" + std::to_string(enemy.get_cost()) + '\n';

	// вставляем ее в sf::Text для представления
	people_info_text.setString(people_info_string);
	// позиционируем ее в info_panel
	people_info_text.setPosition(last_obj_pos.left,
								last_obj_pos.top + last_obj_pos.height);
	// вычисляем позицию предыдущего элемента
	last_obj_pos = people_info_text.getGlobalBounds();

	// Заголовок (что мы смоотрим характеристики оружия)
	title2_text.setString("Gun characteristics");
	title2_text.setPosition(last_obj_pos.left,
								last_obj_pos.top + last_obj_pos.height);
	last_obj_pos = title2_text.getGlobalBounds();
	// получаем спрайт презентации оружия
	sf::Sprite present_sprite_gun = enemy.get_gun_presentation_sprite(cur_time);
	present_sprite_gun.setOrigin(0, 0);
	// устанавливаем его в нужную позицию
	present_sprite_gun.setPosition(last_obj_pos.left,
									last_obj_pos.top + last_obj_pos.height);
	// обновляем позицию картинки на доске
	last_obj_pos = present_sprite_gun.getGlobalBounds();
	// получаем информацию о оружии
	std::string gun_info_str = "\tBullets:\t" + std::to_string(enemy.get_magazine_info().first) + "/" + std::to_string(enemy.get_magazine_info().second) + "\n" +
								"\tDamage:\t" + std::to_string(enemy.get_damage()) + "\n" +
								"\tSpread:\t" + std::to_string(static_cast<int>(enemy.get_spread())) + "deg\n" +
								"\tMagazines:\t" + std::to_string(enemy.get_gun_info()) + "\n" +
								"\tRecharge time:\t" + std::to_string(enemy.get_recharge_time() / 1000) + "sec.";
	gun_info_text.setString(gun_info_str);
	gun_info_text.setPosition(last_obj_pos.left,
									last_obj_pos.top + last_obj_pos.height);

	// рисуем задний фон боковой панели
	window.draw(panel_bg_sprite);

	window.draw(title1_text);
	window.draw(present_sprite_people);
	window.draw(people_info_text);

	window.draw(title2_text);
	window.draw(present_sprite_gun);
	window.draw(gun_info_text);
	return;
}


// отображает 
void Panel::draw_shop(const Hero& h, int cur_level)
{
    // рисуем задний фон боковой панели
	window.draw(panel_bg_sprite);

	// отображаем текущий уровень
	cur_level_text.setString("\t\tLevel:\t" + std::to_string(cur_level) + "\n");
	window.draw(cur_level_text);

	// отображаем cash
	cash_text.setString("\t\tYour balance:\t" + std::to_string(h.get_cash()));
	window.draw(cash_text);

	// получаем координаты мыши
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f globalMousePos = window.mapPixelToCoords(mousePos);

	bomb_btn.draw(window, globalMousePos);
	magazine_btn.draw(window, globalMousePos);
	recharge_btn.draw(window, globalMousePos);
	flame_arrester_btn.draw(window, globalMousePos);
	first_aid_kit_btn.draw(window, globalMousePos);
	max_health_btn.draw(window, globalMousePos);
	speed_btn.draw(window, globalMousePos);
	menu_btn.draw(window, globalMousePos);
}

// изменяет характеристики персонажа в соответсвии с выбранным улучшением
bool Panel::try_press_button(Hero& h)
{
	// получаем позицию мыши на доске
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f globalMousePos = window.mapPixelToCoords(mousePos);

    // пробуем нажать все доступные кнопки
    bomb_btn.try_to_press(globalMousePos);
    magazine_btn.try_to_press(globalMousePos);
    recharge_btn.try_to_press(globalMousePos);
    flame_arrester_btn.try_to_press(globalMousePos);
    first_aid_kit_btn.try_to_press(globalMousePos);
    max_health_btn.try_to_press(globalMousePos);
    speed_btn.try_to_press(globalMousePos);
    menu_btn.try_to_press(globalMousePos);

	bool result = false;
    // обрабатываем ввод
	if (bomb_btn.is_pressed())
	{
		// если хватает денег
		if (h.get_cash() >= costs::BOMB)
		{
			h.add_bomb();					// покупаем улучшение
			h.change_cash(-costs::BOMB);	// уменьшаем cash на цену бомбы
		}
		result = true;						// говорим что была нажата кнопка
	}
	else if(magazine_btn.is_pressed())
	{
		// если хватает денег
		if (h.get_cash() >= costs::MAGAZINE)
		{
			h.add_magazine();				// покупаем доп. магазин
			h.change_cash(-costs::MAGAZINE);// уменьшаем cash на цену магазина
		}
		result = true;						// говорим что была нажата кнопка
	}
	else if(recharge_btn.is_pressed())
	{
		// если хватает денег
		if (h.get_cash() >= costs::RECHARGE)
		{
			h.change_recharge_time(-200);	// уменьшаем время перезарядки на 200мили сек
			h.change_cash(-costs::RECHARGE);// уменьшаем cash на цену увеличения скорости перезарядки
		}
		result = true;						// говорим что была нажата кнопка
	}
	else if(flame_arrester_btn.is_pressed())
	{
		// если хватает денег
		if (h.get_cash() >= costs::FLAME_ARRESTER)
		{
			h.change_spread(-5.0f); 		// уменьшает разброс на 5 градусов
			h.change_cash(-costs::FLAME_ARRESTER);// уменьшаем cash на цену уменьшения перезарядки
		}
		result = true;						// говорим что была нажата кнопка
	}
	else if(first_aid_kit_btn.is_pressed())
	{
		// если хватает денег
		if (h.get_cash() >= costs::FIRST_AID_KIT)
		{
			h.update_cur_health();			// делает текущее здоровье равноым максимально доступному
			h.change_cash(-costs::FIRST_AID_KIT);// уменьшаем cash на цену аптечки
		}
		result = true;						// говорим что была нажата кнопка
	}
	else if(max_health_btn.is_pressed())
	{
		// если хватает денег
		if (h.get_cash() >= costs::MAX_HEALTH)
		{
			h.change_max_health(10);		// увеличивает максимальное здоровье на 10 ед
			h.change_cash(-costs::MAX_HEALTH);// уменьшаем cash на цену аптечки
		}
		result = true;						// говорим что была нажата кнопка
	}
	else if(speed_btn.is_pressed())
	{
		// если хватает денег
		if (h.get_cash() >= costs::SPEED)
		{
			h.change_speed(10); 				// увеличивает скорость на 10 ед
			h.change_cash(-costs::SPEED);// уменьшаем cash на цену аптечки
		}
		result = true;						// говорим что была нажата кнопка
	}
	else if(menu_btn.is_pressed())
	{
		h.suicide();
		result = true;						// говорим что была нажата кнопка
	}
	return result;
}

// инициализирует info panel с кнопами
void Panel::button_init(void)
{
	// резервируем место для вывода cur_level
	cur_level_text.setPosition(pos.left, 15.0f);
	// резервируем место для вывода cash 
	cash_text.setPosition(pos.left, 50.0f);
	// позиционируем кнопки
	sf::FloatRect last_obj_pos(pos.left, 50.0f, pos.width, 50.0f);
	bomb_btn = Button(sf::FloatRect(last_obj_pos.left, last_obj_pos.top + last_obj_pos.height + 15.0f, last_obj_pos.width, last_obj_pos.height), "Bomb:\t" + std::to_string(costs::BOMB));
	last_obj_pos = bomb_btn.get_hitbox();

	magazine_btn = Button(sf::FloatRect(last_obj_pos.left, last_obj_pos.top + last_obj_pos.height + 15.0f, last_obj_pos.width, last_obj_pos.height), "Magazine:\t" + std::to_string(costs::MAGAZINE));
	last_obj_pos = magazine_btn.get_hitbox();

	recharge_btn = Button(sf::FloatRect(last_obj_pos.left, last_obj_pos.top + last_obj_pos.height + 15.0f, last_obj_pos.width, last_obj_pos.height), "Recharge:\t" + std::to_string(costs::RECHARGE));
	last_obj_pos = recharge_btn.get_hitbox();

	flame_arrester_btn = Button(sf::FloatRect(last_obj_pos.left, last_obj_pos.top + last_obj_pos.height + 15.0f, last_obj_pos.width, last_obj_pos.height), "Flame arrester:\t" + std::to_string(costs::FLAME_ARRESTER));
	last_obj_pos = flame_arrester_btn.get_hitbox();

	first_aid_kit_btn = Button(sf::FloatRect(last_obj_pos.left, last_obj_pos.top + last_obj_pos.height + 15.0f, last_obj_pos.width, last_obj_pos.height), "First aid kit:\t" + std::to_string(costs::FIRST_AID_KIT));
	last_obj_pos = first_aid_kit_btn.get_hitbox();

	max_health_btn = Button(sf::FloatRect(last_obj_pos.left, last_obj_pos.top + last_obj_pos.height + 15.0f, last_obj_pos.width, last_obj_pos.height), "Max health:\t" + std::to_string(costs::MAX_HEALTH));
	last_obj_pos = max_health_btn.get_hitbox();

	speed_btn = Button(sf::FloatRect(last_obj_pos.left, last_obj_pos.top + last_obj_pos.height + 15.0f, last_obj_pos.width, last_obj_pos.height), "Speed:\t" + std::to_string(costs::SPEED));
	last_obj_pos = speed_btn.get_hitbox();

	menu_btn = Button(sf::FloatRect(last_obj_pos.left, last_obj_pos.top + last_obj_pos.height + 150.0f, last_obj_pos.width, last_obj_pos.height), "Menu");
	last_obj_pos = menu_btn.get_hitbox();
}

void Panel::settings_text(sf::Text& text, const sf::Color& color, int size)
{
	text.setFont(font);
	text.setCharacterSize(size);
    text.setFillColor(color);
}