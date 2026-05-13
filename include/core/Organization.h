#ifndef SHADOW_EMPIRE_ORGANIZATION_H
#define SHADOW_EMPIRE_ORGANIZATION_H

#include "models/CrewMember.h"
#include "models/Dealer.h"
#include "models/Enforcer.h"
#include "models/PendingDeal.h"
#include "models/Territory.h"

#include <memory>
#include <string>
#include <vector>

class Organization {
private:
    std::string name;
    int week;
    int money;
    int goods;
    int heat;
    int power;
    int nextCrewId;
    std::vector<std::unique_ptr<CrewMember>> crew;
    std::vector<Territory> territories;
    std::vector<PendingDeal> pendingDeals;

public:
    explicit Organization(const std::string& name);

    bool buyGoods(int amount);

    void addCrewMember(std::unique_ptr<CrewMember> member);
    bool hireDealer(const std::string& name, int hirePrice, int salary, int capacity, int risk);
    bool hireEnforcer(const std::string& name, int hirePrice, int salary, int powerBonus, int hireHeat);
    void paySalaries();
    int getTotalWeeklySalaries() const;

    void addMoney(int amount);
    bool spendMoney(int amount);
    void addGoods(int amount);
    bool removeGoods(int amount);
    void addHeat(int amount);
    void reduceHeat(int amount);
    void setHeat(int value);
    void addPower(int amount);
    void incrementWeek();
    void setName(const std::string& name);

    void addTerritory(const Territory& territory);
    bool hasTerritory(const std::string& name) const;
    int countDealersInTerritory(const std::string& name) const;
    int getFreeSlotsInTerritory(const std::string& name) const;
    bool assignDealerToTerritory(int dealerId, const std::string& territoryName);

    bool startDealerSale(int dealerId, int goodsAmount);
    void processPendingDeals();
    bool delayRandomPendingDeal(int weeks);
    void delayAllPendingDeals(int weeks);

    Dealer* findDealerById(int id);
    std::vector<Dealer*> getDealers();
    std::vector<Enforcer*> getEnforcers();

    void reduceMoneyPercent(int percent);
    void reduceGoodsPercent(int percent);

    std::string getName() const;
    int getWeek() const;
    int getMoney() const;
    int getGoods() const;
    int getHeat() const;
    int getPower() const;
    int getNextCrewId() const;
    const std::vector<Territory>& getTerritories() const;
    const std::vector<PendingDeal>& getPendingDeals() const;
    std::string getStatus() const;
};

#endif
