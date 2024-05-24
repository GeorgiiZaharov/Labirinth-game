#include "BoardDFSTest.hpp"

void BoardDFSTest::SetUp() {
    board.n = 5; board.m = 5; // устанавливаем размер доски
    board.was.clear();  // очищаем данные из предыдущих запусков
    board.graph.clear();
}

// Тестирование метода dfs на корректный выход из алгоритма
TEST_F(BoardDFSTest, CorrectAlgorithmTermination) {
    // Вызываем метод dfs с координатами начальной клетки
    board.dfs(0, 0);

    // Проверяем, что размер graph равен n*m
    ASSERT_EQ(board.graph.size(), board.n * board.m);
}

// Тестирование добавления вершин в граф при проходе алгоритма не из начальной клетки
TEST_F(BoardDFSTest, GraphVerticesAddedCorrectly) {
    // Вызываем метод dfs с координатами не начальной клетки
    board.dfs(1, 1);

    // Проверяем, что размер graph равен n*m
    ASSERT_EQ(board.graph.size(), board.n * board.m);
}

// Тестирование обработки некорректных координат
TEST_F(BoardDFSTest, IncorrectCoordinatesHandling) {
    // Вызываем метод dfs с некорректными координатами
    // Например, передаем координаты за пределами границ доски
    board.dfs(-1, -1);

    ASSERT_EQ(board.graph.size(), 0);
}
