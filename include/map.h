#include <stdio.h>
#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <queue>

#define SIZEX 30
#define SIZEY 10

//forest settings
#define FOREST_CHANCE 0.2
#define NUM_FOREST_CLUSTERS SIZEX*SIZEY/60
#define MAX_FOREST_CLUSTER_SIZE  50




#define WATER_CHANCE 0.3
#define MOUNTAIN_CHANCE 0.2

const std::string RESET = "\033[0m";
const std::string BROWN = "\033[38;5;94m";   // Brunatny
const std::string BLUE = "\033[34m";         // Niebieski
const std::string GREEN = "\033[32m";        // Zielen
const std::string GREY = "\033[90m";         // Gory

class map
{
private:
    std::vector<std::vector<char>> tablicaZnakow;
    std::mt19937 generator; 

public:
    map();
    ~map();
    void showMap();
    void inicializeMap();
    void generateMountains();
    void generateWater();
    void generateForest();
};

map::map(/* args */)
{
    tablicaZnakow.resize(SIZEY, std::vector<char>(SIZEX, 'Z'));
}

map::~map()
{
}

void map::showMap()
{
    for (const auto& wiersz : tablicaZnakow)
    {
        for (const auto& znak : wiersz)
        {
            switch (znak)
            {
                case 'Z':
                    std::cout << BROWN << znak << RESET;
                    break;
                case 'W':
                    std::cout << BLUE << znak << RESET;
                    break;
                case 'F':
                    std::cout << GREEN << znak << RESET;
                    break;
                case 'M':
                    std::cout << GREY << znak << RESET;
                    break;
                // Dodaj więcej przypadków dla innych typów terenu
                default:
                    std::cout << znak;
                    break;
            }
        }
        std::cout << std::endl; // Przejście do nowej linii po każdym wierszu
    }
}

void map::generateForest()
{
    std::uniform_int_distribution<int> distY(0, SIZEY - 1);
    std::uniform_int_distribution<int> distX(0, SIZEX - 1);

    std::bernoulli_distribution distribution(FOREST_CHANCE);

    for (int i = 0; i < NUM_FOREST_CLUSTERS; ++i) {
    int seedY = distY(generator);
    int seedX = distX(generator);
    
    if (tablicaZnakow[seedY][seedX] == 'Z') {
        tablicaZnakow[seedY][seedX] = 'F';
        
        // Rozprzestrzenianie się klastrów
        std::queue<std::pair<int, int>> q;
        q.push({seedY, seedX});
        int clusterSize = 1;
        
        while (!q.empty() && clusterSize < MAX_FOREST_CLUSTER_SIZE) {
            auto [y, x] = q.front();
            q.pop();
            
            // Sprawdzenie sąsiadów
            std::vector<std::pair<int, int>> neighbors = {
                {y-1, x}, {y+1, x}, {y, x-1}, {y, x+1},
                {y-1, x-1}, {y-1, x+1}, {y+1, x-1}, {y+1, x+1}
            };
            
            for (auto &[ny, nx] : neighbors) {
                // Sprawdzenie granic mapy
                if (ny >= 0 && ny < SIZEY && nx >= 0 && nx < SIZEX) {
                    if (tablicaZnakow[ny][nx] == 'Z') {
                        // Prawdopodobieństwo dodania do klastru
                        if (distribution(generator)) { 
                            tablicaZnakow[ny][nx] = 'F';
                            q.push({ny, nx});
                            clusterSize++;
                            if (clusterSize >= MAX_FOREST_CLUSTER_SIZE) break;
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
    std::bernoulli_distribution distribution(MOUNTAIN_CHANCE);

    for (int y = 0; y < SIZEY; ++y)
    {
        for (int x = 0; x < SIZEX; ++x)
        {
            if (distribution(generator)&&tablicaZnakow[y][x] == 'Z')
            {
                tablicaZnakow[y][x] = 'M';
            }
        }
    }
}

void map::generateWater()
{
    std::bernoulli_distribution distribution(WATER_CHANCE);

    for (int y = 0; y < SIZEY; ++y)
    {
        for (int x = 0; x < SIZEX; ++x)
        {
            if (distribution(generator)&&tablicaZnakow[y][x] == 'Z')
            {
                tablicaZnakow[y][x] = 'W';
            }
        }
    }
}

void map::inicializeMap(){
    generateForest();
   // generateMountains();
   // generateWater();
    
}
