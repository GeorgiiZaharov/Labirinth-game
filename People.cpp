// People.cpp
#include "People.hpp"

People::People(int health, int speed)
    :cur_health(health), max_health(health), speed(speed), dx(), dy(), health_bar(), hitbox(), last_time(-1), is_walking(false)
{
}

void People::set_position(const sf::FloatRect& new_pos)
{
    pos = new_pos;
    hitbox = new_pos;
    health_bar = HealthBar(sf::FloatRect(pos.left, pos.top - 15, pos.width, 15.0f), cur_health, max_health);
    // изменяем размер спрайтов в анимации
    idle_anim.resize_animation(pos);
    walking_anim.resize_animation(pos);
    blood_spatter_anim.resize_animation(sf::FloatRect(0, 0, pos.width / 2, pos.height / 2));
    dead_anim.resize_animation(pos);
}

void People::set_animation(const Animation& idle_animation, const Animation& walking_animation,
        const Animation& blood_spatter_animation, const Animation& dead_animation, const Animation& presentation_animation)
{
    idle_anim = idle_animation;
    walking_anim = walking_animation;
    blood_spatter_anim = blood_spatter_animation;
    dead_anim = dead_animation;
    presentation_anim = presentation_animation;
}

void People::draw(sf::RenderWindow& window, __int64_t cur_time)
{
    sf::Sprite tmp_sprite;

    // отрисовываем орудие если персонаж жив
    if (alive())
    {
        // отображаем актуальный health_bar
        health_bar.set_health(cur_health);
        health_bar.draw(window);

        gun.draw(window, cur_time);
        // Если запущена анимация брызг крови
        if (blood_spatter_anim.is_running())
        {
            // вычисляем угол в градусах откуда по нам попали
            float bullet_hit_angle = std::atan2(bullet_contact_vec.y, bullet_contact_vec.x) * 180 / M_PI;
            // получаем текущий спрайт анимации
            sf::Sprite blood_spatter_sprite = blood_spatter_anim.get_sprite(cur_time);
            // устанавливаем его в правилное место
            // отображаем анимацию с краю персонажа на радиусе r 
            float rx = pos.width / 2, ry = pos.height / 2;
            float poz_x = pos.left + pos.width / 2 + bullet_contact_vec.x * (rx);
            float poz_y = pos.top + pos.height / 2 + bullet_contact_vec.y * (ry);
            blood_spatter_sprite.setPosition(poz_x, poz_y);
            blood_spatter_sprite.rotate(bullet_hit_angle);
            window.draw(blood_spatter_sprite);
        }
        if (is_walking)
        {
            tmp_sprite = walking_anim.get_sprite(cur_time);
        }
        else
        {
            tmp_sprite = idle_anim.get_sprite(cur_time);
        }
    }

    else
    {
        tmp_sprite = dead_anim.get_sprite(cur_time);
    }

    float angle = std::atan2(dy, dx) * 180 / M_PI;
    tmp_sprite.setPosition(pos.left + pos.width / 2, pos.top + pos.height / 2);
    tmp_sprite.setRotation(angle);

    window.draw(tmp_sprite);  
}

void People::aim(float x, float y)
{
    if (alive())
    {
        dx = x - (pos.left + pos.width / 2); dy = y - (pos.top + pos.height / 2);
        gun.rotate(
            pos.left + pos.width / 2, 
            pos.top + pos.height / 2,  
            dx, 
            dy
        );
    }
}

void People::moving(__int64_t cur_time, float move_dx, float move_dy)
{
    presentation_anim.start_animation(cur_time);
    // пермещаем патроны  
    gun.move_bullets(cur_time);
    if (alive())
    {
        if (last_time == -1)
            last_time = cur_time;
        // смотрим есть ли перемещение персонажа в данный момент
        if (std::abs(move_dx) + std::abs(move_dy) != 0.0f)
        {
            is_walking = true;
            walking_anim.start_animation(cur_time);
        }
        else is_walking = false;

        // нормализуем вектор перемещения
        float length = std::hypot(move_dx, move_dy);
        // если его длина 0 то нет смысла ничего перемещать
        if (length == 0.0f)
        {
            last_time = cur_time;
            return; 
        }
        move_dx /= length; move_dy /= length;
        // расчитываем смещение спрайта в соответствии с текущим временем
        float delta_time = static_cast<float>(cur_time - last_time) / 1000;
        float bias_x = move_dx * (delta_time * speed);
        float bias_y = move_dy * (delta_time * speed);
        // перемещаем объекты на bias_x bias_y  
        pos.left += bias_x; pos.top += bias_y;
        hitbox.left += bias_x; hitbox.top += bias_y;
        health_bar.move(bias_x, bias_y);

        // обновляем предыдущее время
        last_time = cur_time;
    }
    
}

void People::shooting(__int64_t cur_time)
{
    if (alive())
    {
        gun.shoot(cur_time, is_walking);
    }
}

bool People::check_collision(Bullet& bullet, __int64_t cur_time)
{
    if (alive())
    {
        if (!bullet.get_collision() && hitbox.intersects(bullet.getGlobalBounds()))    // если пуля ести и попала
        {
            bullet.collision();                 // пули больше нет
            bullet_contact_vec = bullet.get_direct();   // запоминаем откуда по нам попали
            blood_spatter_anim.start_animation(cur_time); // начинаем анимацию брызг крови
            return true;
        } 
    }
    return false;
}

void People::upgrade_gun(__int64_t cur_time)
{
    gun.upgrade(cur_time);
}

bool People::alive(void) const
{
    return cur_health > 0;
}

bool People::intersection(const sf::Vector2f& mousePos) const
{
    return hitbox.contains(mousePos);
}


void People::set_weapon(const Gun& weapon)
{
    gun = weapon;
}

sf::Vector2f People::get_direct(void) const
{
    return sf::Vector2f(dx, dy);    
}

void People::change_cur_health(int delta_health)
{
    cur_health += delta_health;
}

std::pair<std::vector<Bullet>::iterator, std::vector<Bullet>::iterator> People::get_bullets(void)
{
    return gun.get_bullets();
}

std::pair<std::size_t, std::size_t> People::get_magazine_info(void) const 
{
    return gun.get_magazine_info();
}

std::size_t People::get_gun_info(void) const
{
    return gun.get_gun_info();
}

float People::get_spread(void) const 
{
    return gun.get_spread();
}

std::size_t People::get_recharge_time(void) const
{
    return gun.get_recharge_time();
}

sf::Sprite People::get_presentation_sprite(__int64_t cur_time)
{
    return presentation_anim.get_sprite(cur_time);
}

sf::Sprite People::get_gun_presentation_sprite(__int64_t cur_time)
{
    return gun.get_presentation_sprite(cur_time);
}


void People::start_presentation_animation(__int64_t cur_time)
{
    presentation_anim.start_animation(cur_time);
    gun.start_presentation_animation(cur_time);
}

sf::FloatRect People::get_hitbox(void) const
{
    return hitbox;
}

int People::get_cur_health(void) const
{
    return cur_health;
}

int People::get_max_health(void) const
{
    return max_health;
}

int People::get_damage(void) const
{
    return gun.get_damage();
}

int People::get_speed(void) const
{
    return speed;
}

