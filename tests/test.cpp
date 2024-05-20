#include <gtest/gtest.h>
#include <SFML/Graphics.hpp>
#include "../Board.hpp"
#include "../Bullet.hpp"
#include "../Bomb.hpp"


#include <queue>
#include <map>
#include <algorithm>
/*
* =======================================================
*   Тест алгоритма навигации в лабиринте
* =======================================================
*/
// Класс для тестирования метода navigator класса Board
class BoardNavigationTest : public ::testing::Test {
protected:
    Board board;

    // Метод SetUp, который будет вызываться перед каждым тестом
    void SetUp() override {
        sf::FloatRect pos(0, 0, 1000, 1000); // задаем размер доски
        board = Board(pos);
        board.set_board_size(5, 5); // устанавливаем размер доски

        board.was.clear();
        board.graph.clear();
        board.cells.clear();

        board.cell_w = static_cast<float>(board.pos.width) / board.n;
        board.cell_h = static_cast<float>(board.pos.height) / board.m;

        // Заполняем cells объектами Cell в соответствии с размером доски
        for (std::size_t i = 0; i < board.m; ++i) {
            std::vector<Cell> tmp_cells;
            tmp_cells.resize(board.n);
            for (std::size_t j = 0; j < board.n; ++j) {
                tmp_cells[j] = Cell(board.bg_tex, board.border_tex, board.pos.left + j * board.cell_w, board.pos.top + i * board.cell_h, board.cell_w, board.cell_h, board.border_width);
            }
            board.cells.push_back(tmp_cells);
        }
        
        // Заполняем граф данными для тестирования
        // Пример: создаем простой путь от (0,0) до (2,2)
        board.graph[{0, 0}].push_back({0, 1});
        board.graph[{0, 1}].push_back({0, 2});
        board.graph[{0, 2}].push_back({1, 2});
        board.graph[{1, 2}].push_back({2, 2});
    }
};

// Тест №3.1: Позитивный случай нахождения пути между двумя вершинами
TEST_F(BoardNavigationTest, FindPathBetweenVertices) {
    sf::Vector2i from(0, 0);
    sf::Vector2i to(2, 2);
    int max_depth = 10;

    auto path = board.navigator(from, to, max_depth);

    std::vector<std::pair<int, int>> expected_path = {{0, 1}, {0, 2}, {1, 2}, {2, 2}};
    ASSERT_EQ(path, expected_path);
}

// Тест №3.2: Позитивный случай ограничения глубины поиска
TEST_F(BoardNavigationTest, DepthLimit) {
    sf::Vector2i from(0, 0);
    sf::Vector2i to(2, 2);
    int max_depth = 2;

    auto path = board.navigator(from, to, max_depth);

    // Поскольку максимальная глубина равна 2, полный путь не должен быть найден
    ASSERT_TRUE(path.empty());
}

// Тест №3.3: Негативный случай отсутствия пути
TEST_F(BoardNavigationTest, NoPathExists) {
    sf::Vector2i from(0, 0);
    sf::Vector2i to(4, 4); // предполагаем, что пути до этой точки нет
    int max_depth = 10;

    auto path = board.navigator(from, to, max_depth);

    // Поскольку пути нет, должен быть возвращен пустой список
    ASSERT_TRUE(path.empty());
}
/*
* =======================================================
* Тест алгоритма построения лабиринта
* =======================================================
*/

// Класс для тестирования приватных методов класса Board
class BoardDFSTest : public ::testing::Test {
protected:
    // Создаем экземпляр класса Board для тестирования
    Board board;

    void call_dfs(int w=0, int h=0)
    {
        board.dfs(w, h);
    }

    std::size_t get_graph_size()
    {
        return board.graph.size();
    }

    // Переопределяем метод SetUp(), который будет вызываться перед каждым тестом
    void SetUp() override {

        sf::FloatRect pos(0, 0, 1000, 1000); // задаем ему размер (необходимо для конструктора)
        board = Board(pos);
        board.set_board_size(5, 5);
        // чистим массивы с устаревшими данными
        board.was.clear();
        board.graph.clear();
        board.cells.clear();

        // в соответствии с размером поля вычисляем размеры ячейки
        board.cell_w = static_cast<float>(board.pos.width) / board.n;
        board.cell_h = static_cast<float>(board.pos.height) / board.m; 

        // в соответствии с размером заполняем cells объектами cell
        for (std::size_t i = 0; i < board.m; ++i){
            std::vector<Cell> tmp_cells;
            tmp_cells.resize(board.n);
            for (std::size_t j = 0; j < board.n; ++j){
                tmp_cells[j] = Cell(board.bg_tex, board.border_tex, board.pos.left + j * board.cell_w, board.pos.top + i * board.cell_h, board.cell_w, board.cell_h, board.border_width);
            }
            board.cells.push_back(tmp_cells);
        } 
    }
};

