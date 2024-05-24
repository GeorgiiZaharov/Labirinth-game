#ifndef BOMB_PLANTING_TEST_HPP
#define BOMB_PLANTING_TEST_HPP

#include <gtest/gtest.h>
#include <SFML/Graphics.hpp>
#include "../BombLogic.hpp"

/*
* =======================================================
* Тест алгоритма установки бомбы
* =======================================================
*/
class BombPlantingTest : public ::testing::Test {
protected:
    BombLogic bomb; // экземпляр класса с методами класса Board
    sf::FloatRect initial_pos;
    __int64_t initial_time;

    void SetUp() override;
};
#endif