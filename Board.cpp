#include "Board.hpp"

Board::Board(const sf::FloatRect& pos)
:n(),//ширина
m(),// высота
screen_w(pos.width),
screen_h(pos.height),
border_width(10.0f),
pos(pos)
{
	if (!Cell::loadResources())
		std::cout << "CELL load resources error" << std::endl;
}

bool Board::loadResources(void)
{
	bool success = true;

    if (!bg_tex.loadFromFile("src/bg_cell.png"))
        success = false;

    if (!border_tex.loadFromFile("src/border_cell.png"))
        success = false;

    return success;
}

void Board::build(void)
{
	// чистим массивы с устаревшими данными
	was.clear();
	graph.clear();
	cells.clear();

	// в соответствии с размером поля вычисляем размеры ячейки
	cell_w = static_cast<float>(pos.width) / n;
	cell_h = static_cast<float>(pos.height) / m; 

	// в соответствии с размером заполняем cells объектами cell
	for (std::size_t i = 0; i < m; ++i){
		std::vector<Cell> tmp_cells;
		tmp_cells.resize(n);
		for (std::size_t j = 0; j < n; ++j){
			tmp_cells[j] = Cell(bg_tex, border_tex, pos.left + j * cell_w, pos.top + i * cell_h, cell_w, cell_h, border_width);
		}
		cells.push_back(tmp_cells);
	}
	// генерируем рандомный лабиринт
	dfs(0, 0);
}
void Board::dfs(int w, int h)
{
	if (is_go_abroad(w, h)) return;			// если поступили некорректные координаты прекращаем построение
	was.insert(std::make_pair(w, h));		// отметились что побывали
	while (true)
	{	
		std::vector<std::pair<int, int>> v;			// находим все варианты куда можем пойти
		if (!is_go_abroad(w, h + 1) && !was.count(std::make_pair(w, h + 1))) v.push_back(std::make_pair(0, 1));
		if (!is_go_abroad(w + 1, h) && !was.count(std::make_pair(w + 1, h))) v.push_back(std::make_pair(1, 0));
		if (!is_go_abroad(w, h - 1) && !was.count(std::make_pair(w, h - 1))) v.push_back(std::make_pair(0, -1));
		if (!is_go_abroad(w - 1, h) && !was.count(std::make_pair(w - 1, h))) v.push_back(std::make_pair(-1, 0));

		if (v.size() == 0) break;		//были везде в округе

		int choice = random() % v.size();		// выбираем куда идти
		int delta_w = v[choice].first, delta_h = v[choice].second;		// запоминаем направление

		this->graph[std::make_pair(h, w)].push_back(std::make_pair(h + delta_h, w + delta_w));	// добавляем вершины в граф
		this->graph[std::make_pair(h + delta_h, w + delta_w)].push_back(std::make_pair(h, w));	// добавляем вершины в граф

		// в зависимости от направления удаляем стенки следующего и текущего
		if (delta_h == 1)
		{
			cells[h + delta_h][w + delta_w].is_up = false;
			cells[h][w].is_down = false;
		}
		else if (delta_w == 1)
		{
			cells[h + delta_h][w + delta_w].is_left = false;
			cells[h][w].is_right = false;
		}
		else if (delta_h == -1)
		{
			cells[h + delta_h][w + delta_w].is_down = false;
			cells[h][w].is_up = false;
		}
		else if (delta_w == -1)
		{
			cells[h + delta_h][w + delta_w].is_right = false;
			cells[h][w].is_left = false;
		}
		v.clear();

		dfs(w + delta_w, h + delta_h);
	}
}


float Board::get_people_size(void) const
{
	return std::min(cell_w, cell_h) * 1.0f / 4;
}

sf::Vector2f Board::get_cell_size(void) const
{
	return sf::Vector2f(cell_w, cell_h);
}
void Board::set_board_size(std::size_t board_width, std::size_t board_height)
{
	n = board_width;
	m = board_height;
}

bool Board::is_go_abroad(int w, int h) const
{
	if (w < 0 || h < 0 || w >= static_cast<int>(n) || h >= static_cast<int>(m)) return true;
	return false;
}

std::vector<std::pair<int, int>> Board::navigator(const sf::Vector2i& index_from, const sf::Vector2i& index_to, int max_depth)
{
	bool flag = false;		// флаг того найден путь или еще нет
	std::pair<std::pair<int, int>, int> from;	// хранится родительская вершина и текущяя глубина
	std::map<std::pair<int, int>, std::pair<int, int>> was;	// откуда мы пришли в эту вершину
	was[std::make_pair(index_from.x, index_from.y)] = std::make_pair(-1, -1);
	std::queue<std::pair<std::pair<int, int>, int>> q;		// <позиция <int, int>, глубина int>
	q.push(std::make_pair(std::make_pair(index_from.x, index_from.y), 0));

	std::vector<std::pair<int, int>> result;

	while (!q.empty()){

		from = q.front();			//получаем вершину с которой будем работать
		q.pop();
		if (max_depth != 0 && from.second >= max_depth)
		{
			break;
		}
		for (std::pair<int, int> i : graph[from.first]){	 //смотрим все варианты куда можем идти
			if (!was.count(i)){		//здесь еще не были
				was[i] = from.first;		//запоминаем откуда мы сюда попали
				q.push(std::make_pair(i, from.second + 1));			//добавляем в очередь
			}
			if (i == std::make_pair(index_to.x, index_to.y)){
				flag = true;			//если нашли то прекращаем поиск
				break;
			}
		}
		if (flag) break;
	}
	if (!flag) return result;
	//собираем путь от позиции to до позиции from (невключительно)
	std::pair<int, int> cur = std::make_pair(index_to.x, index_to.y);
	while (cur != std::make_pair(index_from.x, index_from.y)){
		result.push_back(cur);
		cur = was[cur];
	}

	//разворачиваем получившийся путь
	reverse(std::begin(result), std::end(result));
	return result;
}


