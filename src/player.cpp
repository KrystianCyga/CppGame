#include "player.h"

std::pair<std::string, int> getRandomUnitType(const std::unordered_map<std::string, int> &unitCosts) {
    // Create a vector of pairs (unit name, unit cost)
    std::vector<std::pair<std::string, int>> unitList;
    unitList.reserve(unitCosts.size());
    for (const auto& pair : unitCosts) {
        unitList.push_back(pair);
    }

    // Return default values if the HashMap is empty
    if (unitList.empty()) {
        return {"", 0};
    }

    // Create random number generator
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dist(0, unitList.size() - 1);

    // Return the random pair
    return unitList[dist(gen)];
}

template <typename T>
T* getRandomElement(const std::vector<T*> &vec, std::mt19937 &rng)
{
    if (vec.empty()) {
        return nullptr;
    }
    std::uniform_int_distribution<size_t> dist(0, vec.size() - 1);
    return vec[dist(rng)];
}

void Player::buyRandomUnits(const Map &map)
{

    while (_gold >= 0)
    {

        std::pair<std::string, int> unit = getRandomUnitType(unitCosts);
        std::pair<int, int> spawnlocation = getBase().getLocalization();

        if (_gold >= unit.second && map.isTraversable(spawnlocation.first, spawnlocation.second))
        {
            _gold -= unit.second;
            std::unique_ptr<Creature> newUnit;
            if (unit.first == "Mage")
                newUnit = std::make_unique<Mage>(_team, spawnlocation);
            else if (unit.first == "Goblin")
                newUnit = std::make_unique<Goblin>(_team, spawnlocation);
            else if (unit.first == "Elf")
                newUnit = std::make_unique<Elf>(_team, spawnlocation);
            else if (unit.first == "Knight")
                newUnit = std::make_unique<Knight>(_team, spawnlocation);
            else if (unit.first == "Druid")
                newUnit = std::make_unique<Druid>(_team, spawnlocation);

            _creatures.push_back(std::move(newUnit));
            std::cout << "Player " << static_cast<int>(_team) << " bought a " << unit.first << " for " << unit.second << " gold.  Remaining gold: " << _gold << std::endl;
        }
        else
        {
            std::cout << "Player " << static_cast<int>(_team) << " could not afford a " << unit.first << "." << std::endl;
            break;
        }
    }
}

void Player::takeTurn(std::vector<Player*>& allPlayers, const Map& map) {
    if (_creatures.empty()) {
        return;
    }

    // Losowo wybierz jednostkę
    std::uniform_int_distribution<> unitDist(0, _creatures.size() - 1);
    int unitIndex = unitDist(_rng);
    Creature& unit = *_creatures[unitIndex];

    if (!unit.isAlive()) return;

    // 1. Sprawdź, czy są wrogowie w pobliżu
    Creature* closestEnemy = nullptr;
    double minDistance = 999999.0; // Some large number

    for (Player* opponent : allPlayers) {
        if (opponent->getTeam() != _team) {
            for (auto& enemyUnit : opponent->getCreatures()) {
                if (enemyUnit->isAlive()) {
                    double distance = unit.getDistance(*enemyUnit);
                    if (distance < minDistance && unit.isInRange(*enemyUnit)) {
                        minDistance = distance;
                        closestEnemy = enemyUnit.get();
                    }
                }
            }
        }
    }

    // 2. Akcja w zależności od sytuacji
    if (closestEnemy != nullptr) {
        //Atakuj wroga
         unit.attack(*closestEnemy);
          std::cout << "Player " << static_cast<int>(_team) << "'s " << unit.getType() << " attacked a " << closestEnemy->getType() << "." << std::endl;
    } else if (unit.getType() == "Druid") {
          Druid* druid = dynamic_cast<Druid*>(&unit);
            if (druid) {
                 druid->specialAbility(allPlayers);
                  std::cout << "Player " << static_cast<int>(_team) << "'s " << unit.getType() << " used their special ability." << std::endl;
            }
    } else {
        // Ruszaj w kierunku najbliższej bazy wroga
        Base* closestBase = nullptr;
        minDistance = 999999.0;

        for (Player* opponent : allPlayers) {
            if (opponent->getTeam() != _team) {
                Creature tempBaseCreature(opponent->getTeam(), opponent->getBase().getLocalization());
                 double distance = unit.getDistance(tempBaseCreature);
                if (distance < minDistance) {
                    minDistance = distance;
                    closestBase = &(opponent->getBase());
                }
            }
        }

        if (closestBase != nullptr) {
             int targetX = closestBase->getLocalization().first;
             int targetY = closestBase->getLocalization().second;
              unit.moveTowards(targetX, targetY, map);
             std::cout << "Player " << static_cast<int>(_team) << "'s " << unit.getType() << " moved towards enemy base." << std::endl;
        }
    }
}

void Player::removeDeadUnits()
{
    _creatures.erase(std::remove_if(_creatures.begin(), _creatures.end(),
                                    [](const std::unique_ptr<Creature> &unit)
                                    { return !unit->isAlive(); }),
                     _creatures.end());
}
