#ifndef SHADOW_EMPIRE_ENFORCER_H
#define SHADOW_EMPIRE_ENFORCER_H

#include "models/CrewMember.h"

#include <string>

class Enforcer : public CrewMember {
private:
    int powerBonus;
    int hireHeat;

public:
    Enforcer(int id, const std::string& name, int hirePrice, int salary, int powerBonus, int hireHeat);

    std::string getRole() const override;
    int getPowerBonus() const;
    int getHireHeat() const;
};

#endif
