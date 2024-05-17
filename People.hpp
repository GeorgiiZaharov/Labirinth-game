// People.hpp
#ifndef PEOPLE_HPP
#define PEOPLE_HPP

#include "includes.hpp"

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

    // Обновляет состояние оружия (тк кл-во патронов обновляется только при стерльбе)
    void upgrade_gun(__int64_t cur_time);

    // возращает ответ на вопрос: жив ли персоаж?
    bool alive(void) const;

    // возращает ответ на вопрос: мышка указывает на персонажа?
    bool intersection(const sf::Vector2f& mousePos) const;

    // Запускает анимацию презентации
    void start_presentation_animation(__int64_t cur_time);

    /*
     * ===================================================
     * Геттеры/сеттеры
     * ===================================================
     */
    // изменяет текущее положение персонажа
    void set_position(const sf::FloatRect& new_pos);
    
    // устанавливает новое оружие
    void set_weapon(const Gun& weapon);

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

    // возращает вектор текущего направления
    sf::Vector2f get_direct(void) const;

    // Возвращает текущую информацию о пушке. (количество оставшихся магазинов)
    std::size_t get_gun_info(void) const;

    // Возращает угол разброса в градусах
    float get_spread(void) const;

    // Возращает время перезарядки в м/с
    std::size_t get_recharge_time(void) const;

    // возращает массив ссылок на bullets чтобы менять их состояние
    std::pair<std::vector<Bullet>::iterator, std::vector<Bullet>::iterator> get_bullets(void);

    // Возвращает текущую информацию о магазине. (текущее количество патронов, максимальное)
    std::pair<std::size_t, std::size_t> get_magazine_info(void) const;


    // Возращает следующий спрайт анимации презентации
    sf::Sprite get_presentation_sprite(__int64_t cur_time);
    
    // Возращает следующий спрайт анимации презентации оружия
    sf::Sprite get_gun_presentation_sprite(__int64_t cur_time);

    // изменяет текущее здоровье на delta_health
    void change_cur_health(int delta_health);

protected:
    void set_animation(const Animation& idle_animation, const Animation& walking_animation,
        const Animation& blood_spatter_animation, const Animation& dead_animation, const Animation& presentation_animation);

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
    Animation idle_anim, walking_anim, blood_spatter_anim, dead_anim, presentation_anim;
    bool is_walking;
    sf::Vector2f bullet_contact_vec;
};

#endif // PEOPLE_HPP
