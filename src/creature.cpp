#include "creature.h"
#include "map"
#include "Astar.h"
#include <cmath>
#include <SFML/Graphics.hpp>

double Creature::getDistance(const Creature& other) const {
    int dx = other.getLocalization().first - _localization.first;
    int dy = other.getLocalization().second - _localization.second;
    return std::sqrt(dx * dx + dy * dy);
}

double Creature::getDistance(const Point& other) const {
    int dx = other.x - _localization.first;
    int dy = other.y - _localization.second;
    return std::sqrt(dx * dx + dy * dy);
}

void Creature::move(int x, int y, const Map& map) {
    // Basic move implementation - needs more robust validation and pathfinding
        int newX = _localization.first + x;
        int newY = _localization.second + y;
        // Check map boundaries and traversability
            if (map.isTraversable(newX, newY)) {
                _localization.first = newX;
                _localization.second = newY;
            } else {
                std::cout << "Cannot move to non-traversable terrain!" << std::endl;
            }
        
}

void Creature::moveTowards(int targetX, int targetY, const Map& map) {
    // 1. Prepare data for A*
    Point start = {_localization.first, _localization.second};
    Point goal = {targetX, targetY};
    std::vector<std::vector<char>> tablicaZnakow = map.getMap();

    //tablicaZnakow = map.tablicaZnakow;

   // char Map::terrainTypeToChar(TerrainType terrain)
   //std:: cout << "Map " << map.getTerrain();
    
    // 2. Get the path from A*
    std::vector<Point> path = AStarV(tablicaZnakow, start, goal);

    // 3. If a path exists, move towards the first point in the path
    if (!path.empty()) {
        Point nextPoint = path[0];
        int moveX = nextPoint.x - _localization.first;
        int moveY = nextPoint.y - _localization.second;

        move(moveX, moveY, map);
    } else {
        // No path found, stay in place or try a random move
        std::cout << "No path found!" << std::endl;
    }
}

void Creature::takeDamage(double damage) {
    _hp -= static_cast<int>(damage);
    if (_hp <= 0) {
        _hp = 0;
        _isAlive = false;
        std::cout << getType() << " from team " << static_cast<int>(_team) << " died!" << std::endl; // Debugging
    }
}

void Creature::attack(Creature& target) {
    if (!isAlive() || !target.isAlive()) {
            std::cout << "Cannot attack: Attacker or target is dead." << std::endl;
            return;
        }
    if (target.getTeam() != _team) {
        if (isInRange(target))
        {
          double damage = calculateDamage();
          std::cout << getType() << " from team " << static_cast<int>(_team) << " attacks " << target.getType() << " from team " << static_cast<int>(target.getTeam()) << " for " << damage << " damage." << std::endl; // Debugging
          target.takeDamage(damage);
        } else {
          std::cout << getType() << " from team " << static_cast<int>(_team) << " is out of  range to  attack " << target.getType() << " from team " << static_cast<int>(target.getTeam()) << std::endl;
        }
    }
}

bool Creature::isInRange(const Creature& target) const {
    double distance = getDistance(target);
    return distance <= _attackRange;
}

bool Creature::isInRange(const Base& targetBase) const {
     int dx = targetBase.getLocalization().first - _localization.first;
    int dy = targetBase.getLocalization().second - _localization.second;
    double distance = std::sqrt(dx * dx + dy * dy);

    return distance <= _attackRange;
}

void drawUnit(sf::RenderWindow& window, const Creature& creature) {
    sf::CircleShape unitShape(TILESIZE/2); // Radius of 16 pixels
    sf::Vector2f vector;

    vector.x=static_cast<float>(creature.getLocalization().first) * TILESIZE;
    vector.y=static_cast<float>(creature.getLocalization().second) * TILESIZE;

    unitShape.setPosition(vector);

    switch (creature.getTeam()) {
        case Teams::RED: unitShape.setFillColor(sf::Color::Red); break;
        case Teams::BLUE: unitShape.setFillColor(sf::Color::Blue); break;
        case Teams::GREEN: unitShape.setFillColor(sf::Color::Green); break;
        case Teams::YELLOW: unitShape.setFillColor(sf::Color::Yellow); break;
        default: unitShape.setFillColor(sf::Color::White); break;
    }

    window.draw(unitShape);
}

void drawBase(sf::RenderWindow& window, const Base& base) {
    sf::RectangleShape baseRect(sf::Vector2f(TILESIZE, TILESIZE)); 
    sf::Vector2f vector;

    vector.x=static_cast<float>(base.getLocalization().first) * TILESIZE;
    vector.y=static_cast<float>(base.getLocalization().second) * TILESIZE;
    baseRect.setPosition(vector);

    // Set color based on team
    switch (base.getTeam()) {
        case Teams::RED: baseRect.setFillColor(sf::Color::Red); break;
        case Teams::BLUE: baseRect.setFillColor(sf::Color::Blue); break;
        case Teams::GREEN: baseRect.setFillColor(sf::Color::Green); break;
        case Teams::YELLOW: baseRect.setFillColor(sf::Color::Yellow); break;
        default: baseRect.setFillColor(sf::Color::White); break;
    }

    // Draw health bar 
    sf::RectangleShape healthBar(sf::Vector2f(TILESIZE * base.getHp() / 1000.0f, 4));// 4 pixels height
 
    vector.y=static_cast<float>(base.getLocalization().second) * TILESIZE-4;
    healthBar.setPosition(vector);
    healthBar.setFillColor(sf::Color::Green);

    window.draw(baseRect);
    window.draw(healthBar);
}