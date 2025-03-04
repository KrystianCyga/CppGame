#pragma once
#include "base.h"
#include "mage.h"
#include "knight.h"
#include "druid.h"
#include "goblin.h"
#include "elf.h"
#include <memory>
#include "base.h"
#include <utility>

class Player
{
private:
    Teams _team;
    Base _base;
    unsigned int _gold;
    std::vector<std::unique_ptr<Creature>> _creatures;
    std::mt19937 _rng; // Random number generator 

public:
    Player(Teams team_ = Teams::NONE, std::pair<int, int> localization = {0, 0}, unsigned int gold = 100)
        : _team(team_), _base(team_, localization, 1000), _gold(gold), _rng(std::random_device{}()) {}
    ~Player() = default;
    Player& operator=(const Player&) = delete;

    unsigned int getGold() const { return _gold; }
    void setGold(unsigned int gold) { _gold = gold; }
    Teams getTeam() const { return _team; }
    Base &getBase() { return _base; } // Return by reference to allow modification
    const std::vector<std::unique_ptr<Creature>> &getCreatures() const { return _creatures; }

    void buyRandomUnits(const Map &map);
    void takeTurn(std::vector<Player *> &opponents, const Map &map);
    void removeDeadUnits();

    Player(const Player&) = delete;

     // Move constructor
    Player(Player&& other) noexcept
        : _team(other._team),
          _base(std::move(other._base)),
          _gold(other._gold),
          _creatures(std::move(other._creatures)),
          _rng(std::move(other._rng)) {}
};

