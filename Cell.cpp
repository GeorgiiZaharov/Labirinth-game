#include "Cell.hpp"

Cell::Cell(const sf::Texture& bg_tex, const sf::Texture& border_tex, float x, float y, float cell_w, float cell_h, float border_width)
:is_up(true),
is_right(true),
is_down(true),
is_left(true)
{
	// Подготваливаем изображения, масштабируем их в заданном отношениии
	sf::FloatRect pos;
	// Готовим задний фон => полностью масштабируем изображение
	rec.setTexture(bg_tex);
	pos = rec.getGlobalBounds();
	rec.setScale(
		cell_w / pos.width,
		cell_h / pos.height
	);
	rec.setPosition(x, y);

	// готовим анимацию
	boom_anim = Animation(boom_tex, boom_sound, cell_h, cell_w, 3, 100, false);

	// настраиваем изображения для стен
	// Алгоритм: 
	// 		- вырезает соразмерный кусок из текстуры и масшатбирует его (см. pruning)
	pos = sf::FloatRect(x,
		y, 
		cell_w, 
		border_width
	);
	pruning(line_up, border_tex, pos);

	pos = sf::FloatRect(x + cell_w - border_width, 
		y, 
		border_width, 
		cell_h
	);
	pruning(line_right, border_tex, pos);

	pos = sf::FloatRect(x, 
		y + cell_h - border_width, 
		cell_w, 
		border_width
	);
	pruning(line_down, border_tex, pos);

	pos = sf::FloatRect(x, 
		y, 
		border_width,
		cell_h
	);
	pruning(line_left, border_tex, pos);

}

bool Cell::loadResources(void)
{
	bool success = true;
	if (!boom_tex.loadFromFile("src/boom_cell.png"))
		success = false;

	if (!boom_buf.loadFromFile("src/boom.wav"))
		success = false;

	boom_sound.setBuffer(boom_buf);		// устанавливаем буфер для звука
	return success;
} 

void Cell::make_boom(__int64_t cur_time)
{
	boom_anim.start_animation(cur_time);
}

void Cell::draw(sf::RenderWindow& window, __int64_t cur_time)
{
	window.draw(rec);
	if (boom_anim.is_running())
	{
		// получаем спрайт
		sf::Sprite tmp = boom_anim.get_sprite(cur_time);
		// устанавливаем в тоже место, что и rec
		sf::FloatRect rec_pos = rec.getGlobalBounds();
		tmp.setPosition(rec_pos.left + rec_pos.width / 2, rec_pos.top + rec_pos.height / 2);
		// выводим
		window.draw(tmp);
	}
	if (is_up) 		window.draw(line_up);
	if (is_right) 	window.draw(line_right);
	if (is_down) 	window.draw(line_down);
	if (is_left) 	window.draw(line_left);
}

bool Cell::check_collision(const sf::FloatRect& r) const
{
	if (is_up && line_up.getGlobalBounds().intersects(r))		return true;
	if (is_right && line_right.getGlobalBounds().intersects(r))	return true;
	if (is_down && line_down.getGlobalBounds().intersects(r))	return true;
	if (is_left && line_left.getGlobalBounds().intersects(r))	return true;
	return false;
}

void Cell::pruning(sf::Sprite& sprite, const sf::Texture& tex, const sf::FloatRect& pos)
{
	sf::IntRect to_cut = {0, 0, static_cast<int>(pos.width), static_cast<int>(pos.height)};
	sprite.setTextureRect(to_cut);
	sprite.setTexture(tex);

	sf::FloatRect sprite_bounds = sprite.getGlobalBounds();
    float imageScale_x = pos.width / sprite_bounds.width;
    float imageScale_y = pos.height / sprite_bounds.height;
    sprite.setScale(imageScale_x, imageScale_y);
    sprite.setPosition(pos.left, pos.top);
}
sf::Texture Cell::boom_tex;
sf::SoundBuffer Cell::boom_buf;
sf::Sound Cell::boom_sound;