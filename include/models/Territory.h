#ifndef SHADOW_EMPIRE_TERRITORY_H
#define SHADOW_EMPIRE_TERRITORY_H

#include <string>

class Territory {
private:
    std::string name;
    int requiredPower;
    int dealerCapacity;

public:
    Territory(const std::string& name, int requiredPower, int dealerCapacity);

    std::string getName() const;
    int getRequiredPower() const;
    int getDealerCapacity() const;
};

#endif
