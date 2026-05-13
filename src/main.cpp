#include <iostream>
#include "core/Organization.h"
#include "models/Territory.h"

int main() {
    Organization org("Black Raven");

    org.addTerritory(Territory("Old Town", 0, 1));

    std::cout << "Initial status:\n";
    std::cout << org.getStatus() << "\n";

    std::cout << "\nBuying goods...\n";
    org.buyGoods(5);

    std::cout << "\nHiring dealer...\n";
    org.hireDealer("Rico", 1200, 200, 10, 5);

    std::cout << "\nHiring enforcer...\n";
    org.hireEnforcer("Viktor", 1500, 300, 15, 8);

    std::cout << "\nAssigning dealer to Old Town...\n";
    org.assignDealerToTerritory(1, "Old Town");

    std::cout << "\nFinal status:\n";
    std::cout << org.getStatus() << "\n";

    return 0;
}