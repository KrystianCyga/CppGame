#include "../include/map.h"
#include <SFML/Graphics.hpp>

int main (){

    const unsigned int x=SIZEX*TILESIZE;
    const unsigned int y=SIZEY*TILESIZE;
    auto window = sf::RenderWindow(sf::VideoMode({x, y}), "Mapa Gry z SFML");
    window.setFramerateLimit(60);

    map myMap;
    myMap.inicializeMap(3);

    

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