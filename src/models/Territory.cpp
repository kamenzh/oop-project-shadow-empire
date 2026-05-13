#include "models/Territory.h"

Territory::Territory(const std::string& name, int requiredPower, int dealerCapacity)
    : name(name),
      requiredPower(requiredPower),
      dealerCapacity(dealerCapacity) {}

std::string Territory::getName() const {
    return name;
}

int Territory::getRequiredPower() const {
    return requiredPower;
}

int Territory::getDealerCapacity() const {
    return dealerCapacity;
}
