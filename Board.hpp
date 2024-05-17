#ifndef BOARD_HPP
#define BOARD_HPP

#include "includes.hpp"

#include "Cell.hpp"
#include "Bullet.hpp"


class Board
{
public:
	/*
    * ===================================================
    * Конструкторы
    * ===================================================
    */
	explicit Board() = default;
	explicit Board(const sf::FloatRect& pos);

	/*
    * ===================================================
    * Методы
    * ===================================================
    */
    static bool loadResources(void);

    // Генерирует рандомный лабиринт
	void build(void);

	// Возращает кратчайший путь между двумя точками в лабиринте
	std::vector<std::pair<int, int>> navigator(const sf::Vector2i& index_from, const sf::Vector2i& index_to, int max_depth=0);
	
	// Отображает поле
	void draw(sf::RenderWindow& window, __int64_t cur_time);

	// Проверяет столкновение объекта со стенами лабиринта
	bool check_collision(const sf::FloatRect& r) const;

	// Проверяет столкновение пули со стенами лабиринта
	void check_collision(Bullet& b) const;

	// Уничтожает ближайшую к объекту сетну если такая есть
	void wall_destroyer(const sf::FloatRect& obj_pos);

	// Запускает анимацию взрыва на конкретной ячейке
	void boom_cell(int x, int y, __int64_t cur_time);

	// Возращает координаты ячеек соседей путь к которым отркыт
	std::vector<std::pair<int, int>> neighbour(const sf::FloatRect& obj_pos);

	// Возращает рекомендованный размер спрайта персонажа = 1/4 * min(w, h)
	float get_people_size(void) const;

	// Возращает ширину и высоту одной клетки
	sf::Vector2f get_cell_size(void) const;

	// устанавливает новый размер поля
	void set_board_size(std::size_t board_width, std::size_t board_height);

	// Для введенного прямоугольника рассчитывает позицию где он находится
	sf::Vector2i calc_pos_on_board(const sf::FloatRect& obj_pos);

	// Для введенного прямоугольника с направлением рассчитывает позицию где он находится
	sf::Vector2i calc_pos_on_board(const sf::FloatRect& obj_pos, const sf::Vector2f& direct);


private:
	// Проверяет выход за границы поля
	bool is_go_abroad(int w, int h) const;

	// нужно для построения лабиринта
	void dfs(int w=0, int h=0);
	std::size_t n, m;
	int screen_w, screen_h;
	float cell_w, cell_h, border_width;
	sf::FloatRect pos;
	std::vector<std::vector<Cell>> cells;
	std::map<std::pair<int, int>, std::vector<std::pair<int, int>>> graph;
	std::set<std::pair<int, int>> was;

	static sf::Texture bg_tex, border_tex;
};

#endif