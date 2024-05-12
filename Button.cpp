#include "Button.hpp"

Button::Button(const sf::FloatRect& pos, std::string str, const sf::Color& color)
:big_pos(pos),
pressed(false),
text(str, font, 20)
{
	float k = 1.0f / std::sqrt(1.05f); // коэфицент пропорциональности (увеличиваем на 0.5%)
	small_pos = sf::FloatRect(pos.left + (pos.left * (1 - k)) / 2,
							  pos.top + (pos.top * (1 - k)) / 2,
							  k * pos.left, 
							  k * pos.height
							  );
    // инициализируем спрайты 
    // маленький
	small_sprite.setTexture(button_bg_tex);
	sf::FloatRect small_sprite_bounds = small_sprite.getGlobalBounds();
	small_sprite.setScale(
        small_pos.width / small_sprite_bounds.width,
        small_pos.height / small_sprite_bounds.height
    );
    small_sprite.setPosition(small_pos.left, small_pos.top);
    // большой
    big_sprite.setTexture(button_bg_tex);
	sf::FloatRect big_sprite_bounds = big_sprite.getGlobalBounds();
	big_sprite.setScale(
        big_pos.width / big_sprite_bounds.width,
        big_pos.height / big_sprite_bounds.height
    );
    big_sprite.setPosition(big_pos.left, big_pos.top);

    // инициализируем text
	text.setFillColor(color);
    sf::FloatRect text_pos = text.getGlobalBounds();
    text.setPosition(big_pos.left + (big_pos.width - text_pos.width) / 2.f,
    				big_pos.top + (big_pos.height - text_pos.height) / 2.f);
}

void Button::try_to_press(const sf::Vector2f& mousePos)
{
	// проверяем нажата ли данная кнопка
	if (small_pos.contains(mousePos))
	{
		pressed = true;
	}
}

bool Button::is_pressed(void)
{
	bool was_pressed = pressed;
    pressed = false;
    return was_pressed;
}

void Button::draw(sf::RenderWindow& window, const sf::Vector2f& mousePos)
{
	if (small_pos.contains(mousePos))
	{
		window.draw(big_sprite);
	}
	else
	{
		window.draw(small_sprite);
	}
    window.draw(text);
}


bool Button::loadResources(void)
{
	bool success = true;
	if (!button_bg_tex.loadFromFile("button_background.png"))
		success = false;

	if (!font.loadFromFile("Roboto-Black.ttf"))
		success = false;

	return success;
}

sf::FloatRect Button::get_hitbox(void) const
{
	return big_pos;
}


sf::Font Button::font;
sf::Texture Button::button_bg_tex;