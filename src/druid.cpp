#include "druid.h"

#include "druid.h"
#include <iostream>
#include <vector>
#include <algorithm>

void Druid::specialAbility() {
    // Find the weakest ally within a certain range
    std::vector<Creature*> alliesInRange;

    // Assuming you have a way to access all creatures in the game
    //   For example, a global list or a game manager
    //   This is placeholder code - replace with your actual game data
    std::vector<Creature*> allCreatures; // Replace with your actual game list
   //GameManager::getInstance().getAllCreatures(allCreatures);

    for (Creature* creature : allCreatures) {
        if (creature->getTeam() == getTeam() && creature != this) { // Same team and not self
            double distance = getDistance(*creature);
            if (distance <= 3 && creature->isAlive()) { // Example range of 3 units
                alliesInRange.push_back(creature);
            }
        }
    }

    // Find the ally with the lowest HP
    Creature* weakestAlly = nullptr;
    for (Creature* ally : alliesInRange) {
        if (weakestAlly == nullptr || ally->getHp() < weakestAlly->getHp()) {
            weakestAlly = ally;
        }
    }

    // Heal the weakest ally
    if (weakestAlly != nullptr) {
        double healAmount = getBaseDamage() * getHealingMultiplier();  // Example: Based on druid's damage and a multiplier
        weakestAlly->setHp(std::min(weakestAlly->getHp() + static_cast<unsigned int>(healAmount), 100u)); // Heal, but don't exceed max HP
         std::cout << "Druid from team " << static_cast<int>(getTeam()) << " heals " << weakestAlly->getType() << " from team " << static_cast<int>(getTeam()) << " for " << healAmount << " HP." << std::endl;
    } else {
        std::cout << "No allies in range to heal." << std::endl;
    }
}