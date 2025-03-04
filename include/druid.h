#include "creature.h"

#ifndef DRUID_H
#define DRUID_H

class Druid : public Creature {
private:
    double _healingMultiplier;
public:
    Druid(Teams team_ = Teams::NONE, std::pair<int, int> localization_ = {0, 0}) :
     Creature(team_, localization_, 60, true, 1, 1.0, 8.0),_healingMultiplier(0.2) {}

    ~Druid() = default;

    void specialAbility() override; // Heals the weakest nearby ally
    std::string getType() const override { return "Druid"; }
    double getHealingMultiplier() const { return _healingMultiplier; }
};
#endif
