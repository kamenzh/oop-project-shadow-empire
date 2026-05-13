#ifndef SHADOW_EMPIRE_DEALER_H
#define SHADOW_EMPIRE_DEALER_H

#include "models/CrewMember.h"

#include <string>

class Dealer : public CrewMember {
private:
    int capacity;
    int risk;
    std::string assignedTerritoryName;
    bool busy;

public:
    Dealer(int id, const std::string& name, int hirePrice, int salary, int capacity, int risk);

    std::string getRole() const override;
    void assign(const std::string& territoryName);
    void unassign();
    bool isBusy() const;
    void setBusy(bool value);
    bool hasAssignedTerritory() const;
    int getCapacity() const;
    int getRisk() const;
    std::string getAssignedTerritoryName() const;
};

#endif
