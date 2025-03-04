#include "creature.h"

#ifndef GOBLIN_H
#define GOBLIN_H

class Goblin : public Creature {
private:
    double _criticalHitChance; // Percentage chance (e.g., 0.2 for 20%)
    double _criticalHitMultiplier;  // Damage multiplier for a critical hit

public:
    Goblin(Teams team_ = Teams::NONE, std::pair<int, int> localization_ = {0, 0}) :
        Creature(team_, localization_, 50, true, 1,1.0,10.0), _criticalHitChance(0.3), _criticalHitMultiplier(2.0) {} // Lower HP, higher damage

    ~Goblin() = default;

    void attack(Creature& target) override;
    double calculateDamage() const override;
    std::string getType() const override { return "Goblin"; }
};
#endif
