#include "models/Enforcer.h"

Enforcer::Enforcer(int id,
                   const std::string& name,
                   int hirePrice,
                   int salary,
                   int powerBonus,
                   int hireHeat)
    : CrewMember(id, name, hirePrice, salary),
      powerBonus(powerBonus),
      hireHeat(hireHeat) {}

std::string Enforcer::getRole() const {
    return "Enforcer";
}

int Enforcer::getPowerBonus() const {
    return powerBonus;
}

int Enforcer::getHireHeat() const {
    return hireHeat;
}
