#include "Gun.hpp"

Gun::Gun(sf::Texture& gun_tex, sf::Texture& bullet_tex, float gun_h, float gun_w, float bullet_h, float bullet_w, float bullet_speed, __int64_t bullet_duration, __int64_t recharge_time, std::size_t magazine_size, std::size_t magazine_cnt, float r, float spread, int damage)
    :gun_tex(&gun_tex), bullet_tex(&bullet_tex), gun_h(gun_h), gun_w(gun_w), bullet_h(bullet_h), bullet_w(bullet_w),
    bullets_in_gun(magazine_size),magazine_size(magazine_size), magazine_cnt(magazine_cnt), last_shoot_time(-10000),
    bullet_duration(bullet_duration), recharge_time(recharge_time), speed_bullet(bullet_speed), bullet_height(bullet_h),
    bullet_width(bullet_w), r(r), spread(spread), damage(damage)
{
    load_data();    
    magazine.resize(magazine_size);
}

void Gun::load_data()
{   
    // загружаем спрайт пушки
    gun_sprite.setTexture(*gun_tex);
    sf::FloatRect imageGunBounds = gun_sprite.getGlobalBounds();
    float imageGunScale_x = gun_w / imageGunBounds.width;
    float imageGunScale_y = gun_h / imageGunBounds.height;
    gun_sprite.setScale(imageGunScale_x, imageGunScale_y);
    gun_sprite.setOrigin(gun_w / 2 * (1.0f / imageGunScale_x),
        gun_h / 2 * (1.0f / imageGunScale_y));

    // загружаем спрайт пули
    bullet_sprite.setTexture(*bullet_tex);
    sf::FloatRect imageBulletBounds = bullet_sprite.getGlobalBounds();
    float imageBulletScale_x = bullet_w / imageBulletBounds.width;
    float imageBulletScale_y = bullet_h / imageBulletBounds.height;
    bullet_sprite.setScale(imageBulletScale_x, imageBulletScale_y);
    bullet_sprite.setOrigin(bullet_w / 2.0f * (1.0f / imageBulletScale_x),
         bullet_h / 2.0f * (1.0f / imageBulletScale_x));
}

void Gun::rotate(float x, float y, float direct_x, float direct_y)
{
    // Логика вращения пушки на основе заданных координат и направления.
    // Этот метод обновляет позицию и ориентацию пушки.
    float length = std::hypot(direct_x, direct_y);
    dx = direct_x / length;
    dy = direct_y / length;
    sf::FloatRect gun_rect = gun_sprite.getGlobalBounds();
    poz_x = x + dx * (r + gun_rect.width / 2);
    poz_y = y + dy * (r + gun_rect.height / 2);
}

void Gun::new_magazine(void)
{
    magazine_cnt += 1;
}

void Gun::shoot(__int64_t cur_time, bool is_running)
{
    // Логика выстрела пули, если возможно, на основе текущего времени и состояния игры.
    // Этот метод обновляет магазин и стреляет пулей, если условия выполнены.
    if (bullets_in_gun != 0 && cur_time - last_shoot_time >= bullet_duration)
    {   
        sf::FloatRect bullet_rect = {
            poz_x,
            poz_y, 
            bullet_width,
            bullet_height
        };
        if (!is_running)
        {
            magazine[magazine_size - bullets_in_gun] = Bullet(bullet_sprite, bullet_rect, dx, dy, speed_bullet, cur_time);
            bullets_in_gun -= 1;
            last_shoot_time = cur_time;
        }
        else
        {
            std::random_device rd; // Источник энтропии для инициализации
            std::mt19937 gen(rd()); // Стандартный мергенный генератор
            std::normal_distribution<float> dist(spread, spread / 4.0f);                
            float randomAngle = std::max(0.0f, std::min(60.0f, static_cast<float>(dist(gen)))) - spread; // Округление до ближайшего целого
            float new_dx = dx * std::cos(randomAngle * M_PI / 180.0f) - dy * std::sin(randomAngle * M_PI / 180.0f);
            float new_dy = dx * std::sin(randomAngle * M_PI / 180.0f) + dy * std::cos(randomAngle * M_PI / 180.0f);    
            magazine[magazine_size - bullets_in_gun] = Bullet(bullet_sprite, bullet_rect, new_dx, new_dy, speed_bullet, cur_time);
            bullets_in_gun -= 1;
            last_shoot_time = cur_time;
        }
        fire_anim.start_animation(cur_time);
    }
    else if (bullets_in_gun == 0 && cur_time - last_shoot_time >= recharge_time && magazine_cnt != 0)
    {
        bullets_in_gun = magazine_size;
        --magazine_cnt;
    }
}

