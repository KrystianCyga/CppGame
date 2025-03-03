#include <stdio.h>
#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <queue>
#include <map>
#include <SFML/Graphics.hpp>

#define SIZEX 60
#define SIZEY 30

#define TILESIZE 16.0f

#define MIN_DISTANCE_BETWEEN_BASES SIZEX *SIZEY / 80

// forest settings
#define FOREST_CHANCE 0.4
#define NUM_FOREST_CLUSTERS SIZEX *SIZEY / 60
#define MAX_FOREST_CLUSTER_SIZE 50

// mountains settings
#define MOUNTAIN_CHANCE 0.3
#define NUM_MOUNTAIN_CLUSTERS SIZEX *SIZEY / 60
#define MAX_MOUNTAIN_CLUSTER_SIZE 40

// water settings
#define WATER_CHANCE 0.4
#define NUM_WATER_CLUSTERS SIZEX *SIZEY / 60
#define MAX_WATER_CLUSTER_SIZE 40



class map
{
private:
    std::vector<std::vector<char>> tablicaZnakow;
    std::mt19937 generator;

public:
    map();
    ~map();
    void showMap(sf::RenderWindow& window);
    void inicializeMap(int playerNum);
    void generateMountains();
    void generateWater();
    void generateForest();
    void makeSwamp();
    std::vector<std::pair<int, int>> makeBases(int playerNum);
    bool ifFarEnough(std::vector<std::pair<int, int>> &bazy, int x1, int y1);
};

map::map(/* args */)
{
    tablicaZnakow.resize(SIZEY, std::vector<char>(SIZEX, 'Z'));
}

map::~map()
{
}


void map::showMap(sf::RenderWindow& window){
    // Definiowanie kolorów dla poszczególnych znaków
    std::map<char, sf::Color> colorMap = {
        
            {'F', sf::Color(34, 139, 34)},   // las - ciemnozielony (Forest Green)
            {'W', sf::Color(0, 191, 255)},   // woda - jasnoniebieski (Deep Sky Blue)
            {'Z', sf::Color(139, 69, 19)},   // ziemia - brązowy (Saddle Brown)
            {'M', sf::Color(169, 169, 169)}, // góry - szary (Dark Gray)
            {'B', sf::Color(255, 0, 0)},     // bazy - czerwony (Red)
            {'S', sf::Color(107, 152, 35)}   // bagna - oliwkowozielony (Olive Drab)
        
        
        // Dodaj więcej znaków i kolorów w razie potrzeby
    };

     // Rozmiar pojedynczego kafelka

    // Iteracja po tablicy znaków i rysowanie kafelków
    for (size_t y = 0; y < tablicaZnakow.size(); ++y) {
        for (size_t x = 0; x < tablicaZnakow[y].size(); ++x) {
            char znak = tablicaZnakow[y][x];
            sf::RectangleShape tile(sf::Vector2f(TILESIZE, TILESIZE));
            tile.setPosition({x * TILESIZE, y * TILESIZE});

            // Ustawienie koloru na podstawie znaku
            auto it = colorMap.find(znak);
            if (it != colorMap.end()) {
                tile.setFillColor(it->second);
            } else {
                tile.setFillColor(sf::Color::White); // Domyślny kolor, jeśli znak nie jest zmapowany
            }

            // Opcjonalnie: Dodanie obramowania
            tile.setOutlineThickness(1.0f);
            tile.setOutlineColor(sf::Color::Black);

            // Rysowanie kafelka na oknie
            window.draw(tile);
        }
    }
}


