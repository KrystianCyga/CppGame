
#include "base.h"
#include "mage.h"
#include "knight.h"
#include "druid.h"
#include "goblin.h"
#include "elf.h"
#include <memory> 
#include "base.h"
#include <utility>

#ifndef PLAYER_H
#define PLAYER_H



class Player {
    private:
        Teams _team;
        Base _base;
        unsigned int _gold;
        std::vector<std::unique_ptr<Creature>> _creatures;
        std::mt19937 _rng; // Random number generator for this player
    
    public:
        Player(Teams team_ = Teams::NONE, std::pair<int, int> localization = {0, 0}, unsigned int gold = 100)
            : _team(team_), _base(team_, localization, 1000), _gold(gold), _rng(std::random_device{}()) {}
        ~Player() = default;
    
        unsigned int getGold() const { return _gold; }
        void setGold(unsigned int gold) { _gold = gold; }
        Teams getTeam() const { return _team; }
        Base& getBase() { return _base; } // Return by reference to allow modification
        const std::vector<std::unique_ptr<Creature>>& getCreatures() const { return _creatures; }
    
        void buyRandomUnits(const Map& map);
        void takeTurn(std::vector<Player>& opponents, const Map& map);
        void removeDeadUnits();
    };


std::pair<std::string, int> getRandomUnitType(const std::unordered_map<std::string, int>& unitCosts) {
        // Tworzymy wektor wskaźników do elementów mapy
        std::vector<std::unordered_map<std::string, int>::const_iterator> keys;
        keys.reserve(unitCosts.size()); 
    
        for (auto it = unitCosts.begin(); it != unitCosts.end(); ++it) {
            keys.push_back(it);
        }
    

        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<size_t> dist(0, keys.size() - 1);
    
        // Losujemy iterator do mapy i zwracamy parę (nazwa, koszt)
        auto it = keys[dist(gen)];
        return {it->first, it->second};
}
    

void Player::buyRandomUnits(const Map& map) {

    while (_gold >= 0)
    {

        std::pair<std::string, int> unit = getRandomUnitType(unitCosts);
        std::pair<int, int> spawnlocation=getBase().getLocalization();

        if (_gold >= unit.second && map.isTraversable(spawnlocation.first,spawnlocation.second)) {
            _gold -= unit.second;
            std::unique_ptr<Creature> newUnit;
            if (unit.first == "Mage") newUnit = std::make_unique<Mage>(_team, spawnlocation);
            else if (unit.first == "Goblin") newUnit = std::make_unique<Goblin>(_team,spawnlocation);
            else if (unit.first == "Elf") newUnit = std::make_unique<Elf>(_team, spawnlocation);
            else if (unit.first == "Knight") newUnit = std::make_unique<Knight>(_team, spawnlocation);
            else if (unit.first == "Druid") newUnit = std::make_unique<Druid>(_team, spawnlocation);

            _creatures.push_back(std::move(newUnit));
             std::cout << "Player " << static_cast<int>(_team) << " bought a " << unit.first << " for " << unit.second << " gold.  Remaining gold: " << _gold << std::endl;
        } else {
             std::cout << "Player " << static_cast<int>(_team) << " could not afford a " << unit.first << "." << std::endl;
        }
    }
    
}


#endif