#include "Astar.h"

double heuristic(const Point& a, const Point& b) {
    return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
}

bool AStar(std::vector<std::vector<char>> tablicaZnakow, const Point& start, const Point& goal) {
    // Checking the validity of the start and goal points

    int sizeX = tablicaZnakow[0].size();
    int sizeY = tablicaZnakow.size();
    if (start.x < 0 || start.x >= sizeX || start.y < 0 || start.y >= sizeY ||
        goal.x < 0 || goal.x >= sizeX || goal.y < 0 || goal.y >= sizeY) {
        std::cerr << "Start or goal point is out of map boundaries.\n";
        return false;
    }

    // Open list as a priority queue
    std::priority_queue<Node> openList;

    // Initialization
    double h_start = heuristic(start, goal);
    Node startNode = { start, h_start, 0.0, h_start };
    openList.push(startNode);

    // Dictionary to track where we came from
    std::unordered_map<Point, Point, HashFunction> cameFrom;

    // Cost to reach a given point
    std::unordered_map<Point, double, HashFunction> costTo;
    costTo[start] = 0.0;

    // Closed set
    std::vector<std::vector<bool>> closedSet(sizeY, std::vector<bool>(sizeX, false));

    // Possible moves: up, down, left, right, and diagonals
    std::vector<Point> movements = {
        { -1,  0 }, // Up
        {  1,  0 }, // Down
        {  0, -1 }, // Left
        {  0,  1 }, // Right
        { -1, -1 }, // Up-left
        { -1,  1 }, // Up-right
        {  1, -1 }, // Down-left
        {  1,  1 }  // Down-right
    };

    while (!openList.empty()) {
        Node currentNode = openList.top();
        openList.pop();
        Point current = currentNode.point;

        // Skip if already visited
        if (closedSet[current.y][current.x])
            continue;

        closedSet[current.y][current.x] = true;

        // Check if we have reached the goal
        if (current == goal) {
            return true; // Path exists
        }

        // Iterate through neighbors
        for (const auto& move : movements) {
            int neighborX = current.x + move.x;
            int neighborY = current.y + move.y;
            Point neighbor = { neighborX, neighborY };

            // Check map boundaries
            if (neighborX < 0 || neighborX >= sizeX || neighborY < 0 || neighborY >= sizeY) {
                continue; // Skip this neighbor
            }

            // Check for obstacles (e.g., 'M' - mountains, 'W' - water)
            if (tablicaZnakow[neighbor.y][neighbor.x] == 'M' || tablicaZnakow[neighbor.y][neighbor.x] == 'W') {
                continue; // Skip obstacle
            }

            // Calculate new cost
            double tentativeG = currentNode.g + ((move.x != 0 && move.y != 0) ? std::sqrt(2.0) : 1.0); // Cost 1 for orthogonal moves, sqrt(2) for diagonal

            // Skip if the point is already in the closed set and the new cost is higher
            if (closedSet[neighbor.y][neighbor.x] && tentativeG >= costTo[neighbor]) {
                continue;
            }

            // If the point is not in costTo or the new cost is lower
            if (costTo.find(neighbor) == costTo.end() || tentativeG < costTo[neighbor]) {
                costTo[neighbor] = tentativeG;
                double priority = tentativeG + heuristic(neighbor, goal);
                Node neighborNode = { neighbor, priority, tentativeG, heuristic(neighbor, goal) };
                openList.push(neighborNode);
                cameFrom[neighbor] = current;
            }
        }
    }

    // If no path was found
    return false;
}

std::vector<Point> AStarV(const std::vector<std::vector<char>>& tablicaZnakow, const Point& start, const Point& goal) {
    std::vector<Point> path; // Inicjalizacja pustej ścieżki
     int sizeX = tablicaZnakow[0].size();
    int sizeY = tablicaZnakow.size();

    if (start.x < 0 || start.x >= sizeX || start.y < 0 || start.y >= sizeY ||
        goal.x < 0 || goal.x >= sizeX || goal.y < 0 || goal.y >= sizeY) {        return path;
    }

    // Open list as a priority queue
    std::priority_queue<Node> openList;

    // Initialization
    double h_start = heuristic(start, goal);
    Node startNode = { start, h_start, 0.0, h_start };
    openList.push(startNode);

    // Dictionary to track where we came from
    std::unordered_map<Point, Point, HashFunction> cameFrom;

    // Cost to reach a given point
    std::unordered_map<Point, double, HashFunction> costTo;
    costTo[start] = 0.0;

    // Closed set
    std::vector<std::vector<bool>> closedSet(sizeY, std::vector<bool>(sizeX, false));

    // Possible moves: up, down, left, right, and diagonals
    std::vector<Point> movements = {
        { -1,  0 }, // Up
        {  1,  0 }, // Down
        {  0, -1 }, // Left
        {  0,  1 }, // Right
        { -1, -1 }, // Up-left
        { -1,  1 }, // Up-right
        {  1, -1 }, // Down-left
        {  1,  1 }  // Down-right
    };

    while (!openList.empty()) {
        Node currentNode = openList.top();
        openList.pop();
        Point current = currentNode.point;

        // Skip if already visited
        if (closedSet[current.y][current.x])
            continue;

        closedSet[current.y][current.x] = true;

        // Check if we have reached the goal
        if (current == goal) {
                           // Reconstruct path

                Point currentPoint = goal;
                while (currentPoint.x != start.x || currentPoint.y != start.y) {
                  path.push_back(currentPoint);
                     if (cameFrom.find(currentPoint) == cameFrom.end()) {
                            std::cerr << "Path reconstruction failed: cameFrom does not contain "
                                         << currentPoint.x << ", " << currentPoint.y << std::endl;
                      }

                   currentPoint = cameFrom[currentPoint];
               }

              std::reverse(path.begin(), path.end());


            return path; // Path exists
        }

        // Iterate through neighbors
        for (const auto& move : movements) {
            int neighborX = current.x + move.x;
            int neighborY = current.y + move.y;
            Point neighbor = { neighborX, neighborY };

            // Check map boundaries
            if (neighborX < 0 || neighborX >= sizeX || neighborY < 0 || neighborY >= sizeY) {
                continue; // Skip this neighbor
            }

            // Check for obstacles (e.g., 'M' - mountains, 'W' - water)
            if (tablicaZnakow[neighbor.y][neighbor.x] == 'M' || tablicaZnakow[neighbor.y][neighbor.x] == 'W') {
                continue; // Skip obstacle
            }

            // Calculate new cost
            double tentativeG = currentNode.g + ((move.x != 0 && move.y != 0) ? std::sqrt(2.0) : 1.0); // Cost 1 for orthogonal moves, sqrt(2) for diagonal

            // Skip if the point is already in the closed set and the new cost is higher
            if (closedSet[neighbor.y][neighbor.x] && tentativeG >= costTo[neighbor]) {
                continue;
            }

            // If the point is not in costTo or the new cost is lower
            if (costTo.find(neighbor) == costTo.end() || tentativeG < costTo[neighbor]) {
                costTo[neighbor] = tentativeG;
                double priority = tentativeG + heuristic(neighbor, goal);
                Node neighborNode = { neighbor, priority, tentativeG, heuristic(neighbor, goal) };
                openList.push(neighborNode);
                cameFrom[neighbor] = current;
            }
        }
    }

    // If no path was found
    return path;
}