void Board::draw(sf::RenderWindow& window, __int64_t cur_time)
{
	for (std::size_t i = 0; i < m; ++i)
	{
		for (std::size_t j = 0; j < n; ++j)
		{
			cells[i][j].draw(window, cur_time);
		}
	}
}

bool Board::check_collision(const sf::FloatRect& r) const
{
	// получаем индексы ячейки в которой находится r
	int cor_x = static_cast<int>((r.top - pos.top) / cell_h), cor_y = static_cast<int>((r.left - pos.left) / cell_w);
	// проверяем если мв не выходим и есть пересечение => коллизия
	if (!is_go_abroad(cor_y, cor_x) && cells[cor_x][cor_y].check_collision(r)) 		   return true;
	if (!is_go_abroad(cor_y + 1, cor_x) && cells[cor_x][cor_y + 1].check_collision(r)) return true;
	if (!is_go_abroad(cor_y, cor_x + 1) && cells[cor_x + 1][cor_y].check_collision(r)) return true;
	if (!is_go_abroad(cor_y - 1, cor_x) && cells[cor_x][cor_y - 1].check_collision(r)) return true;
	if (!is_go_abroad(cor_y, cor_x - 1) && cells[cor_x - 1][cor_y].check_collision(r)) return true;
	return false;
}

void Board::check_collision(Bullet& b) const
{
	if (check_collision(b.getGlobalBounds()))
	{
		b.collision();
	}
}

sf::Vector2i Board::calc_pos_on_board(const sf::FloatRect& obj_pos)
{
	float x = obj_pos.left + obj_pos.width / 2;
	float y = obj_pos.top + obj_pos.height / 2;
	return sf::Vector2i(static_cast<int>((y - pos.top) / cell_h), 
						static_cast<int>((x - pos.left) / cell_w));
}

sf::Vector2i Board::calc_pos_on_board(const sf::FloatRect& obj_pos, const sf::Vector2f& direct)
{
	int poz_x = 0, poz_y = 0;
	// рассчитывем позицию изходя из предыдущего направления, чтобы он не ходил по краям клеток
	if (direct.y == -1.0f)
	{			//up
		poz_x = static_cast<int>((obj_pos.left + obj_pos.width / 2 - pos.left) / cell_w);
		poz_y = static_cast<int>((obj_pos.top + obj_pos.height / 2 + 1.f / 4 * cell_h - pos.top) / cell_h);
	}			
	else if (direct.x == 1.0f)
	{		//right
		poz_x = static_cast<int>((obj_pos.left + obj_pos.width / 2 - 1.f / 4 * cell_w - pos.left) / cell_w);
		poz_y = static_cast<int>((obj_pos.top + obj_pos.height / 2 - pos.top) / cell_h);
	}
	else if (direct.y == 1.0f)
	{		//down
		poz_x = static_cast<int>((obj_pos.left + obj_pos.width / 2 - pos.left) / cell_w);
		poz_y = static_cast<int>((obj_pos.top + obj_pos.height / 2 - 1.f / 4 * cell_h - pos.top) / cell_h);
	}
	else if (direct.x == -1.0f)
	{		//left
		poz_x = static_cast<int>((obj_pos.left + obj_pos.width / 2 + 1.f / 4 * cell_w - pos.left) / cell_w);
		poz_y = static_cast<int>((obj_pos.top + obj_pos.height / 2 - pos.top) / cell_h);
	}
	else
	{
		return calc_pos_on_board(obj_pos);
	}
	return sf::Vector2i(poz_y, poz_x);
}


