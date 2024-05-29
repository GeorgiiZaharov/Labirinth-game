#include "Board.hpp"

Board::Board(const sf::FloatRect& pos)
:pos(pos)
{
	n = 0; m = 0; // ширина и высота лабиринта в клетках
	border_width = 10.0f; // ширина стен лабиринта
	if (!Cell::loadResources())	// загружаем ресурсы для клетки
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
	cell_w = pos.width / static_cast<float>(n);
	cell_h = pos.height / static_cast<float>(m); 

	// в соответствии с размером заполняем cells объектами cell
	for (std::size_t i = 0; i < m; ++i){
		std::vector<Cell> tmp_cells;
		tmp_cells.resize(n);
		for (std::size_t j = 0; j < n; ++j){
			tmp_cells[j] = Cell(bg_tex, border_tex, pos.left + j * cell_w, pos.top + i * cell_h, cell_w, cell_h, border_width);
		}
		cells.push_back(tmp_cells);
	}
	// генерируем рандомный граф
	dfs(0, 0);
	// согласно этому графу удаляем ненужные стены
	destroy_cell_borders();
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

std::vector<std::pair<int, int>> Board::navigator(const sf::Vector2i& index_from, const sf::Vector2i& index_to, int max_depth) const
{
	return BoardLogic::navigator(index_from, index_to, max_depth);
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

void Board::boom_cell(int x, int y, __int64_t cur_time)
{
	cells[x][y].make_boom(cur_time);
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

std::vector<std::pair<int, int>> Board::neighbour(const sf::FloatRect& obj_pos) const
{
	// получаем координаты ячейки в которой находится объект
	sf::Vector2i cell_cor = calc_pos_on_board(obj_pos);
	// Создаем множество для хранения уникальных пар
    std::set<std::pair<int, int>> unique_pairs(graph.at(std::make_pair(cell_cor.x, cell_cor.y)).begin(), graph.at(std::make_pair(cell_cor.x, cell_cor.y)).end());

    // Создаем новый вектор и заполняем его уникальными парами из множества
    std::vector<std::pair<int, int>> output(unique_pairs.begin(), unique_pairs.end());

    // добавляем и текущюю ячеку тудаже
    output.push_back(std::make_pair(cell_cor.x, cell_cor.y));

    return output;
}

sf::Vector2i Board::calc_pos_on_board(const sf::FloatRect& obj_pos) const
{
	float x = obj_pos.left + obj_pos.width / 2;
	float y = obj_pos.top + obj_pos.height / 2;
	return sf::Vector2i(static_cast<int>((y - pos.top) / cell_h), 
						static_cast<int>((x - pos.left) / cell_w));
}

sf::Vector2i Board::calc_pos_on_board(const sf::FloatRect& obj_pos, const sf::Vector2f& direct) const
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

float Board::get_people_size(void) const
{
	return std::min(cell_w, cell_h) * 1.0f / 3;
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

void Board::destroy_cell_borders(void)
{
    std::queue<std::pair<int, int>> q; // Очередь для обхода в ширину (BFS)
    std::set<std::pair<int, int>> visited; // Множество для хранения посещенных узлов

    // Начинаем обход с клетки (0, 0)
    q.push(std::make_pair(0, 0)); 
    visited.insert(std::make_pair(0, 0)); // Добавляем стартовую клетку в множество посещенных

    // Пока очередь не пуста
    while (!q.empty()) {
        auto current = q.front(); // Берем первый элемент из очереди
        q.pop(); // Удаляем его из очереди

        int h = current.first; // Текущая строка
        int w = current.second; // Текущий столбец

        // Проходим по всем соседям текущей клетки, которые есть в графе
        for (const auto& neighbor : graph[current]) {
            int nh = neighbor.first; // Строка соседа
            int nw = neighbor.second; // Столбец соседа

            // Если сосед еще не был посещен
            if (!visited.count(neighbor)) {
                visited.insert(neighbor); // Добавляем соседа в множество посещенных
                q.push(neighbor); // Добавляем соседа в очередь для дальнейшего обхода

                int delta_h = nh - h; // Разница по строкам между текущей клеткой и соседом
                int delta_w = nw - w; // Разница по столбцам между текущей клеткой и соседом

                // Удаляем стены между текущей клеткой и соседом в зависимости от направления
                if (delta_h == 1) { // Сосед находится снизу
                    cells[h][w].is_down = false;
                    cells[nh][nw].is_up = false;
                } else if (delta_w == 1) { // Сосед находится справа
                    cells[h][w].is_right = false;
                    cells[nh][nw].is_left = false;
                } else if (delta_h == -1) { // Сосед находится сверху
                    cells[h][w].is_up = false;
                    cells[nh][nw].is_down = false;
                } else if (delta_w == -1) { // Сосед находится слева
                    cells[h][w].is_left = false;
                    cells[nh][nw].is_right = false;
                }
            }
        }
    }
}

sf::Texture Board::bg_tex, Board::border_tex;