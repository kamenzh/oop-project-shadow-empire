#include "models/Dealer.h"

CrewMember::CrewMember(int id, const std::string& name, int hirePrice, int weeklySalary)
    : id(id),
      name(name),
      hirePrice(hirePrice),
      weeklySalary(weeklySalary) {}

int CrewMember::getId() const {
    return id;
}

std::string CrewMember::getName() const {
    return name;
}

int CrewMember::getHirePrice() const {
    return hirePrice;
}

int CrewMember::getWeeklySalary() const {
    return weeklySalary;
}

Dealer::Dealer(int id, const std::string& name, int hirePrice, int salary, int capacity, int risk)
    : CrewMember(id, name, hirePrice, salary),
      capacity(capacity),
      risk(risk),
      assignedTerritoryName(""),
      busy(false) {}

std::string Dealer::getRole() const {
    return "Dealer";
}

void Dealer::assign(const std::string& territoryName) {
    assignedTerritoryName = territoryName;
}

void Dealer::unassign() {
    assignedTerritoryName.clear();
}

bool Dealer::isBusy() const {
    return busy;
}

void Dealer::setBusy(bool value) {
    busy = value;
}

bool Dealer::hasAssignedTerritory() const {
    return !assignedTerritoryName.empty();
}

int Dealer::getCapacity() const {
    return capacity;
}

int Dealer::getRisk() const {
    return risk;
}

std::string Dealer::getAssignedTerritoryName() const {
    return assignedTerritoryName;
}