void Board::wall_destroyer(const sf::FloatRect& obj_pos)
{
	// получаем координаты ячейки в которой находится объект
	sf::Vector2i cell_cor = calc_pos_on_board(obj_pos);
	// вычисляем центр прямоугольника
	float pos_x = obj_pos.left + obj_pos.width / 2;
	float pos_y = obj_pos.top + obj_pos.height / 2;
	// вычиcляем координаты относительно доски
	sf::Vector2i cor_on_board(static_cast<int>(pos_x - pos.left), static_cast<int>(pos_y - pos.top));
	// Вычисляем растояния до границ ячейки
	int dist_left = cor_on_board.x % static_cast<int>(cell_w), dist_up = cor_on_board.y % static_cast<int>(cell_h);
	int dist_right = cell_w - dist_left, dist_down = cell_h - dist_up;

	// вычисляем направление к смежной ближайшей ячейке
	sf::Vector2i direct(0, 0);
	if (dist_left <= dist_up && dist_left <= dist_down && dist_left <= dist_right) // если кратчайшее до левой
		direct.y -= 1;
	else if (dist_right <= dist_left && dist_right <= dist_up && dist_right <= dist_down) // если кратчайшее до правой
		direct.y += 1;
	else if (dist_up < dist_left && dist_up < dist_down && dist_up < dist_right) // если кратчайшее до верхей
		direct.x -= 1;
	else direct.x += 1;

	auto v_begin = graph[std::make_pair(cell_cor.x, cell_cor.y)].begin();
	auto v_end = graph[std::make_pair(cell_cor.x, cell_cor.y)].end();

	// удаляем стены которые лежат рядом с объектом
	if (direct.x == 1 && cell_cor.x + 1 != static_cast<int>(m)) // вниз и мы не на последнем ряду
	{
		// удаляем соответсвующие стенки
		cells[cell_cor.x][cell_cor.y].is_down = false;
		cells[cell_cor.x + 1][cell_cor.y].is_up = false;
		// добавляем новый путь в graph, если он еще не там
		if (std::find(v_begin, v_end, std::make_pair(cell_cor.x + 1, cell_cor.y)) == v_end)
		{
			graph[std::make_pair(cell_cor.x, cell_cor.y)].push_back(std::make_pair(cell_cor.x + 1, cell_cor.y));
			graph[std::make_pair(cell_cor.x + 1, cell_cor.y)].push_back(std::make_pair(cell_cor.x, cell_cor.y));
		}
	}
	else if (direct.x == -1 && cell_cor.x != 0) // вверх и мы не на верхнем ряду
	{
		// удаляем соответсвующие стенки
		cells[cell_cor.x][cell_cor.y].is_up = false;		
		cells[cell_cor.x - 1][cell_cor.y].is_down = false;
		// добавляем новый путь в graph, если он еще не там
		if (std::find(v_begin, v_end, std::make_pair(cell_cor.x - 1, cell_cor.y)) == v_end)
		{
			graph[std::make_pair(cell_cor.x, cell_cor.y)].push_back(std::make_pair(cell_cor.x - 1, cell_cor.y));
			graph[std::make_pair(cell_cor.x - 1, cell_cor.y)].push_back(std::make_pair(cell_cor.x, cell_cor.y));
		}
	}
	else if (direct.y == 1 && cell_cor.y + 1 != static_cast<int>(n)) // вправо и мы не в крайней правой колонке
	{
		// удаляем соответсвующие стенки
		cells[cell_cor.x][cell_cor.y].is_right = false;
		cells[cell_cor.x][cell_cor.y + 1].is_left = false;
		// добавляем новый путь в graph, если он еще не там
		if (std::find(v_begin, v_end, std::make_pair(cell_cor.x, cell_cor.y + 1)) == v_end)
		{
			graph[std::make_pair(cell_cor.x, cell_cor.y)].push_back(std::make_pair(cell_cor.x, cell_cor.y + 1));
			graph[std::make_pair(cell_cor.x, cell_cor.y + 1)].push_back(std::make_pair(cell_cor.x, cell_cor.y));
		}
	}
	else if (direct.y == -1 && cell_cor.y != 0) // влево и мы не в крайней левой колонке
	{
		// удаляем соответсвующие стенки
		cells[cell_cor.x][cell_cor.y].is_left = false;
		cells[cell_cor.x][cell_cor.y - 1].is_right = false;
		// добавляем новый путь в graph, если он еще не там
		if (std::find(v_begin, v_end, std::make_pair(cell_cor.x, cell_cor.y - 1)) == v_end)
		{
			graph[std::make_pair(cell_cor.x, cell_cor.y)].push_back(std::make_pair(cell_cor.x, cell_cor.y - 1));
			graph[std::make_pair(cell_cor.x, cell_cor.y - 1)].push_back(std::make_pair(cell_cor.x, cell_cor.y));
		}
	}
}

void Board::boom_cell(int x, int y, __int64_t cur_time)
{
	cells[x][y].make_boom(cur_time);
}

std::vector<std::pair<int, int>> Board::neighbour(const sf::FloatRect& obj_pos)
{
	// получаем координаты ячейки в которой находится объект
	sf::Vector2i cell_cor = calc_pos_on_board(obj_pos);
	// Создаем множество для хранения уникальных пар
    std::set<std::pair<int, int>> unique_pairs(graph[std::make_pair(cell_cor.x, cell_cor.y)].begin(), graph[std::make_pair(cell_cor.x, cell_cor.y)].end());

    // Создаем новый вектор и заполняем его уникальными парами из множества
    std::vector<std::pair<int, int>> output(unique_pairs.begin(), unique_pairs.end());

    // добавляем и текущюю ячеку тудаже
    output.push_back(std::make_pair(cell_cor.x, cell_cor.y));

    return output;
}

sf::Texture Board::bg_tex, Board::border_tex;