// Тестирование метода dfs на корректный выход из алгоритма
TEST_F(BoardDFSTest, CorrectAlgorithmTermination) {
    // Вызываем метод dfs с координатами начальной клетки
    call_dfs(0, 0);

    // Проверяем, что размер graph равен n*m
    ASSERT_EQ(get_graph_size(), board.n * board.m);
}

// Тестирование добавления вершин в граф при проходе алгоритма не из начальной клетки
TEST_F(BoardDFSTest, GraphVerticesAddedCorrectly) {
    // Вызываем метод dfs с координатами не начальной клетки
    call_dfs(1, 1);

    // Проверяем, что размер graph равен n*m
    ASSERT_EQ(get_graph_size(), board.n * board.m);
}

// Тестирование обработки некорректных координат
TEST_F(BoardDFSTest, IncorrectCoordinatesHandling) {
    // Вызываем метод dfs с некорректными координатами
    // Например, передаем координаты за пределами границ доски
    call_dfs(-1, -1);

    ASSERT_EQ(get_graph_size(), 0);
}
/*
* =======================================================
* Тест алгоритма установки бомбы
* =======================================================
*/
class BombTest : public ::testing::Test {
protected:
    Bomb bomb;
    sf::FloatRect initial_pos;
    __int64_t initial_time;

    void SetUp() override {
        initial_pos = sf::FloatRect(0, 0, 50, 50);
        initial_time = 1000; // Начальное время в миллисекундах
        bomb = Bomb(10, 10); // Инициализируем объект бомбы с шириной и высотой
    }
};

// Тест №1: Проверка корректной установки бомбы
TEST_F(BombTest, PlantingBombSuccessfully) {
    // Пытаемся установить бомбу на начальную позицию в начальное время
    bomb.planting(initial_pos, initial_time);

    // Проверяем, что бомба не активирована сразу
    ASSERT_FALSE(bomb.is_activate());

    // Спустя время, превышающее время установки, проверяем, что бомба активируется
    __int64_t later_time = initial_time + 5000; // Например, через 5000 миллисекунд
    bomb.planting(initial_pos, later_time);

    // Проверяем, что бомба активирована
    ASSERT_TRUE(bomb.is_activate());
}

// Тест №2: Проверка, что бомба не активируется при перемещении
TEST_F(BombTest, BombNotActivatedWhenMoved) {
    // Пытаемся установить бомбу на начальную позицию в начальное время
    bomb.planting(initial_pos, initial_time);

    // Перемещаем бомбу на новую позицию через некоторое время
    sf::FloatRect new_pos(100, 100, 50, 50);
    __int64_t move_time = initial_time + 2000; // Через 2000 миллисекунд
    bomb.planting(new_pos, move_time);

    // Проверяем, что бомба не активирована после перемещения
    ASSERT_FALSE(bomb.is_activate());

    // Возвращаем бомбу на начальную позицию и проверяем, что она все еще не активирована
    bomb.planting(initial_pos, move_time + 3000); // Через 3000 миллисекунд после перемещения
    ASSERT_FALSE(bomb.is_activate());
}

// Тест №3: Проверка корректной обработки повторной установки бомбы
TEST_F(BombTest, ReplantingBomb) {
    // Пытаемся установить бомбу на начальную позицию в начальное время
    bomb.planting(initial_pos, initial_time);

    // Спустя время, превышающее время установки, проверяем, что бомба активируется
    __int64_t later_time = initial_time + 5000; // Например, через 5000 миллисекунд
    bomb.planting(initial_pos, later_time);
    ASSERT_TRUE(bomb.is_activate());

    // Сбрасываем установку бомбы
    bomb.deactivate();
    ASSERT_FALSE(bomb.is_activate());

    // Пытаемся установить бомбу снова
    bomb.planting(initial_pos, later_time + 1000);
    __int64_t replant_time = later_time + 6000; // Например, через 6000 миллисекунд
    bomb.planting(initial_pos, replant_time);

    // Проверяем, что бомба снова активируется
    ASSERT_TRUE(bomb.is_activate());
}

// Тест №4: Проверка игнорирования установки бомбы, если она уже активирована (негативный)
TEST_F(BombTest, IgnorePlantingWhenActivated) {
    // Устанавливаем бомбу на начальную позицию в начальное время
    bomb.planting(initial_pos, initial_time);
    __int64_t later_time = initial_time + 5000; // Через 5000 миллисекунд
    bomb.planting(initial_pos, later_time);
    ASSERT_TRUE(bomb.is_activate());

    // Пытаемся установить бомбу снова, когда она уже активирована
    sf::FloatRect new_pos(100, 100, 50, 50);
    __int64_t new_time = later_time + 1000; // Еще через 1000 миллисекунд
    bomb.planting(new_pos, new_time);

    // Проверяем, что бомба не изменила своего состояния и осталась активированной
    ASSERT_TRUE(bomb.is_activate());
    ASSERT_EQ(bomb.get_pos(), sf::Vector2f(initial_pos.left + initial_pos.width / 2, initial_pos.top + initial_pos.height / 2));
}

// Запуск всех тестов
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
