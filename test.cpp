// #include <SFML/Graphics.hpp>
// #include "Button.hpp"

// int main()
// {
//     sf::RenderWindow window(sf::VideoMode(800, 600), "Button Test");
//     window.setFramerateLimit(60);
//     // Загрузка ресурсов
//     if (!Button::loadResources())
//     {
//         std::cout << "Ошибка загрузки ресурсов" << std::endl;
//         return -1;
//     }

//     // Создание кнопки
//     sf::FloatRect buttonRect(200.f, 200.f, 200.f, 50.f);
//     Button button(buttonRect, "Click Me");

//     while (window.isOpen())
//     {
//         sf::Event event;
//         while (window.pollEvent(event))
//         {
//             if (event.type == sf::Event::Closed)
//                 window.close();
//             if (event.type == sf::Event::MouseButtonPressed)
//             {
//                 sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
//                 button.try_to_press(mousePos);
//             }
//         }
//         // Проверка нажатия на кнопку
//         if (button.is_pressed())
//         {
//             std::cout << "Кнопка нажата" << std::endl;
//         }

//         window.clear();

//         // Отрисовка кнопки
//         sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
//         button.draw(window, mousePos);

//         window.display();
//     }

//     return 0;
// }
