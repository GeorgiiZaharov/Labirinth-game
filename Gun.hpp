#ifndef GUN_HPP
#define GUN_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <random>
#include "Animation.hpp"
#include "Bullet.hpp"

class Gun
{
public:
    /*
     * ===================================================
     * Конструкторы
     * ===================================================
     */
    explicit Gun(void) = default;
    explicit Gun(sf::Texture& gun_tex, sf::Texture& bullet_tex, float gun_h, float gun_w, float bullet_h, float bullet_w, float bullet_speed, __int64_t bullet_duration, __int64_t recharge_time, std::size_t magazine_size, std::size_t magazine_cnt, float r, float spread, int damage);
    virtual ~Gun(void) = default;
    /*
     * ===================================================
     * Методы
     * ===================================================
     */
    // Вращает пушку на основе заданных координат и направления.
    void rotate(float x, float y, float direct_x, float direct_y);

    // Увеличивает на 1 количество магазинов
    void new_magazine(void);

    // Выстрел, если возможно, на основе текущего времени и состояния игры.
    void shoot(__int64_t cur_time, bool is_running);

    // Перемещает все пули в магазине.
    void move_bullets(__int64_t cur_time);

    // Рисует пушку и пули на окне.
    void draw(sf::RenderWindow& window, __int64_t cur_time);

    /*
     * ===================================================
     * Геттеры/сеттеры
     * ===================================================
     */
    // Возвращает текущую информацию о магазине. (текущее количество патронов, максимальное)
    std::pair<std::size_t, std::size_t> get_magazine_info(void) const {return std::make_pair(bullets_in_gun, magazine_size);}

    // Возвращает текущую информацию о пушке. (количество оставшихся магазинов)
    std::size_t get_gun_info(void) const {return magazine_cnt;}

    // Возращает угол разброса в градусах
    float get_spread(void) const {return spread;}

    // Возращает урон оружия персонажа
    int get_damage(void) const {return damage;}

    void change_damage(int delta_damage) {damage += delta_damage;}

    // Возращает время перезарядки в м/с
    std::size_t get_recharge_time(void) const {return static_cast<std::size_t>(recharge_time);}
    
    // Возращает вектор ссылок на текущее состояние magazine
    std::pair<std::vector<Bullet>::iterator, std::vector<Bullet>::iterator> get_bullets(void);

    // Изменяет максимальное количество патронов
    void change_magazine_size(std::size_t delta_size) {magazine_size += delta_size;}

    // Изменяет скорость перезарядки
    void change_recharge_time(__int64_t delta_time) {recharge_time = std::max(static_cast<__int64_t>(0), recharge_time + delta_time);}

    // Изменяет разброс
    void change_spread(float delta_spread) {spread = std::max(0.0f, spread + delta_spread);}

protected:
    // Устанавливает анимацию для выстрела пушки.
    void set_animation(const Animation& fire_anim, const Animation& recharge_anim);

    // Загружает необходимы ресурсы, которые нелязя просто копировать
    void load_data(void);

    // Данные, связанные с пушкой и пулями.
    sf::Texture *gun_tex, *bullet_tex, *anim_tex;

private:
    float gun_h, gun_w, bullet_h, bullet_w;
    std::size_t bullets_in_gun, magazine_size, magazine_cnt;
    __int64_t last_shoot_time, bullet_duration, recharge_time;
    float speed_bullet, bullet_height, bullet_width;
    sf::Sprite gun_sprite, bullet_sprite;
    std::vector<Bullet> magazine;
    float poz_x, poz_y, r, dx, dy, spread;
    int damage;
    Animation fire_anim, recharge_anim;
};

#endif // GUN_HPP
