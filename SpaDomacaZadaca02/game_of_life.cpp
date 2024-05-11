#include "game_of_life.h"
#include <ctime>

//Postavljanje prve mreze sa 25% sanse da je celija ziva
game_of_life::game_of_life(int sirina, int visina, int velicina_celije) : SirinaMreze(sirina), VisinaMreze(visina), VelicinaCelije(velicina_celije), BrojCelija(sirina* visina), Mreza(BrojCelija), SljedecaMreza(BrojCelija), BojaCelija(BrojCelija)
{
    srand(time(nullptr));
    for (int i = 0; i < BrojCelija; i++)
        Mreza[i] = (double(rand()) / RAND_MAX < 0.25) ? 1 : 0;
}

//Kad piksel izade iz mape vraca se kroz suprotni dio mape
int game_of_life::Prebacivanje(int p, int kraj)
{
    if (p == -1) return kraj - 1;
    if (p == kraj) return 0;
    return p;
}

//Provjera koje su celije umrle a koje su nove nastale
void game_of_life::NoveBoje() {
    for (int i = 0; i < BrojCelija; i++) {
        if (Mreza[i] == 1) {
            //Nove celije se korak po korak mijenjaju iz zelene u bijelu
            if (Mreza[i] != SljedecaMreza[i]) {
                BojaCelija[i] = PromjenaBoje(sf::Color::Green, sf::Color::White);
            }
            else {
                BojaCelija[i] = sf::Color::White;
            }
        }
        else {
            //Mrtve celije se korak po korak mijenjaju iz crvene u crnu
            if (Mreza[i] != SljedecaMreza[i]) {
                BojaCelija[i] = PromjenaBoje(sf::Color::Red, sf::Color::Black);
            }
            else {
                BojaCelija[i] = sf::Color::Black;
            }
        }
    }
}

//Crtanje celije
void game_of_life::draw(sf::RenderWindow& window)
{
    //Prije crtanja celija se mijenjaju boje
    deltaTime += clock.restart().asMilliseconds();
    NoveBoje();

    window.clear(sf::Color(128, 128, 128));
    for (int x = 0; x < SirinaMreze; x++) {
        for (int y = 0; y < VisinaMreze; y++) {
            int index = x + y * SirinaMreze;
            sf::RectangleShape cell(sf::Vector2f(VelicinaCelije, VelicinaCelije));
            cell.setPosition(x * VelicinaCelije, y * VelicinaCelije);
            cell.setOutlineThickness(1);
            cell.setOutlineColor(sf::Color::Black);
            cell.setFillColor(BojaCelija[index]);
            window.draw(cell);
        }
    }
}

//Mijenjanje boja novih i mrtvih celija korak po korak
sf::Color game_of_life::PromjenaBoje(sf::Color trenutnaBoja, sf::Color ciljanaBoja) {
    //Promjena boja treba trajati 2 sekunde
    float totalDuration = 2000.0f;
    float blendFactor = deltaTime / totalDuration;
    sf::Color PromjenjenaBoja(
        trenutnaBoja.r + blendFactor * (ciljanaBoja.r - trenutnaBoja.r),
        trenutnaBoja.g + blendFactor * (ciljanaBoja.g - trenutnaBoja.g),
        trenutnaBoja.b + blendFactor * (ciljanaBoja.b - trenutnaBoja.b)
    );
    return PromjenjenaBoja;
}

//Racunanje novih celija u mrezi
void game_of_life::NovaMreza() {
    for (int x = 0; x < SirinaMreze; x++) {
        for (int y = 0; y < VisinaMreze; y++) {
            int BrojSusjeda = 0;
            for (int i = -1; i < 2; i++) {
                for (int j = -1; j < 2; j++) {
                    int xi = Prebacivanje(x + i, SirinaMreze);
                    int yj = Prebacivanje(y + j, VisinaMreze);
                    BrojSusjeda += Mreza[xi + yj * SirinaMreze];
                }
            }
            int current = x + y * SirinaMreze;
            BrojSusjeda -= Mreza[current];
            SljedecaMreza[current] = Mreza[current];
            if (Mreza[current] == 1 && (BrojSusjeda < 2 || BrojSusjeda > 3))
                SljedecaMreza[current] = 0;
            else if (BrojSusjeda == 3)
                SljedecaMreza[current] = 1;
        }
    }
    Mreza.swap(SljedecaMreza);
}

//Provjerava jeli promjena boja gotova
bool game_of_life::isColorTransitionComplete() {
    //Ponekad je program za racunanje jeli promjena boje gotova neprecizan.
    //Zato stavljam treshold gdje ce funkcija vratiti true cak ako se boje nisu promjenile tocno do bijele/crne.
    //Tocnost racunanja ovisi dosta o snazi racunala pa se treshold mozda treba mijenjati.
    //Za jaca racunala treshold bi trebao biti manji a za sporija veci
    //Sto imam vise celija u programu to je program sporiji i teze se boje blendaju.
    //Ponekad ce se dogoditi da ce se boje ponovo updejtati bez promjene mreze ili ce se mreza promjeniti bez promjene boja.
    //Sto je bolje postavljen treshold ili sto je manje celija to se takva situacija rijede dogada.
    const int colorThreshold = 9;

    for (int i = 0; i < BrojCelija; i++) {
        if (Mreza[i] == 1 && BojaCelija[i] != sf::Color::White) {
            if (abs(BojaCelija[i].r - sf::Color::White.r) > colorThreshold ||
                abs(BojaCelija[i].g - sf::Color::White.g) > colorThreshold ||
                abs(BojaCelija[i].b - sf::Color::White.b) > colorThreshold) {
                return false;
            }
        }
        else if (Mreza[i] == 0 && BojaCelija[i] != sf::Color::Black) {
            if (abs(BojaCelija[i].r - sf::Color::Black.r) > colorThreshold ||
                abs(BojaCelija[i].g - sf::Color::Black.g) > colorThreshold ||
                abs(BojaCelija[i].b - sf::Color::Black.b) > colorThreshold) {
                return false;
            }
        }
    }
    return true;
}