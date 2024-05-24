#ifndef BOARD_NAVIGATOR_TEST_HPP
#define BOARD_NAVIGATOR_TEST_HPP

#include <gtest/gtest.h>
#include <SFML/Graphics.hpp>

#include "../BoardLogic.hpp"
#include <queue>
#include <map>
#include <algorithm>

/*
* =======================================================
*   Тест алгоритма навигации в лабиринте
* =======================================================
*/
// Класс для тестирования метода navigator класса Board
class BoardNavigatorTest : public ::testing::Test {
protected:
    BoardLogic board;   // экземпляр класса с методами класса Board
    // Метод SetUp, который будет вызываться перед каждым тестом
    void SetUp() override;
};
#endif