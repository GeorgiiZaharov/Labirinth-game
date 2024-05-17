// Animation.cpp
#include "Animation.hpp"

// Конструктор класса Animation
// Инициализирует анимацию с файлом текстуры, размерами, количеством спрайтов, временем изменения и настройкой цикличности
Animation::Animation(sf::Texture& tex, const sf::Sound& sound, float h, float w, std::size_t n, __int64_t time_for_change, bool is_looped)
    : tex(&tex),
      sound(sound),
      h(h), w(w),
      num_sprites(n),
      cur_sprite_num(0),
      time_for_change(time_for_change),
      last_time(0), 
      running(false),
      is_looped(is_looped),
      with_sound(true)
{
    load_data();
}

Animation::Animation(sf::Texture& tex, float h, float w, std::size_t n, __int64_t time_for_change, bool is_looped)
    : tex(&tex),
      h(h), w(w),
      num_sprites(n),
      cur_sprite_num(0),
      time_for_change(time_for_change),
      last_time(0),
      running(false),
      is_looped(is_looped),
      with_sound(false)
{
    load_data();
}


// Загружает текстуру и разрезает ее на отдельные спрайты в соответствии с количеством спрайтов
void Animation::load_data(void)
{
    sf::Vector2u tex_size = tex->getSize();
    sf::IntRect to_cut = sf::IntRect(0, 0, tex_size.x / num_sprites, tex_size.y);
    int one_frame_width = tex_size.x / num_sprites;
    for (std::size_t i = 0; i < num_sprites; ++i) {
        sf::Sprite sprite;
        sprite.setTexture(*tex);
        sprite.setTextureRect(to_cut);

        sf::FloatRect sprite_bounds = sprite.getGlobalBounds();
        float imageScale_x = w / sprite_bounds.width;
        float imageScale_y = h / sprite_bounds.height;
        sprite.setScale(imageScale_x, imageScale_y);
        sprite.setOrigin(w / 2 * (1.0f / imageScale_x), h / 2 * (1.0f / imageScale_y));
        sprites.push_back(sprite);
        to_cut.left += one_frame_width;
    }
}


// Запускает анимацию в текущее время
void Animation::start_animation(__int64_t cur_time)
{
    if (!is_looped && !running) {
        if (with_sound) sound.play();
        last_time = cur_time;
        cur_sprite_num = 0;
    }
    if (is_looped && with_sound)
    {
        sound.play();
    }
    
    running = true;
}

// Изменяет размер sf::Sprite-ов
void Animation::resize_animation(const sf::FloatRect& pos)
{
    for (auto& sprite : sprites) {
        sprite.setScale(1.0f, 1.0f);
        sprite.setScale(
            pos.width / sprite.getGlobalBounds().width,
            pos.height / sprite.getGlobalBounds().height
        );
    }
    w = pos.width; h = pos.height;
}


// Проверяет, запущена ли анимация
bool Animation::is_running(void) const
{
    return running;
}

// Возвращает текущий спрайт на основе прошедшего времени с момента последнего изменения спрайта
// !!! ВНИМАНИЕ! Возращает спрайт с Origin по центру!!!
sf::Sprite Animation::get_sprite(__int64_t cur_time)
{
    if (cur_time - last_time >= time_for_change) {

        cur_sprite_num = (cur_sprite_num + 1) % num_sprites;
        if (cur_sprite_num == 0) 
        {
            running = false;
        }
        last_time = cur_time;
    }
    return sprites[cur_sprite_num];
}
