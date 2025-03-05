#include "map.h"
#include <SFML/Graphics.hpp>
#include "base.h"
#include "player.h"

int main()
{
    unsigned int x = SIZEX * TILESIZE;
    unsigned int y = SIZEY * TILESIZE;
    auto window = sf::RenderWindow(sf::VideoMode({x, y}), "Mapa Gry z SFML");
    window.setFramerateLimit(140);

    Map myMap;
    unsigned int iloscGraczy = 10;

    std::vector<std::pair<int, int>> bazy = myMap.inicializeMap(iloscGraczy);

    std::vector<Player *> gracze;
    for (size_t i = 0; i < bazy.size(); ++i)
    {
        gracze.push_back(new Player(static_cast<Teams>(i + 1), bazy[i], 1000)); // Daj każdemu graczowi 300 złota
        gracze.back()->buyRandomUnits(myMap);                                  // Kup losowe jednostki na start
    }

    // Pętla gry - trwa dopóki więcej niż jeden gracz ma żywą bazę
    while (window.isOpen())
    {
        // Sprawdź czy tylko jeden gracz ma żywą bazę
        int aliveBases = 0;
        for (const auto &gracz : gracze)
        {
            if (gracz->getBase().getHp() > 0)
            {
                aliveBases++;
            }
        }

        if (aliveBases <= 1)
        {
            std::cout << "Game Over! ";
            for (const auto &gracz : gracze)
            {
                if (gracz->getBase().getHp() > 0)
                {
                    std::cout << "Player " << static_cast<int>(gracz->getTeam()) << " won!" << std::endl;
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
        for (size_t i = 0; i < gracze.size(); ++i)
        {
            // // Zbierz listę przeciwników (wszyscy oprócz aktualnego gracza)
            std::vector<Player *> allPlayers = gracze; 

            gracze[i]->takeTurn(allPlayers, myMap);
            gracze[i]->removeDeadUnits();
        }
        // Usuń graczy, którzy przegrali (baza zniszczona)
        for (int i = 0; i < gracze.size();)
        {
            if (gracze[i]->getBase().getHp() <= 0)
            {
                std::cout << "Player " << static_cast<int>(gracze[i]->getTeam()) << "'s base was destroyed!" << std::endl;

                // Delete the player object
                delete gracze[i];

                // Erase from the vector
                gracze.erase(gracze.begin() + i);
            }
            else
            {
                ++i;
            }
        }

        // Symulacja strat 
        for (auto &gracz : gracze)
        {
            for (auto &innyGracz : gracze)
            {
                if (gracz->getTeam() != innyGracz->getTeam())
                {
                    // Każda jednostka atakuje bazę przeciwnika (uproszczone)
                    for (auto &jednostka : gracz->getCreatures())
                    {
                        if (jednostka->isInRange(innyGracz->getBase()))
                        {
                            innyGracz->getBase().takeDamage(10);
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
            drawBase(window, gracz->getBase());
        for (auto &gracz : gracze)
        {
            for (auto &creature : gracz->getCreatures())
                drawUnit(window, *creature);
        }


        window.display();
    }
    // Deallocating memory
    for (auto &gracz : gracze)
    {
        delete gracz;
    }

    return 0;
}