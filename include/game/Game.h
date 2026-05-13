#ifndef SHADOW_EMPIRE_GAME_H
#define SHADOW_EMPIRE_GAME_H

#include "core/Organization.h"
#include "models/Territory.h"
#include "systems/SaveManager.h"

#include <string>
#include <vector>

class Game {
private:
    Organization organization;
    SaveManager saveManager;
    std::vector<Territory> availableTerritories;
    bool actionMadeThisWeek;
    bool running;

public:
    Game();

    void start();
    void showMainMenu();
    void showStatus();
    void buyGoods();
    void hireCrewMember();
    void hireDealerMenu();
    void hireEnforcerMenu();
    void manageDealerTerritory();
    void sellGoodsThroughDealer();
    void showPendingDeals();
    void expandTerritory();
    void saveGame();
    void loadGame();
    void rebuildAvailableTerritories();
    void processNextWeek();
    bool checkWinLoseConditions();
    void markActionMade();
    void initializeTerritories();
    int getPoliceEventChance() const;
    bool rollChance(int percent) const;
    void handlePoliceEvents();

    int readInt(const std::string& prompt);
    std::string readLine(const std::string& prompt);
    void waitForEnter();
};

#endif
