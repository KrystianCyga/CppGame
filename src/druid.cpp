#include "druid.h"
#include "player.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>

void Druid::specialAbility(std::vector<Player*>& allPlayers) {
    // Find the weakest ally within a certain range
    std::vector<Creature*> alliesInRange;

    // Access all creatures in the game through the provided vector of players
    for (Player* player : allPlayers) {
        if (player->getTeam() == getTeam()) {
            const auto& creatures = player->getCreatures(); // Get creatures of this player
            for (const auto& creaturePtr : creatures) { // Iterating through unique_ptr’s
                Creature* creature = creaturePtr.get(); // Get raw pointer
                if (creature != this && creature->isAlive()) { // Same team, not self, and alive
                    double distance = getDistance(*creature);
                    if (distance <= 3) { // Example range of 3 units
                        alliesInRange.push_back(creature);
                    }
                }
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
        double healAmount = getBaseDamage() * getHealingMultiplier();
        unsigned int maxHeal =100 ; //Max Heal Amount
        unsigned int newHP = std::min(weakestAlly->getHp() + static_cast<unsigned int>(healAmount), maxHeal);
        weakestAlly->setHp(newHP); // Heal, but don't exceed max HP
        std::cout << "Druid from team " << static_cast<int>(getTeam()) << " heals " << weakestAlly->getType() << " from team " << static_cast<int>(getTeam()) << " for " << healAmount << " HP. New HP is "<< newHP << std::endl;
    } else {
        std::cout << "No allies in range to heal." << std::endl;
    }
}