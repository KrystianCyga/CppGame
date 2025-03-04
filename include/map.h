#pragma once

#include <stdio.h>
#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <queue>
#include <map>
#include <SFML/Graphics.hpp>
#include "Astar.h"

#define SIZEX 60
#define SIZEY 60

#define TILESIZE 8.0f

#define MIN_DISTANCE_BETWEEN_BASES SIZEX *SIZEY / 40

// forest settings
#define FOREST_CHANCE 0.5
#define NUM_FOREST_CLUSTERS SIZEX *SIZEY / 60
#define MAX_FOREST_CLUSTER_SIZE 50

// mountains settings
#define MOUNTAIN_CHANCE 0.4
#define NUM_MOUNTAIN_CLUSTERS SIZEX *SIZEY / 60
#define MAX_MOUNTAIN_CLUSTER_SIZE 40

// water settings
#define WATER_CHANCE 0.5
#define NUM_WATER_CLUSTERS SIZEX *SIZEY / 60
#define MAX_WATER_CLUSTER_SIZE 40


const std::string RESET = "\033[0m";
const std::string BROWN = "\033[38;5;94m"; // Brunatny
const std::string BLUE = "\033[34m";       // Niebieski
const std::string GREEN = "\033[32m";      // Zielen
const std::string GREY = "\033[90m";       // Gory



enum class TerrainType {
    Z,  // Ziemia
    M,  // Góry
    W,  // Woda
    S,  // Bagno
    F   // Las
};

class Map
{
private:
    std::vector<std::vector<char>> tablicaZnakow;
    std::mt19937 generator;

public:
    Map();
    ~Map();
    void showMap(sf::RenderWindow &window);
    void showMap();
    std::vector<std::pair<int, int>> inicializeMap(int playerNum);
    unsigned int getWidth() const;
    unsigned int getHeight() const;
    std::vector<std::vector<char>> getMap() const;


    void generateMountains();
    void generateWater();
    void generateForest();
    void makeSwamp();
    std::vector<std::pair<int, int>> makeBases(int playerNum);
    bool ifFarEnough(std::vector<std::pair<int, int>> &bazy, int x1, int y1,int playerNum);
    bool isPossibleRouteBetweenPoint(const std::vector<std::pair<int, int>> &bazy, int x1, int y1);
    int calculateCost(int x, int y, int targetX, int targetY) const ;
    TerrainType getTerrain(int x, int y) const;
    bool isTraversable(int x, int y) const ;

};
