#include "map.h"
#include <SFML/Graphics.hpp>
#include "base.h"
#include "player.h"

int main()
{
    unsigned int x= SIZEX*TILESIZE;
    unsigned int y= SIZEY*TILESIZE;
    auto window = sf::RenderWindow(sf::VideoMode({x, y}), "Mapa Gry z SFML");
    window.setFramerateLimit(60);

    Map myMap;
    unsigned int iloscGraczy = 4;

    std::vector<std::pair<int, int>> bazy = myMap.inicializeMap(iloscGraczy);

    std::vector<Player> gracze;
    for (size_t i = 0; i < bazy.size(); ++i)
    {
        gracze.emplace_back(static_cast<Teams>(i + 1), bazy[i], 300); // Daj każdemu graczowi 300 złota
        gracze.back().buyRandomUnits(myMap);                          // Kup losowe jednostki na start
    }

    while (true)
    {
        // Sprawdź czy tylko jeden gracz ma żywą bazę
        int aliveBases = 0;
        for (auto &gracz : gracze)
        {
            if (gracz.getBase().getHp() > 0)
            {
                aliveBases++;
            }
        }

        if (aliveBases <= 1)
        {
            std::cout << "Game Over! ";
            for (auto &gracz : gracze)
            {
                if (gracz.getBase().getHp() > 0)
                {
                    std::cout << "Player " << static_cast<int>(gracz.getTeam()) << " won!" << std::endl;
                }
            }
            break;
        }
        // Event
        while (const std::optional event = window.pollEvent())
        {
            // Obsługa zdarzeń
            if (event->is<sf::Event::Closed>())
            {
                window.close();
                return 0;
            }
        }

        // Wykonaj turę dla każdego gracza
        for (size_t i = 0; i < gracze.size(); ++i) {
            std::vector<Player*> opponents;
            for (size_t j = 0; j < gracze.size(); ++j) {
                if (i != j) {
                    opponents.push_back(&gracze[j]); // Push back a POINTER to the existing Player object
                }
            }

            gracze[i].takeTurn(opponents, myMap); // Przekazujesz kopię gracze[i]???
            gracze[i].removeDeadUnits();
        }

        // Usuwanie martwych jednostek od każdego gracza
        for (auto &gracz : gracze)
        {
            gracz.removeDeadUnits();
        }

        // Symulacja strat
        for (auto &gracz : gracze)
        {
            for (auto &innyGracz : gracze)
            {
                if (gracz.getTeam() != innyGracz.getTeam())
                {
                    // Każda jednostka atakuje bazę przeciwnika (uproszczone)
                    for (auto &jednostka : gracz.getCreatures())
                    {
                        if (jednostka->isInRange(innyGracz.getBase()))
                        {
                            innyGracz.getBase().takeDamage(10);
                            break;
                        }
                    }
                }
            }
        }

        // Czyszczenie okna
        window.clear(sf::Color::Black);
        myMap.showMap(window);
        for (auto &gracz : gracze)
            drawBase(window, gracz.getBase());
        for (auto &gracz : gracze)
        {
            for (auto &creature : gracz.getCreatures())
                drawUnit(window, *creature);
        }

        // Rysowanie mapy i jednostek (SFML code here)
        // drawMap(window, myMap);
        //  drawUnits(window, gracze);  Potrzebujesz napisać te funkcje

        window.display();
    }

    /* Główna pętla gry
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            // Obsługa zdarzeń
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // Czyszczenie okna
        window.clear(sf::Color::Black);

        // Rysowanie mapy
        myMap.showMap(window);

        // Wyświetlanie zawartości okna
        window.display();
    }*/

    return 0;
}
