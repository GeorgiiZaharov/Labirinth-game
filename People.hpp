// People.hpp
#ifndef PEOPLE_HPP
#define PEOPLE_HPP

#include <SFML/Graphics.hpp>
#include "Animation.hpp"
#include "Gun.hpp"
#include "HealthBar.hpp"

class People
{
public:
    /*
    * ===================================================
    * Конструкторы
    * ===================================================
    */
    explicit People() = default;
    explicit People(int health, int speed);
    virtual ~People() = default;
    /*
     * ===================================================
     * Методы
     * ===================================================
     */
    // Рисование персонажа на окне
    void draw(sf::RenderWindow& window, __int64_t cur_time);

    // Изменение угла поворота персонажа и оружия
    void aim(float x, float y);

    // Движение персонажа
    void moving(__int64_t cur_time, float move_dx, float move_dy);

    // Логика стрельбы
    void shooting(__int64_t cur_time);

    // Проверяет столкновение пули с персонажем
    bool check_collision(Bullet& bullet, __int64_t cur_time);

    // возращает ответ на вопрос: жив ли персоаж?
    bool alive(void) const;

    // возращает ответ на вопрос: мышка указывает на персонажа?
    bool intersection(const sf::Vector2f& mousePos) const;

    /*
     * ===================================================
     * Геттеры/сеттеры
     * ===================================================
     */
    // изменяет текущее положение персонажа
    void set_position(const sf::FloatRect& new_pos);
    // устанавливает новое оружие
    void set_weapon(const Gun& weapon);

    // возращает вектор текущего направления
    sf::Vector2f get_direct(void) const;

    // измненяет текущее здоровье
    void change_cur_health(int delta_health);
    // изменяет максимальное здоровье
    void change_max_health(int delta_health);
    // устанавливает максимальное здоровье
    void set_max_health(int new_max_health);
    // изменяет урон
    void change_damage(int delta_damage); 
    // устанавливает скорость персонажа
    void set_speed(int new_speed);

    // возращает hitbox персонажа
    sf::FloatRect get_hitbox(void) const;
    // возращает текущее здоровье персонажа
    int get_cur_health(void) const;
    // возращает максимальное здоровье персонажа
    int get_max_health(void) const;
    // возращяет урон оружия персонажа
    int get_damage(void) const;
    // возращает скорость персонажа
    int get_speed(void) const;

    // возращает массив ссылок на bullets чтобы менять их состояние
    std::pair<std::vector<Bullet>::iterator, std::vector<Bullet>::iterator> get_bullets(void);

    // Возвращает текущую информацию о магазине. (текущее количество патронов, максимальное)
    std::pair<std::size_t, std::size_t> get_magazine_info(void) const;

    // Возвращает текущую информацию о пушке. (количество оставшихся магазинов)
    std::size_t get_gun_info(void) const;

    // Возращает угол разброса в градусах
    float get_spread(void) const;

    // Возращает время перезарядки в м/с
    std::size_t get_recharge_time(void) const;

    // Изменяет максимальное количество патронов
    void change_magazine_size(std::size_t delta_size);

    // Изменяет скорость перезарядки
    void change_recharge_time(__int64_t delta_time);

    // Изменяет разброс
    void change_spread(float delta_spread);

protected:
    void set_animation(const Animation& idle_animation, const Animation& walking_animation,
        const Animation& blood_spatter_animation, const Animation& dead_animation);

    // Загрузка данных для спрайта
    void load_data(void);

    // Члены
    Gun gun;
    sf::FloatRect pos;
    int cur_health, max_health, speed;
    float dx, dy;
    HealthBar health_bar;
    sf::FloatRect hitbox;
    __int64_t last_time;
    Animation idle_anim, walking_anim, blood_spatter_anim, dead_anim;
    bool is_walking;
    sf::Vector2f bullet_contact_vec;
};

#endif // PEOPLE_HPP
