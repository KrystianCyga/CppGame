#include "mage.h"

void Mage::attack(Creature& target) {
    if (!isAlive() || !target.isAlive()) {
            std::cout << "Cannot attack: Attacker or target is dead." << std::endl;
            return;
        }
    if (_mana >= 10) { // Cost of the spell
        if (target.getTeam() != _team) {
            if (isInRange(target))
            {
                double damage = calculateDamage();
                std::cout << "Mage from team " << static_cast<int>(_team) << " casts a spell on " << target.getType() << " from team " << static_cast<int>(target.getTeam()) << " for " << damage << " damage." << std::endl;
                target.takeDamage(damage);
                _mana -= 10;
            } else {
                std::cout << "Mage from team " << static_cast<int>(_team) << " is out of range to attack" << target.getType() << " from team " << static_cast<int>(target.getTeam()) << std::endl;
            }
        }
    } else {
        std::cout << "Mage from team " << static_cast<int>(_team) << " is out of mana!" << std::endl;
    }
}


void Mage::regenerateMana() {
    _mana = std::min(_mana + MANA_REGEN_RATE, MAX_MANA);
    std::cout << "Mage from team " << static_cast<int>(_team)<< " regenerated mana. Current mana: " << _mana << std::endl;
}