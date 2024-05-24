#include <gtest/gtest.h>
#include "BoardNavigatorTest.hpp"
#include "BoardDFSTest.hpp"
#include "BombPlantingTest.hpp"

// Запуск всех тестов
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
