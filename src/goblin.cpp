#include "goblin.h"


void Goblin::attack(Creature& target) {
    if (!isAlive() || !target.isAlive()) {
            std::cout << "Cannot attack: Attacker or target is dead." << std::endl;
            return;
        }
    if (target.getTeam() != _team) {
         if (isInRange(target))
        {
                double damage = calculateDamage();
                std::cout << "Goblin from team " << static_cast<int>(_team) << " attacks " << target.getType() << " from team " << static_cast<int>(target.getTeam()) << " for " << damage << " damage." << std::endl;
                target.takeDamage(damage);
         } else {
           std::cout << "Goblin from team " << static_cast<int>(_team) << " is out of range to attack" << target.getType() << " from team " << static_cast<int>(target.getTeam()) << std::endl;
         }
    }
}

double Goblin::calculateDamage() const {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> distrib(0.0, 1.0);

    double baseDamage = Creature::calculateDamage(); // Get base damage
    if (distrib(gen) <= _criticalHitChance) {
        std::cout << "Critical hit!" << std::endl;
        return baseDamage * _criticalHitMultiplier;
    } else {
        return baseDamage;
    }
}