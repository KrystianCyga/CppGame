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
double heuristic(const Point& a, const Point& b) {
    return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
}


// Implementacja algorytmu A* jako metoda członkowska klasy Map
bool AStar(std::vector<std::vector<char>> tablicaZnakow,const Point& start, const Point& goal) {
    // Sprawdzenie poprawności punktów startowego i docelowego

    int sizeX=tablicaZnakow[0].size();
    int sizeY=tablicaZnakow.size();
    if (start.x < 0 || start.x >= sizeX || start.y < 0 || start.y >= sizeY ||
        goal.x < 0 || goal.x >= sizeX || goal.y < 0 || goal.y >= sizeY) {
        std::cerr << "Start or goal point is out of map boundaries.\n";
        return false;
    }

    // Lista otwarta jako kolejka priorytetowa
    std::priority_queue<Node> openList;

    // Inicjalizacja
    double h_start = heuristic(start, goal);
    Node startNode = { start, h_start, 0.0, h_start };
    openList.push(startNode);

    // Słownik do śledzenia skąd przyszliśmy
    std::unordered_map<Point, Point, HashFunction> cameFrom;

    // Koszt do danego punktu
    std::unordered_map<Point, double, HashFunction> costTo;
    costTo[start] = 0.0;

    // Zbiór zamknięty
    std::vector<std::vector<bool>> closedSet(sizeY, std::vector<bool>(sizeX, false));

    // Możliwe ruchy: góra, dół, lewo, prawo oraz przekątne
    std::vector<Point> movements = {
        { -1,  0 }, // Góra
        {  1,  0 }, // Dół
        {  0, -1 }, // Lewo
        {  0,  1 }, // Prawo
        { -1, -1 }, // Góra-lewo
        { -1,  1 }, // Góra-prawo
        {  1, -1 }, // Dół-lewo
        {  1,  1 }  // Dół-prawo
    };

    while (!openList.empty()) {
        Node currentNode = openList.top();
        openList.pop();
        Point current = currentNode.point;

        // Jeśli już odwiedzone, pomiń
        if (closedSet[current.y][current.x])
            continue;

        closedSet[current.y][current.x] = true;

        // Sprawdzenie, czy osiągnęliśmy cel
        if (current == goal) {
            return true; // Ścieżka istnieje
        }

        // Iteracja przez sąsiadów
        for (const auto& move : movements) {
            int neighborX = current.x + move.x;
            int neighborY = current.y + move.y;
            Point neighbor = { neighborX, neighborY };

            // Sprawdzenie granic mapy
            if (neighborX < 0 || neighborX >= sizeX || neighborY < 0 || neighborY >= sizeY) {
                continue; // Pomiń tego sąsiada
            }

            // Sprawdzenie przeszkód (np. 'M' - góry, 'W' - woda)
            if (tablicaZnakow[neighbor.y][neighbor.x] == 'M' || tablicaZnakow[neighbor.y][neighbor.x] == 'W') {
                continue; // Pomiń przeszkodę
            }

            // Obliczenie nowego kosztu
            double tentativeG = currentNode.g + ((move.x != 0 && move.y != 0) ? std::sqrt(2.0) : 1.0); // Koszt 1 za ruch ortogonalny, sqrt(2) za przekątny

            // Jeśli punkt jest już w zamkniętym zbiorze i nowy koszt jest wyższy, pomiń
            if (closedSet[neighbor.y][neighbor.x] && tentativeG >= costTo[neighbor]) {
                continue;
            }

            // Jeśli punkt nie jest w costTo lub nowy koszt jest niższy
            if (costTo.find(neighbor) == costTo.end() || tentativeG < costTo[neighbor]) {
                costTo[neighbor] = tentativeG;
                double priority = tentativeG + heuristic(neighbor, goal);
                Node neighborNode = { neighbor, priority, tentativeG, heuristic(neighbor, goal) };
                openList.push(neighborNode);
                cameFrom[neighbor] = current;
            }
        }
    }

    // Jeśli nie znaleziono ścieżki
    return false;
}