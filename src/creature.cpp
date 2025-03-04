#include "creature.h"
#include "map"
#include <cmath>

double Creature::getDistance(const Creature& other) const {
    int dx = other.getLocalization().first - _localization.first;
    int dy = other.getLocalization().second - _localization.second;
    return std::sqrt(dx * dx + dy * dy);
}

void Creature::move(int x, int y, const Map& map) {
    // Basic move implementation - needs more robust validation and pathfinding
     int newX = _localization.first + x;
        int newY = _localization.second + y;

        // Check map boundaries and traversability
        if (newX >= 0 && newX < map.getWidth() && newY >= 0 && newY < map.getHeight() && map.isTraversable(newX, newY)) {
            _localization.first = newX;
            _localization.second = newY;
        } else {
            // Handle invalid move (e.g., print a message, don't move, try a different direction)
             std::cout << "Invalid move!" << std::endl;
        }
}

void Creature::takeDamage(double damage) {
    _hp -= static_cast<unsigned int>(damage);
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