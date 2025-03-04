#include "../include/map.h"
#include <SFML/Graphics.hpp>
#include "base.h"
#include "player.h"

int main (){

    

    const unsigned int x=SIZEX*TILESIZE;
    const unsigned int y=SIZEY*TILESIZE;
    auto window = sf::RenderWindow(sf::VideoMode({x, y}), "Mapa Gry z SFML");
    window.setFramerateLimit(60);

    Map myMap;
    unsigned int iloscGraczy=4;

    std::vector<std::pair<int, int>> bazy=myMap.inicializeMap(iloscGraczy);
    std::vector<Player> gracze;

    for (size_t i = 0; i < bazy.size(); i++)
    {
        gracze.emplace_back(static_cast<Teams>(i + 1), bazy[i],100);
        
    }
    
    

    // Główna pętla gry
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
    }


    return 0;
}