#ifndef _GAME_OF_LIFE_H_
#define _GAME_OF_LIFE_H_

#include <SFML/Graphics.hpp>
#include <vector>

class game_of_life
{
public:
    game_of_life(int sirina, int visina, int velicina_celije);
    void draw(sf::RenderWindow& window);
    void NovaMreza();
    bool isColorTransitionComplete();

private:
    int SirinaMreze;
    int VisinaMreze;
    int VelicinaCelije;
    int BrojCelija;
    std::vector<int> Mreza;
    std::vector<int> SljedecaMreza;
    std::vector<sf::Color> BojaCelija;
    sf::Clock clock;
    float deltaTime = 0.0f;
    void NoveBoje();
    sf::Color PromjenaBoje(sf::Color trenutnaBoja, sf::Color ciljanaBoja);
    int Prebacivanje(int p, int kraj);
};

#endif