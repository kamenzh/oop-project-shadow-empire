#include "core/Organization.h"

#include "utils/Constants.h"

#include <algorithm>
#include <sstream>

Organization::Organization(const std::string& name)
    : name(name),
      week(1),
      money(5000),
      goods(10),
      heat(0),
      power(0),
      nextCrewId(1) {}

bool Organization::buyGoods(int amount) {
    if (amount <= 0) {
        return false;
    }

    const int totalCost = amount * BUY_PRICE_PER_GOOD;
    if (!spendMoney(totalCost)) {
        return false;
    }

    addGoods(amount);
    return true;
}

void Organization::addCrewMember(std::unique_ptr<CrewMember> member) {
    if (member) {
        crew.push_back(std::move(member));
    }
}

bool Organization::hireDealer(const std::string& name,
                              int hirePrice,
                              int salary,
                              int capacity,
                              int risk) {
    if (!spendMoney(hirePrice)) {
        return false;
    }

    addCrewMember(std::make_unique<Dealer>(nextCrewId, name, hirePrice, salary, capacity, risk));
    ++nextCrewId;
    return true;
}

bool Organization::hireEnforcer(const std::string& name,
                                int hirePrice,
                                int salary,
                                int powerBonus,
                                int hireHeat) {
    if (!spendMoney(hirePrice)) {
        return false;
    }

    addCrewMember(std::make_unique<Enforcer>(nextCrewId, name, hirePrice, salary, powerBonus, hireHeat));
    ++nextCrewId;
    addPower(powerBonus);
    addHeat(hireHeat);
    return true;
}

void Organization::paySalaries() {
    int totalSalary = 0;
    for (const auto& member : crew) {
        totalSalary += member->getWeeklySalary();
    }

    spendMoney(totalSalary);
}

void Organization::addMoney(int amount) {
    if (amount > 0) {
        money += amount;
    }
}

bool Organization::spendMoney(int amount) {
    if (amount <= 0 || amount > money) {
        return false;
    }

    money -= amount;
    return true;
}

void Organization::addGoods(int amount) {
    if (amount > 0) {
        goods += amount;
    }
}

bool Organization::removeGoods(int amount) {
    if (amount <= 0 || amount > goods) {
        return false;
    }

    goods -= amount;
    return true;
}

void Organization::addHeat(int amount) {
    if (amount > 0) {
        heat += amount;
    }
}

void Organization::reduceHeat(int amount) {
    if (amount > 0) {
        heat = std::max(0, heat - amount);
    }
}

void Organization::setHeat(int value) {
    heat = std::max(0, value);
}

void Organization::addPower(int amount) {
    if (amount > 0) {
        power += amount;
    }
}

void Organization::incrementWeek() {
    ++week;
}

void Organization::setName(const std::string& name) {
    this->name = name;
}

void Organization::addTerritory(const Territory& territory) {
    territories.push_back(territory);
}

bool Organization::hasTerritory(const std::string& name) const {
    return std::any_of(territories.begin(), territories.end(), [&name](const Territory& territory) {
        return territory.getName() == name;
    });
}

int Organization::countDealersInTerritory(const std::string& name) const {
    int count = 0;
    for (const auto& member : crew) {
        const Dealer* dealer = dynamic_cast<const Dealer*>(member.get());
        if (dealer && dealer->getAssignedTerritoryName() == name) {
            ++count;
        }
    }

    return count;
}

int Organization::getFreeSlotsInTerritory(const std::string& name) const {
    for (const auto& territory : territories) {
        if (territory.getName() == name) {
            return std::max(0, territory.getDealerCapacity() - countDealersInTerritory(name));
        }
    }

    return 0;
}

bool Organization::assignDealerToTerritory(int dealerId, const std::string& territoryName) {
    Dealer* dealer = findDealerById(dealerId);
    if (!dealer || !hasTerritory(territoryName)) {
        return false;
    }

    if (dealer->getAssignedTerritoryName() == territoryName) {
        return true;
    }

    if (dealer->isBusy() || getFreeSlotsInTerritory(territoryName) <= 0) {
        return false;
    }

    dealer->assign(territoryName);
    return true;
}

