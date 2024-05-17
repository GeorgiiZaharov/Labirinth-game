#include "GameManager.hpp"

int main()
{
	// Создаем объект окна
	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Game", sf::Style::Fullscreen);
	// Устанавливаем лимит на кодры в секунду
	window.setFramerateLimit(60);

	GameManager g = GameManager(window);
	g.run();
}