void map::generateForest()
{
    std::mt19937 generator(static_cast<unsigned int>(std::time(nullptr)));
    std::uniform_int_distribution<int> distY(0, SIZEY - 1);
    std::uniform_int_distribution<int> distX(0, SIZEX - 1);

    std::bernoulli_distribution distribution(FOREST_CHANCE);
    std::uniform_int_distribution<int> distribution2(1, NUM_FOREST_CLUSTERS);

    for (int i = 0; i < distribution2(generator); ++i)
    {
        int seedY = distY(generator);
        int seedX = distX(generator);

        if (tablicaZnakow[seedY][seedX] == 'Z')
        {
            tablicaZnakow[seedY][seedX] = 'F';

            // Rozprzestrzenianie się klastrów
            std::queue<std::pair<int, int>> q;
            q.push({seedY, seedX});
            int clusterSize = 1;

            while (!q.empty() && clusterSize < MAX_FOREST_CLUSTER_SIZE)
            {
                auto [y, x] = q.front();
                q.pop();

                // Sprawdzenie sąsiadów
                std::vector<std::pair<int, int>> neighbors = {
                    {y - 1, x}, {y + 1, x}, {y, x - 1}, {y, x + 1}, {y - 1, x - 1}, {y - 1, x + 1}, {y + 1, x - 1}, {y + 1, x + 1}};

                for (auto &[ny, nx] : neighbors)
                {
                    // Sprawdzenie granic mapy
                    if (ny >= 0 && ny < SIZEY && nx >= 0 && nx < SIZEX)
                    {
                        if (tablicaZnakow[ny][nx] == 'Z')
                        {
                            // Prawdopodobieństwo dodania do klastru
                            if (distribution(generator))
                            {
                                tablicaZnakow[ny][nx] = 'F';
                                q.push({ny, nx});
                                clusterSize++;
                                if (clusterSize >= MAX_FOREST_CLUSTER_SIZE)
                                    break;
                            }
                        }
                    }
                }
            }
        }
    }
}

void map::generateMountains()
{
    std::mt19937 generator(static_cast<unsigned int>(std::time(nullptr)));
    std::uniform_int_distribution<int> distY(0, SIZEY - 1);
    std::uniform_int_distribution<int> distX(0, SIZEX - 1);

    std::bernoulli_distribution distribution(MOUNTAIN_CHANCE);
    std::uniform_int_distribution<int> distribution2(1, NUM_MOUNTAIN_CLUSTERS);

    for (int i = 0; i < distribution2(generator); ++i)
    {
        int seedY = distY(generator);
        int seedX = distX(generator);

        if (tablicaZnakow[seedY][seedX] == 'Z')
        {
            tablicaZnakow[seedY][seedX] = 'M';

            // Rozprzestrzenianie się klastrów
            std::queue<std::pair<int, int>> q;
            q.push({seedY, seedX});
            int clusterSize = 1;

            while (!q.empty() && clusterSize < MAX_MOUNTAIN_CLUSTER_SIZE)
            {
                auto [y, x] = q.front();
                q.pop();

                // Sprawdzenie sąsiadów
                std::vector<std::pair<int, int>> neighbors = {
                    {y - 1, x}, {y + 1, x}, {y, x - 1}, {y, x + 1}, {y - 1, x - 1}, {y - 1, x + 1}, {y + 1, x - 1}, {y + 1, x + 1}};

                for (auto &[ny, nx] : neighbors)
                {
                    // Sprawdzenie granic mapy
                    if (ny >= 0 && ny < SIZEY && nx >= 0 && nx < SIZEX)
                    {
                        if (tablicaZnakow[ny][nx] == 'Z')
                        {
                            // Prawdopodobieństwo dodania do klastru
                            if (distribution(generator))
                            {
                                tablicaZnakow[ny][nx] = 'M';
                                q.push({ny, nx});
                                clusterSize++;
                                if (clusterSize >= MAX_MOUNTAIN_CLUSTER_SIZE)
                                    break;
                            }
                        }
                    }
                }
            }
        }
    }
}

void map::generateWater()
{
    std::mt19937 generator(static_cast<unsigned int>(std::time(nullptr)));
    std::uniform_int_distribution<int> distY(0, SIZEY - 1);
    std::uniform_int_distribution<int> distX(0, SIZEX - 1);

    std::bernoulli_distribution distribution(WATER_CHANCE);
    std::uniform_int_distribution<int> distribution2(1, NUM_WATER_CLUSTERS);

    for (int i = 0; i < distribution2(generator); ++i)
    {
        int seedY = distY(generator);
        int seedX = distX(generator);

        if (tablicaZnakow[seedY][seedX] == 'Z')
        {
            tablicaZnakow[seedY][seedX] = 'W';

            // Rozprzestrzenianie się klastrów
            std::queue<std::pair<int, int>> q;
            q.push({seedY, seedX});
            int clusterSize = 1;

            while (!q.empty() && clusterSize < MAX_WATER_CLUSTER_SIZE)
            {
                auto [y, x] = q.front();
                q.pop();

                // Sprawdzenie sąsiadów
                std::vector<std::pair<int, int>> neighbors = {
                    {y - 1, x}, {y + 1, x}, {y, x - 1}, {y, x + 1}, {y - 1, x - 1}, {y - 1, x + 1}, {y + 1, x - 1}, {y + 1, x + 1}};

                for (auto &[ny, nx] : neighbors)
                {
                    // Sprawdzenie granic mapy
                    if (ny >= 0 && ny < SIZEY && nx >= 0 && nx < SIZEX)
                    {
                        if (tablicaZnakow[ny][nx] == 'Z')
                        {
                            // Prawdopodobieństwo dodania do klastru
                            if (distribution(generator))
                            {
                                tablicaZnakow[ny][nx] = 'W';
                                q.push({ny, nx});
                                clusterSize++;
                                if (clusterSize >= MAX_WATER_CLUSTER_SIZE)
                                    break;
                            }
                        }
                    }
                }
            }
        }
    }
}

