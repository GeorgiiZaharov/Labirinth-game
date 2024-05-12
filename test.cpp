// #include <SFML/Graphics.hpp>
// #include <SFML/Window.hpp>
// #include <SFML/System.hpp>
// #include <SFML/Audio.hpp>
// #include <SFML/Window/Keyboard.hpp>

// #include <string>
// #include <vector>
// #include <iostream>
// #include <cmath>
// #include <random>
// #include "Weapon.hpp"
// #include "People.hpp"
// #include "Board.hpp"

// class Hero : public People
// {
// public:
//     explicit Hero (const Gun& weapon, const sf::IntRect& pos)
//     :People(hero_tex, weapon, pos, 100, 100, 25)
//     {
//         People::set_animation(Animation(walking_anim_tex, pos.width, pos.height, 2, 500, true),
//             Animation(blood_spatter_tex, pos.width / 2, pos.height / 2, 1, 1000, false));
//     }
//     static bool loadResources(void)
//     {
//         bool success = true;

//         if (!hero_tex.loadFromFile("hero.png"))
//             success = false;

//         if (!walking_anim_tex.loadFromFile("walking_anim.png"))
//             success = false;

//         if (!blood_spatter_tex.loadFromFile("blood_spatter_anim.png"))
//             success = false;

//         return success;
//     }

// private:
//     static sf::Texture hero_tex, walking_anim_tex, blood_spatter_tex;
// };
// sf::Texture Hero::hero_tex, Hero::walking_anim_tex, Hero::blood_spatter_tex;

// int main()
// {
//     AKR::loadResources();

//     Hero::loadResources();

//     Board::loadResources();
//     sf::FloatRect rec(0,0, 1000, 600);
//     Board b(rec, 10, 10);
//     b.build();

//     // Создаем объект окна
//     sf::RenderWindow window(sf::VideoMode(1000, 600), "My SFML Window");
//     window.setFramerateLimit(60);
//     sf::Clock clock;
    
//     AKR akr (20, 40, 20);
//     sf::IntRect pos(100, 100, 60, 60);

//     Hero h (akr, pos);
//     int dx=0, dy=0;

//     // Главный цикл приложения
//     while (window.isOpen())
//     {
//         sf::Event event;
//         while (window.pollEvent(event))
//         {
//             if (event.type == sf::Event::Closed)
//                 window.close();
           
//             else if (event.type == sf::Event::MouseButtonPressed)
//             {
//                 if (event.mouseButton.button == sf::Mouse::Left)
//                 {
//                     h.shooting(clock.getElapsedTime().asMilliseconds());
//                 }
//             }

//             else if (event.type == sf::Event::KeyPressed)
//             {
//                 if (event.key.code == sf::Keyboard::Left)
//                 {
//                     dx = -1;
//                 }
//                 if (event.key.code == sf::Keyboard::Right)
//                 {
//                     dx = 1;
//                 }
//                 if (event.key.code == sf::Keyboard::Up)
//                 {
//                     dy = -1;
//                 }
//                 if (event.key.code == sf::Keyboard::Down)
//                 {
//                     dy = 1;
//                 }
//             }
//             else if (event.type == sf::Event::KeyReleased)
//             {
//                 if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Right)
//                     dx = 0;
//                 else if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::Down)
//                     dy = 0;
//             }
//         }

//         sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
//         sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
//         h.aim(worldPos.x, worldPos.y);
//         h.moving(clock.getElapsedTime().asMilliseconds(), dx, dy);
//         // Очищаем окно
//         window.clear();
//         // Отрисовываем содержимое окна
//         b.draw(window);
//         h.draw(window, clock.getElapsedTime().asMilliseconds());

//         // Отображаем содержимое окна на экране
//         window.display();
//     }

//     return 0;
// }
