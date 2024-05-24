#include "BoardLogic.hpp"

void BoardLogic::dfs(int w, int h)
{
	if (is_go_abroad(w, h)) return; // если поступили некорректные координаты прекращаем построение
    was.insert(std::make_pair(w, h)); // отметились что побывали
    while (true) {
        std::vector<std::pair<int, int>> v; // находим все варианты куда можем пойти
        if (!is_go_abroad(w, h + 1) && !was.count(std::make_pair(w, h + 1))) v.push_back(std::make_pair(0, 1));
        if (!is_go_abroad(w + 1, h) && !was.count(std::make_pair(w + 1, h))) v.push_back(std::make_pair(1, 0));
        if (!is_go_abroad(w, h - 1) && !was.count(std::make_pair(w, h - 1))) v.push_back(std::make_pair(0, -1));
        if (!is_go_abroad(w - 1, h) && !was.count(std::make_pair(w - 1, h))) v.push_back(std::make_pair(-1, 0));

        if (v.size() == 0) break; // были везде в округе

        int choice = random() % v.size(); // выбираем куда идти
        int delta_w = v[choice].first, delta_h = v[choice].second; // запоминаем направление

        graph[std::make_pair(h, w)].push_back(std::make_pair(h + delta_h, w + delta_w)); // добавляем вершины в граф
        graph[std::make_pair(h + delta_h, w + delta_w)].push_back(std::make_pair(h, w)); // добавляем вершины в граф

        v.clear();
        dfs(w + delta_w, h + delta_h);
    }
}

std::vector<std::pair<int, int>> BoardLogic::navigator(const sf::Vector2i& index_from, const sf::Vector2i& index_to, int max_depth) const
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
		if (graph.find(from.first) != graph.end())	// если есть такой элемент
		{
			for (std::pair<int, int> i : graph.at(from.first)){	 //смотрим все варианты куда можем идти
				if (!was.count(i)){		//здесь еще не были
					was[i] = from.first;		//запоминаем откуда мы сюда попали
					q.push(std::make_pair(i, from.second + 1));			//добавляем в очередь
				}
				if (i == std::make_pair(index_to.x, index_to.y)){
					flag = true;			//если нашли то прекращаем поиск
					break;
				}
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

bool BoardLogic::is_go_abroad(int w, int h) const
{
	if (w < 0 || h < 0 || w >= static_cast<int>(n) || h >= static_cast<int>(m)) return true;
	return false;
}