bool Organization::startDealerSale(int dealerId, int goodsAmount) {
    (void)dealerId;
    (void)goodsAmount;
    return false;
}

void Organization::processPendingDeals() {}

bool Organization::delayRandomPendingDeal(int weeks) {
    (void)weeks;
    return false;
}

void Organization::delayAllPendingDeals(int weeks) {
    (void)weeks;
}

Dealer* Organization::findDealerById(int id) {
    for (const auto& member : crew) {
        Dealer* dealer = dynamic_cast<Dealer*>(member.get());
        if (dealer && dealer->getId() == id) {
            return dealer;
        }
    }

    return nullptr;
}

std::vector<Dealer*> Organization::getDealers() {
    std::vector<Dealer*> dealers;
    for (const auto& member : crew) {
        Dealer* dealer = dynamic_cast<Dealer*>(member.get());
        if (dealer) {
            dealers.push_back(dealer);
        }
    }

    return dealers;
}

std::vector<Enforcer*> Organization::getEnforcers() {
    std::vector<Enforcer*> enforcers;
    for (const auto& member : crew) {
        Enforcer* enforcer = dynamic_cast<Enforcer*>(member.get());
        if (enforcer) {
            enforcers.push_back(enforcer);
        }
    }

    return enforcers;
}

void Organization::reduceMoneyPercent(int percent) {
    if (percent > 0) {
        money -= money * std::min(percent, 100) / 100;
    }
}

void Organization::reduceGoodsPercent(int percent) {
    if (percent > 0) {
        goods -= goods * std::min(percent, 100) / 100;
    }
}

std::string Organization::getName() const {
    return name;
}

int Organization::getWeek() const {
    return week;
}

int Organization::getMoney() const {
    return money;
}

int Organization::getGoods() const {
    return goods;
}

int Organization::getHeat() const {
    return heat;
}

int Organization::getPower() const {
    return power;
}

int Organization::getNextCrewId() const {
    return nextCrewId;
}

std::string Organization::getStatus() const {
    std::ostringstream output;

    output << "Organization: " << name << '\n'
           << "Week: " << week << '\n'
           << "Money: " << money << '\n'
           << "Goods: " << goods << '\n'
           << "Heat: " << heat << '\n'
           << "Power: " << power << '\n';

    output << "\nDealers:\n";
    bool hasDealers = false;
    for (const auto& member : crew) {
        const Dealer* dealer = dynamic_cast<const Dealer*>(member.get());
        if (dealer) {
            hasDealers = true;
            output << "  [" << dealer->getId() << "] " << dealer->getName()
                   << " | territory: "
                   << (dealer->hasAssignedTerritory() ? dealer->getAssignedTerritoryName() : "None")
                   << " | capacity: " << dealer->getCapacity()
                   << " | risk: " << dealer->getRisk()
                   << " | busy: " << (dealer->isBusy() ? "yes" : "no") << '\n';
        }
    }
    if (!hasDealers) {
        output << "  None\n";
    }

    output << "\nEnforcers:\n";
    bool hasEnforcers = false;
    for (const auto& member : crew) {
        const Enforcer* enforcer = dynamic_cast<const Enforcer*>(member.get());
        if (enforcer) {
            hasEnforcers = true;
            output << "  [" << enforcer->getId() << "] " << enforcer->getName()
                   << " | powerBonus: " << enforcer->getPowerBonus()
                   << " | hireHeat: " << enforcer->getHireHeat() << '\n';
        }
    }
    if (!hasEnforcers) {
        output << "  None\n";
    }

    output << "\nTerritories:\n";
    if (territories.empty()) {
        output << "  None\n";
    } else {
        for (const auto& territory : territories) {
            output << "  " << territory.getName()
                   << " | requiredPower: " << territory.getRequiredPower()
                   << " | capacity: " << territory.getDealerCapacity()
                   << " | freeSlots: " << getFreeSlotsInTerritory(territory.getName()) << '\n';
        }
    }

    return output.str();
}
