#include "BoardNavigatorTest.hpp"
void BoardNavigatorTest::SetUp() {
    board.n = 5; board.m = 5; // устанавливаем размер доски
    board.was.clear();  // очищаем данные из предыдущих запусков
    board.graph.clear();
    
    // Заполняем граф данными для тестирования
    // Пример: создаем простой путь от (0,0) до (2,2)
    board.graph[{0, 0}].push_back({0, 1});
    board.graph[{0, 1}].push_back({0, 2});
    board.graph[{0, 2}].push_back({1, 2});
    board.graph[{1, 2}].push_back({2, 2});
}

// Тест №3.1: Позитивный случай нахождения пути между двумя вершинами
TEST_F(BoardNavigatorTest, FindPathBetweenVertices) {
    sf::Vector2i from(0, 0);
    sf::Vector2i to(2, 2);
    int max_depth = 10;

    auto path = board.navigator(from, to, max_depth);

    std::vector<std::pair<int, int>> expected_path = {{0, 1}, {0, 2}, {1, 2}, {2, 2}};
    ASSERT_EQ(path, expected_path);
}

// Тест №3.2: Позитивный случай ограничения глубины поиска
TEST_F(BoardNavigatorTest, DepthLimit) {
    sf::Vector2i from(0, 0);
    sf::Vector2i to(2, 2);
    int max_depth = 2;

    auto path = board.navigator(from, to, max_depth);

    // Поскольку максимальная глубина равна 2, полный путь не должен быть найден
    ASSERT_TRUE(path.empty());
}

// Тест №3.3: Негативный случай отсутствия пути
TEST_F(BoardNavigatorTest, NoPathExists) {
    sf::Vector2i from(0, 0);
    sf::Vector2i to(4, 4); // предполагаем, что пути до этой точки нет
    int max_depth = 10;

    auto path = board.navigator(from, to, max_depth);

    // Поскольку пути нет, должен быть возвращен пустой список
    ASSERT_TRUE(path.empty());
}
