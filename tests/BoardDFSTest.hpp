#ifndef BOARD_DFS_TEST_HPP
#define BOARD_DFS_TEST_HPP

#include <gtest/gtest.h>
#include <SFML/Graphics.hpp>

#include "../BoardLogic.hpp"
#include <queue>
#include <map>
#include <algorithm>

/*
* =======================================================
* Тест алгоритма построения лабиринта
* =======================================================
*/
// Класс для тестирования приватных методов класса Board
class BoardDFSTest : public ::testing::Test {
protected:
    BoardLogic board;   // экземпляр класса с методами класса Board

    // Переопределяем метод SetUp(), который будет вызываться перед каждым тестом
    void SetUp() override;
};

#endif