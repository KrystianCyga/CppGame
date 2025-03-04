#pragma once

#include <stdio.h>
#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <queue>
#include <map>
#include "base.h"
#include "map.h"



static const std::unordered_map<std::string, int> unitCosts = {
    {"Mage", 100},
    {"Goblin", 50},
    {"Elf", 80},
    {"Knight", 120},
    {"Druid", 90}
};

class Creature {
protected:
    unsigned int _hp;
    std::pair<int, int> _localization;
    Teams _team;
    bool _isAlive;
    int _attackRange; // Base attack range
    double _moveSpeed;       // Base move speed
    double _baseDamage;    // Base damage

public:
    Creature(Teams team_ = Teams::NONE, std::pair<int, int> localization_ = {0, 0},
             int hp_ = 100, bool isAlive_ = true, int attackRange_ = 1,double moveSpeed_ = 1.0,double baseDamage_ = 10.0) :
        _hp(hp_), _isAlive(isAlive_), _localization(localization_), _team(team_),
        _attackRange(attackRange_),_moveSpeed(moveSpeed_), _baseDamage(baseDamage_) {}
    virtual ~Creature() = default;
    Creature(const Creature&) = delete;
    Creature& operator=(const Creature&) = delete;

    virtual void move(int x, int y, const Map& map);
    void moveTowards(int targetX, int targetY, const Map& map);
    virtual void attack(Creature& target);
    virtual void takeDamage(double damage);
    virtual double calculateDamage() const { return _baseDamage; }  // Returns damage *before* any modifiers.
    double getDistance(const Creature& other) const; // Distance between two creatures

    // Getters
    unsigned int getHp() const { return _hp; }
    Teams getTeam() const { return _team; }
    std::pair<int, int> getLocalization() const { return _localization; }
    bool isAlive() const { return _isAlive; }
    int getAttackRange() const { return _attackRange; }
    double getMoveSpeed() const { return _moveSpeed; }
    double getBaseDamage() const { return _baseDamage; }

    // Setters
    void setHp(unsigned int hp) { _hp = hp; }
    void setTeam(Teams team) { _team = team; }
    void setLocalization(std::pair<int, int> localization) { _localization = localization; }
    void setIsAlive(bool isAlive) { _isAlive = isAlive; } 

    //Utility
    bool isInRange(const Creature& target) const;
    bool isInRange(const Base& targetBase) const ;

    virtual void specialAbility() {}; 
    virtual std::string getType() const { return "Creature"; }; 
};

void drawUnit(sf::RenderWindow& window, const Creature& creature);
void drawBase(sf::RenderWindow& window, const Base& base);


