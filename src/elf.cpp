#include "elf.h"

#include <iostream>

void Elf::move(int x, int y, const Map& map) {

    int newX = getLocalization().first + x;
    int newY = getLocalization().second + y;

    // Move faster in forests
    double moveSpeed = getMoveSpeed();
    if (map.getTerrain(newX, newY) == TerrainType::F) {
        moveSpeed *= 1.5; // Bonus: 50% faster
       std::cout << "Elf from team " << static_cast<int>(getTeam()) << " moves faster through the forest!" << std::endl;
    }
         if (newX >= 0 && newX < map.getWidth() && newY >= 0 && newY < map.getHeight() && map.isTraversable(newX, newY)) {
            setLocalization({newX, newY});
        } else {
            // Invalid move 
             std::cout << "Invalid move!" << std::endl;
        }
}


void Elf::attack(Creature& target) {
     if (!isAlive() || !target.isAlive()) {
            std::cout << "Cannot attack: Attacker or target is dead." << std::endl;
            return;
        }
    // Elves can only attack at range
    if (getDistance(target) <= 1.5) { // Can't attack if closer than 1.5 units
      std::cout << "Elf from team " << static_cast<int>(getTeam()) << " is too close to attack!" << std::endl;
        return;
    }

    if (target.getTeam() != getTeam()) {
          if (isInRange(target))
        {
             double damage = calculateDamage();
             std::cout << "Elf from team " << static_cast<int>(getTeam()) << " fires an arrow at" << target.getType() << " from team " << static_cast<int>(target.getTeam()) << " for " << damage << " damage." << std::endl;
             target.takeDamage(damage);
          } else {
            std::cout << "Elf from team " << static_cast<int>(getTeam()) << " is out of range to attack" << target.getType() << " from team " << static_cast<int>(target.getTeam()) << std::endl;
          }

    }
}