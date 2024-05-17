#include "Enemy.hpp"

Enemy::Enemy(std::size_t num_model)
:People(100, 100),
saw_the_player(false),
distance(distances[num_model]), // дальность обнаружения distance клеток
cost(costs[num_model])			// цена за убийство cost монет
{
	// устанавливаем анимацию
	People::set_animation(
		Animation(textures[num_model][0], 100, 100, 1, 0, true),
		Animation(textures[num_model][1], 100, 100, 2, 500, false),
		Animation(textures[num_model][2], 100, 100 / 2, 1, 1000, false),
		Animation(textures[num_model][3], 100, 100, 1, 0, true),
		Animation(textures[num_model][4], 250, 250, 2, 500, false)
	);
}

bool Enemy::loadResources(void)
{
	// для первой модели врага
	textures.resize(1);
	sf::Texture tex1, tex2, tex3, tex4, tex5;
	bool success = true;

	if (!tex1.loadFromFile("src/enemy1.png"))
		success = false;

	if (!tex2.loadFromFile("src/enemy1_walking.png"))
		success = false;

	if (!tex3.loadFromFile("src/blood_spatter.png"))
		success = false;

	if (!tex4.loadFromFile("src/dead.png"))
		success = false;

	if (!tex5.loadFromFile("src/enemy1_presentation.png"))
		success = false;

	textures[0] = std::vector<sf::Texture>({tex1, tex2, tex3, tex4, tex5});
	distances.push_back(5); // для num_model = 0 обнаружение 5 клеток
	costs.push_back(2); // для num_model = 0 цена 2 монеты

	// для второй модели врага
	textures.resize(2);
	if (!tex1.loadFromFile("src/enemy2.png"))
		success = false;

	if (!tex2.loadFromFile("src/enemy2_walking.png"))
		success = false;

	if (!tex3.loadFromFile("src/blood_spatter.png"))
		success = false;

	if (!tex4.loadFromFile("src/dead.png"))
		success = false;

	if (!tex5.loadFromFile("src/enemy2_presentation.png"))
		success = false;

	textures[1] = std::vector<sf::Texture>({tex1, tex2, tex3, tex4, tex5});
	distances.push_back(3); // для num_model = 1 обнаружение 3 клеток
	costs.push_back(3); // для num_model = 1 цена 3 монеты

	return success;
}

// загружает путь до hero
void Enemy::set_path_to_hero(std::vector<std::pair<int, int>> path_to_hero)
{
	this->path_to_hero = path_to_hero;
}
// загружает текущюю позицию на карте
void Enemy::set_cooridinate(const sf::Vector2i& cor)
{
	this->cur_cor = cor;
}
// устанавливает координаты героя
void Enemy::set_hero_pos(const sf::FloatRect& hero_pos)
{
	this->hero_pos = get_mid_dot(hero_pos);
}

void Enemy::aim(void)
{
	// Далее оочень важно что People::aim принимает не направление а точку на доске, поэтому 
	// нужно откладывать вектор от текущей координаты enemy
	sf::Vector2f mid_of_self = get_mid_dot(pos);
	if (is_open_pozition())	// если мы находимся на одной линии с hero поворачиваемся к нему
	{
		People::aim(mid_of_self.x + hero_pos.x - mid_of_self.x, 
			mid_of_self.y + hero_pos.y - mid_of_self.y);
	}
	else   	// иначе разворачиваем enemy в кратчайшем направлении к hero
	{
		// умножение на 100 нужно, чтобы прин нормализации истинное направление не сильно уменьшилось
		sf::Vector2i direct(mid_of_self.x + (path_to_hero[0].second - cur_cor.y) * 100,
			mid_of_self.y + (path_to_hero[0].first - cur_cor.x) * 100);
		People::aim(direct.x, direct.y);
	}
}

void Enemy::shooting(__int64_t cur_time)
{
	if (is_open_pozition()) // если находимся на одной линии с hero
	{	
		People::shooting(cur_time);	// стреляем
	}
}

void Enemy::moving(__int64_t cur_time)
{

	if (alive())
	{
		if (path_to_hero.size() == 0){ // если мы находимся в одной клетке c hero
			saw_the_player = true;	// отмчаем, что видели его
			
			sf::Vector2f coordinates_enemy = get_mid_dot(hitbox);
			// направление движения это вектор между их центрами
			dx = hero_pos.x - coordinates_enemy.x;
			dy = hero_pos.y - coordinates_enemy.y;

			// нормализуем этот вектор
			float len_direct = std::hypot(dx, dy);
			dx = dx / len_direct;
			dy = dy / len_direct;

			// перемещаем
			People::moving(cur_time, dx, dy);
		}

		// если видели игорка или он в первый раз вошел в зону видимости
		else if (static_cast<int>(path_to_hero.size()) <= distance || saw_the_player){
			saw_the_player = true;
			// вычисляем вектор по клеточкам
			std::pair<int, int> target = path_to_hero[0];
			dx = target.second - cur_cor.y;
			dy = target.first - cur_cor.x;
			// перемещаем персонажа
			People::moving(cur_time, dx, dy);
		}
		else
		{
			People::moving(cur_time, 0, 0);
		}
	}
	else 	// если персонаж мертв передвигаем его пули
	{
		gun.move_bullets(cur_time);
	}

}

// считает координаты сережины объекта
sf::Vector2f Enemy::get_mid_dot(const sf::FloatRect& rec)
{
	return sf::Vector2f((rec.left + rec.width / 2), (rec.top + rec.height / 2));
}

// проверяет путь до hero, возращает true если enemy его видит и false иначе
bool Enemy::is_open_pozition(void)
{
	if (path_to_hero.size() == 0)
		return true;

	bool on_one_line = true;

	for (auto i : path_to_hero)
	{
		if (i.first != cur_cor.x)
		{
			on_one_line = false;
			break;
		}
	}
	if (on_one_line) return true;
	on_one_line = true;
	for (auto i : path_to_hero){
		if (i.second != cur_cor.y){
			on_one_line = false;
			break;
		}
	}
	if (on_one_line) return true;

	return false;
}

int Enemy::get_cost(void) const
{
	return cost;
}

int Enemy::get_distance(void) const
{
	return distance;
}

std::vector<std::vector<sf::Texture>> Enemy::textures;
std::vector<int> Enemy::distances;
std::vector<int> Enemy::costs;