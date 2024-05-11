#include <SFML/Graphics.hpp>
#include "game_of_life.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(600, 400), "GAME OF LIFE");
    window.setFramerateLimit(60);

    game_of_life game(60, 40, 10);

    bool colorTransitionComplete = false;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (!colorTransitionComplete) {
            game.draw(window);
            window.display();
            colorTransitionComplete = game.isColorTransitionComplete();
        }
        else {
            game.NovaMreza();
            colorTransitionComplete = false;
        }
    }

    return 0;
}