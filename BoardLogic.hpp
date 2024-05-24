#ifndef BOARD_LOGIC_HPP
#define BOARD_LOGIC_HPP

#include "includes.hpp"

class BoardLogic
{
public:
	// генерирует рандомный путь и записывает его в graph
	void dfs(int w=0, int h=0);

	// Возращает кратчайший путь между двумя точками в лабиринте
	std::vector<std::pair<int, int>> navigator(const sf::Vector2i& index_from, const sf::Vector2i& index_to, int max_depth=0) const;

	// Проверяет выход за границы поля
	bool is_go_abroad(int w, int h) const;

	std::map<std::pair<int, int>, std::vector<std::pair<int, int>>> graph;
	std::set<std::pair<int, int>> was;
	std::size_t n, m;
};

#endif