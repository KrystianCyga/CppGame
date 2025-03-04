#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <unordered_map>
#include <algorithm>
#include <random>
#include <ctime>
#include <utility>

// Definicja punktu na mapie
struct Point {
    int x;
    int y;

    bool operator==(const Point& other) const {
        return (x == other.x && y == other.y);
    }
};

// Funkcja haszująca dla punktów, niezbędna do używania w unordered_map
struct HashFunction {
    std::size_t operator()(const Point& p) const {
        return std::hash<int>()(p.x) ^ (std::hash<int>()(p.y) << 1);
    }
};

// Definicja węzła do kolejki priorytetowej
struct Node {
    Point point;
    double f; // f = g + h
    double g; // Koszt od startu
    double h; // Heurystyka do celu

    bool operator<(const Node& other) const {
        // Kolejka priorytetowa w C++ jest max-heap, więc odwracamy porównanie
        return f > other.f;
    }
};

// Funkcja heurystyczna - odległość Euklidesowa
double heuristic(const Point& a, const Point& b);

// Implementacja algorytmu A* jako metoda członkowska klasy Map
bool AStar(std::vector<std::vector<char>> tablicaZnakow,const Point& start, const Point& goal);

std::vector<Point> AStarV(const std::vector<std::vector<char>>& tablicaZnakow, const Point& start, const Point& goal);