void Gun::move_bullets(__int64_t cur_time)
{
    // Перемещает все пули в магазине.
    // Этот метод обновляет позицию каждой пули в магазине.
    for (std::size_t i = 0; i < magazine_size - bullets_in_gun; ++i)
        magazine[i].move(cur_time);
}

void Gun::draw(sf::RenderWindow& window, __int64_t cur_time)
{
    // Рисует пушку и пули на окне.
    // Этот метод отвечает за отрисовку пушки и ее пуль.

    for (std::size_t i = 0; i < magazine_size - bullets_in_gun; ++i)
        magazine[i].draw(window);

    if (bullets_in_gun == 0 && cur_time - last_shoot_time < recharge_time && magazine_cnt != 0)
    {
        recharge_anim.start_animation(cur_time);
    }

    sf::Sprite to_draw;
    if (fire_anim.is_running())
    {
        to_draw = fire_anim.get_sprite(cur_time);
    }
    else if (recharge_anim.is_running())
    {
        to_draw = recharge_anim.get_sprite(cur_time);
    }
    else
    {
        to_draw = gun_sprite;
    }
    to_draw.setPosition(poz_x, poz_y);
    float angle = std::atan2(dy, dx) * 180 / M_PI;
    to_draw.setRotation(angle);
    window.draw(to_draw);
}

void Gun::upgrade(__int64_t cur_time)
{
    if (bullets_in_gun == 0 && cur_time - last_shoot_time >= recharge_time && magazine_cnt != 0)
    {
        bullets_in_gun = magazine_size;
        --magazine_cnt;
    }
}

void Gun::set_animation(const Animation& fire_anim, const Animation& recharge_anim, const Animation& presentation_anim)
{
    // Устанавливает анимацию для выстрела пушки.
    // Этот метод обновляет анимацию выстрела пушки.
    this->fire_anim = fire_anim;
    this->recharge_anim = recharge_anim;
    this->presentation_anim = presentation_anim;

}

void Gun::start_presentation_animation(__int64_t cur_time)
{
    presentation_anim.start_animation(cur_time);
}

sf::Sprite Gun::get_presentation_sprite(__int64_t cur_time)
{
    return presentation_anim.get_sprite(cur_time);
}


std::pair<std::vector<Bullet>::iterator, std::vector<Bullet>::iterator> Gun::get_bullets(void)
{
    return std::make_pair(magazine.begin(), magazine.begin() + magazine_size - bullets_in_gun);    
}

std::pair<std::size_t, std::size_t> Gun::get_magazine_info(void) const 
{
    return std::make_pair(bullets_in_gun, magazine_size);
}

// Возвращает текущую информацию о пушке. (количество оставшихся магазинов)
std::size_t Gun::get_gun_info(void) const 
{
    return magazine_cnt;
}

// Возращает угол разброса в градусах
float Gun::get_spread(void) const 
{
    return spread;
}

// Возращает урон оружия персонажа
int Gun::get_damage(void) const 
{
    return damage;
}

void Gun::change_damage(int delta_damage) 
{
    damage += delta_damage;
}

// Возращает время перезарядки в м/с
std::size_t Gun::get_recharge_time(void) const 
{
    return static_cast<std::size_t>(recharge_time);
}

// Изменяет максимальное количество патронов
void Gun::change_magazine_size(std::size_t delta_size) 
{
    magazine_size += delta_size;
}

// Изменяет скорость перезарядки
void Gun::change_recharge_time(__int64_t delta_time) 
{
    recharge_time = std::max(static_cast<__int64_t>(0), recharge_time + delta_time);
}

// Изменяет разброс
void Gun::change_spread(float delta_spread) 
{
    spread = std::max(0.0f, spread + delta_spread);
}

