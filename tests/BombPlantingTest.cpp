#include "BombPlantingTest.hpp"
void BombPlantingTest::SetUp() 
{
    bomb.last_bomb_pos = sf::Vector2f(-100.0f, -100.0f);
    bomb.start_plant_time = 0;
    bomb.planting_time = 2000; // время установки 2 сек
    bomb.plant_duration = 0;
    bomb.activate = false;
    initial_pos = sf::FloatRect(0, 0, 50, 50);  // Начальная позиция
    initial_time = 1000; // Начальное время в миллисекундах
}

// Тест №1: Проверка корректной установки бомбы
TEST_F(BombPlantingTest, PlantingBombSuccessfully) {
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
TEST_F(BombPlantingTest, BombNotActivatedWhenMoved) {
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
TEST_F(BombPlantingTest, ReplantingBomb) {
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
TEST_F(BombPlantingTest, IgnorePlantingWhenActivated) {
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

// Тест №5: Проверка корректного обновления plant_duration при не завершенной установке бомбы (позитивный)
TEST_F(BombPlantingTest, DropPlanting) {
    // Устанавливаем бомбу на начальную позицию в начальное время
    bomb.planting(initial_pos, initial_time);

    // Время через 500 миллисекунд после начального, проверяем что установка не завершена
    __int64_t partial_time = initial_time + 500;
    bomb.planting(initial_pos, partial_time);

    // Проверяем, что бомба не активирована и plant_duration обновляется корректно
    ASSERT_FALSE(bomb.is_activate());

    auto plant_info = bomb.get_plant_info();
    ASSERT_EQ(plant_info.first, 500); // Проверяем, что plant_duration = 500 миллисекунд
    ASSERT_LT(plant_info.first, plant_info.second); // Проверяем, что plant_duration < planting_time
}

// Тест №6: Проверка прерывания установки бомбы
TEST_F(BombPlantingTest, PlantDurationUpdateWhenNotCompleted) {
    // Устанавливаем бомбу на начальную позицию в начальное время
    bomb.planting(initial_pos, initial_time);

    // Время через 1999 миллисекунд после начального, проверяем что установка не завершена
    __int64_t partial_time = initial_time + 1999; // секунда до установки
    bomb.planting(initial_pos, partial_time);

    // Проверяем, что бомба не активирована
    ASSERT_FALSE(bomb.is_activate());

    // сбрасываем установку бомбы
    bomb.drop_planting();

    // Проверяем, что бомба не активирована
    ASSERT_FALSE(bomb.is_activate());

    // проходит очень много времени, прежде чем начнется новая установка бомбы
    partial_time += 50000;

    // начинаем снова установку бомбы
    bomb.planting(initial_pos, partial_time);

    // Проверяем, что бомба не активирована
    ASSERT_FALSE(bomb.is_activate());

    auto plant_info = bomb.get_plant_info();
    ASSERT_EQ(plant_info.first, 0); // Проверяем, что plant_duration = 0 миллисекунд, так как мы только начали установку
}