void map::makeSwamp()
{
    std::mt19937 generator(static_cast<unsigned int>(std::time(nullptr)));
    std::bernoulli_distribution distribution(0.1);

    for (size_t i = 0; i < SIZEY; i++)
    {
        for (size_t j = 0; j < SIZEX; j++)
        {

            if (tablicaZnakow[i][j] == 'F') //&&distribution(generator)
            {
                // tablicaZnakow[i][j] = 'B';

                // Rozprzestrzenianie się klastrów
                std::queue<std::pair<int, int>> q;
                q.push({i, j});
                int clusterSize = 1;

                while (!q.empty() && clusterSize < MAX_WATER_CLUSTER_SIZE)
                {
                    auto [y, x] = q.front();
                    q.pop();

                    // Sprawdzenie sąsiadów
                    std::vector<std::pair<int, int>> neighbors = {
                        {y - 1, x}, {y + 1, x}, {y, x - 1}, {y, x + 1}, {y - 1, x - 1}, {y - 1, x + 1}, {y + 1, x - 1}, {y + 1, x + 1}

                    };

                    for (auto &[ny, nx] : neighbors)
                    {
                        // Sprawdzenie granic mapy
                        if (ny >= 0 && ny < SIZEY && nx >= 0 && nx < SIZEX)
                        {
                            if ((tablicaZnakow[ny][nx] == 'Z'))
                            {
                                // Prawdopodobieństwo dodania do klastru
                                if (distribution(generator))
                                {
                                    tablicaZnakow[ny][nx] = 'S';
                                    q.push({ny, nx});
                                    clusterSize++;
                                    if (clusterSize >= MAX_WATER_CLUSTER_SIZE)
                                        break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}


bool map::ifFarEnough(std::vector<std::pair<int, int>> &bazy, int x1, int y1)
{


    for (size_t i = 0; i < bazy.size(); i++)
    {
        if ([](int x1, int y1, int x2, int y2){return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));}
        (bazy[i].first, bazy[i].second, x1, y1) < MIN_DISTANCE_BETWEEN_BASES)
        {

            return false;
        }
    }

    return true;
}

std::vector<std::pair<int, int>> map::makeBases(int playerNum)
{
    std::mt19937 generator(static_cast<unsigned int>(std::time(nullptr)));
    std::uniform_int_distribution<int> distY(0, SIZEY - 1);
    std::uniform_int_distribution<int> distX(0, SIZEX - 1);

    std::bernoulli_distribution distribution(0.05);

    std::vector<std::pair<int, int>> bazy(1);

    for (int i = 0; i < playerNum;)
    {
        int seedY = distY(generator);
        int seedX = distX(generator);

        if (tablicaZnakow[seedY][seedX] == 'Z'&& distribution(generator))
        {
            if (i == 0)
            {
                tablicaZnakow[seedY][seedX] = 'B';
                bazy[i]={seedX, seedY};
                i++;
            }
            else if (ifFarEnough(bazy, seedX, seedY))
            {
                tablicaZnakow[seedY][seedX] = 'B';
                bazy.push_back({seedX, seedY});
                i++;
            }
        }
    }
    return bazy;
}

void map::inicializeMap(int playerNum)
{
    generateForest();
    generateMountains();
    generateWater();
    makeSwamp();
    std::vector<std::pair<int, int>> bazes=makeBases(playerNum);
}
