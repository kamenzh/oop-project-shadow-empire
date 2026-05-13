#include "game/Game.h"

#include "systems/CrackdownEvent.h"
#include "systems/DealerQuestionedEvent.h"
#include "systems/GameEvent.h"
#include "systems/PolicePatrolEvent.h"
#include "utils/Constants.h"

#include <iostream>
#include <limits>
#include <memory>
#include <random>
#include <string>

namespace {
struct DealerTemplate {
    std::string name;
    int price;
    int salary;
    int capacity;
    int risk;
};

struct EnforcerTemplate {
    std::string name;
    int price;
    int salary;
    int powerBonus;
    int hireHeat;
};

std::mt19937& gameRng() {
    static std::mt19937 rng(std::random_device{}());
    return rng;
}
}

Game::Game()
    : organization("Black Raven"),
      actionMadeThisWeek(false),
      running(true) {
    organization.addTerritory(Territory("Old Town", 0, 1));
    initializeTerritories();
}

void Game::start() {
    std::cout << "Welcome to Shadow Empire!\n\n";

    const std::string organizationName = readLine("Organization name (leave empty for Black Raven): ");
    if (!organizationName.empty()) {
        organization.setName(organizationName);
    }

    while (running && !checkWinLoseConditions()) {
        showMainMenu();
    }
}

void Game::showMainMenu() {
    std::cout << "\n==== Shadow Empire ====\n"
              << "1. Show Status\n"
              << "2. Buy Goods\n"
              << "3. Hire Crew Member\n"
              << "4. Manage Dealer Territory\n"
              << "5. Sell Goods Through Dealer\n"
              << "6. Show Pending Deals\n"
              << "7. Expand Territory\n"
              << "8. Next Week\n"
              << "9. Exit\n";

    const int choice = readInt("Choose an option: ");
    std::cout << '\n';

    switch (choice) {
    case 1:
        showStatus();
        break;
    case 2:
        buyGoods();
        break;
    case 3:
        hireCrewMember();
        break;
    case 4:
        manageDealerTerritory();
        break;
    case 5:
        sellGoodsThroughDealer();
        break;
    case 6:
        showPendingDeals();
        break;
    case 7:
        expandTerritory();
        break;
    case 8:
        processNextWeek();
        break;
    case 9:
        std::cout << "Exiting Shadow Empire.\n";
        running = false;
        return;
    default:
        std::cout << "Invalid menu option.\n";
        break;
    }

    if (choice == 9) {
        return;
    }

    waitForEnter();
}

void Game::showStatus() {
    std::cout << organization.getStatus();
}

void Game::buyGoods() {
    const int quantity = readInt("Goods quantity to buy: ");
    if (quantity <= 0) {
        std::cout << "Quantity must be greater than 0.\n";
        return;
    }

    if (organization.buyGoods(quantity)) {
        std::cout << "Bought " << quantity << " goods.\n";
        markActionMade();
    } else {
        std::cout << "Purchase failed. Not enough money or invalid purchase.\n";
    }
}

void Game::hireCrewMember() {
    while (true) {
        std::cout << "\nHire Crew Member\n"
                  << "1. Hire Dealer\n"
                  << "2. Hire Enforcer\n"
                  << "3. Back\n";

        const int choice = readInt("Choose an option: ");
        std::cout << '\n';

        switch (choice) {
        case 1:
            hireDealerMenu();
            return;
        case 2:
            hireEnforcerMenu();
            return;
        case 3:
            return;
        default:
            std::cout << "Invalid menu option.\n";
            break;
        }
    }
}

void Game::hireDealerMenu() {
    const std::vector<DealerTemplate> templates = {
        {"Cheap Dealer", 600, 100, 5, 8},
        {"Street Dealer", 1200, 200, 10, 5},
        {"Professional Dealer", 2500, 400, 18, 3},
    };

    std::cout << "Dealer Templates\n";
    for (std::size_t i = 0; i < templates.size(); ++i) {
        const DealerTemplate& option = templates[i];
        std::cout << i + 1 << ". " << option.name
                  << " | price: " << option.price
                  << " | salary: " << option.salary
                  << " | capacity: " << option.capacity
                  << " | risk: " << option.risk << '\n';
    }

    const int choice = readInt("Choose a dealer template: ");
    if (choice < 1 || choice > static_cast<int>(templates.size())) {
        std::cout << "Invalid dealer template.\n";
        return;
    }

    const DealerTemplate& selected = templates[choice - 1];
    std::string name = readLine("Dealer name (leave empty for template name): ");
    if (name.empty()) {
        name = selected.name;
    }

    if (organization.hireDealer(name, selected.price, selected.salary, selected.capacity, selected.risk)) {
        std::cout << "Hired dealer: " << name << ".\n";
        markActionMade();
    } else {
        std::cout << "Not enough money to hire this dealer.\n";
    }
}

void Game::hireEnforcerMenu() {
    const std::vector<EnforcerTemplate> templates = {
        {"Local Enforcer", 1000, 250, 10, 6},
        {"Heavy Enforcer", 2200, 450, 22, 12},
        {"Elite Enforcer", 4000, 700, 40, 20},
    };

    std::cout << "Enforcer Templates\n";
    for (std::size_t i = 0; i < templates.size(); ++i) {
        const EnforcerTemplate& option = templates[i];
        std::cout << i + 1 << ". " << option.name
                  << " | price: " << option.price
                  << " | salary: " << option.salary
                  << " | powerBonus: " << option.powerBonus
                  << " | hireHeat: " << option.hireHeat << '\n';
    }

    const int choice = readInt("Choose an enforcer template: ");
    if (choice < 1 || choice > static_cast<int>(templates.size())) {
        std::cout << "Invalid enforcer template.\n";
        return;
    }

    const EnforcerTemplate& selected = templates[choice - 1];
    std::string name = readLine("Enforcer name (leave empty for template name): ");
    if (name.empty()) {
        name = selected.name;
    }

    if (organization.hireEnforcer(name, selected.price, selected.salary, selected.powerBonus, selected.hireHeat)) {
        std::cout << "Hired enforcer: " << name << ".\n";
        markActionMade();
    } else {
        std::cout << "Not enough money to hire this enforcer.\n";
    }
}

void Game::manageDealerTerritory() {
    std::vector<Dealer*> dealers = organization.getDealers();
    if (dealers.empty()) {
        std::cout << "No dealers available. Hire a dealer first.\n";
        return;
    }

    std::cout << "Dealers\n";
    for (const Dealer* dealer : dealers) {
        std::cout << "[" << dealer->getId() << "] " << dealer->getName()
                  << " | Territory: "
                  << (dealer->hasAssignedTerritory() ? dealer->getAssignedTerritoryName() : "None")
                  << " | Busy: " << (dealer->isBusy() ? "yes" : "no") << '\n';
    }

    const int dealerId = readInt("Enter dealer ID: ");
    Dealer* selectedDealer = organization.findDealerById(dealerId);
    if (!selectedDealer) {
        std::cout << "Dealer not found.\n";
        return;
    }

    if (selectedDealer->isBusy()) {
        std::cout << "This dealer is busy and cannot be moved.\n";
        return;
    }

    const std::vector<Territory>& territories = organization.getTerritories();
    if (territories.empty()) {
        std::cout << "No unlocked territories available.\n";
        return;
    }

    std::cout << "\nUnlocked Territories\n";
    for (std::size_t i = 0; i < territories.size(); ++i) {
        const Territory& territory = territories[i];
        std::cout << i + 1 << ". " << territory.getName()
                  << " | Required power: " << territory.getRequiredPower()
                  << " | Capacity: " << territory.getDealerCapacity()
                  << " | Free slots: " << organization.getFreeSlotsInTerritory(territory.getName())
                  << '\n';
    }

    const int territoryChoice = readInt("Choose a territory: ");
    if (territoryChoice < 1 || territoryChoice > static_cast<int>(territories.size())) {
        std::cout << "Invalid territory choice.\n";
        return;
    }

    const Territory& selectedTerritory = territories[territoryChoice - 1];
    const std::string territoryName = selectedTerritory.getName();

    if (selectedDealer->getAssignedTerritoryName() == territoryName) {
        std::cout << "Dealer is already assigned to this territory.\n";
        return;
    }

    if (organization.getFreeSlotsInTerritory(territoryName) <= 0) {
        std::cout << "This territory has no free dealer slots.\n";
        return;
    }

    if (organization.assignDealerToTerritory(dealerId, territoryName)) {
        std::cout << "Dealer assigned successfully.\n";
        markActionMade();
    } else {
        std::cout << "Could not assign dealer to that territory.\n";
    }
}

void Game::sellGoodsThroughDealer() {
    std::vector<Dealer*> dealers = organization.getDealers();
    if (dealers.empty()) {
        std::cout << "No dealers available. Hire a dealer first.\n";
        return;
    }

    std::cout << "Dealers\n";
    for (const Dealer* dealer : dealers) {
        std::cout << "[" << dealer->getId() << "] " << dealer->getName()
                  << " | Territory: "
                  << (dealer->hasAssignedTerritory() ? dealer->getAssignedTerritoryName() : "None")
                  << " | Capacity: " << dealer->getCapacity()
                  << " | Risk: " << dealer->getRisk()
                  << " | Busy: " << (dealer->isBusy() ? "yes" : "no") << '\n';
    }

    const int dealerId = readInt("Enter dealer ID: ");
    Dealer* dealer = organization.findDealerById(dealerId);
    if (!dealer) {
        std::cout << "Dealer not found.\n";
        return;
    }

    const int goodsAmount = readInt("Goods amount to sell: ");
    if (goodsAmount <= 0) {
        std::cout << "Goods amount must be greater than 0.\n";
        return;
    }

    if (dealer->isBusy()) {
        std::cout << "This dealer is busy and cannot start another sale.\n";
        return;
    }

    if (!dealer->hasAssignedTerritory()) {
        std::cout << "This dealer must be assigned to a territory first.\n";
        return;
    }

    if (goodsAmount > dealer->getCapacity()) {
        std::cout << "This dealer cannot carry that many goods.\n";
        return;
    }

    if (goodsAmount > organization.getGoods()) {
        std::cout << "Not enough goods available.\n";
        return;
    }

    if (organization.startDealerSale(dealerId, goodsAmount)) {
        std::cout << "Deal started successfully. Payment will arrive next week.\n"
                  << "Expected payment: " << goodsAmount * SELL_PRICE_PER_GOOD << '\n';
        markActionMade();
    } else {
        std::cout << "Could not start dealer sale.\n";
    }
}

void Game::showPendingDeals() {
    const std::vector<PendingDeal>& pendingDeals = organization.getPendingDeals();
    if (pendingDeals.empty()) {
        std::cout << "No pending deals.\n";
        return;
    }

    std::cout << "Pending Deals\n";
    for (const auto& deal : pendingDeals) {
        std::cout << deal.getDealerName()
                  << " | Territory: " << deal.getTerritoryName()
                  << " | Goods: " << deal.getGoodsAmount()
                  << " | Payment: " << deal.getPayment()
                  << " | Weeks remaining: " << deal.getWeeksRemaining()
                  << " | Heat risk: " << deal.getHeatRisk() << '\n';
    }
}

void Game::expandTerritory() {
    if (availableTerritories.empty()) {
        std::cout << "No more territories available.\n";
        return;
    }

    std::cout << "Current Power: " << organization.getPower() << "\n\n";
    for (std::size_t i = 0; i < availableTerritories.size(); ++i) {
        const Territory& territory = availableTerritories[i];
        std::cout << i + 1 << ". " << territory.getName()
                  << " | Required Power: " << territory.getRequiredPower()
                  << " | Dealer Capacity: " << territory.getDealerCapacity()
                  << '\n';
    }

    const int choice = readInt("Choose a territory to unlock: ");
    if (choice < 1 || choice > static_cast<int>(availableTerritories.size())) {
        std::cout << "Invalid territory choice.\n";
        return;
    }

    const std::size_t selectedIndex = static_cast<std::size_t>(choice - 1);
    const Territory selectedTerritory = availableTerritories[selectedIndex];

    if (organization.getPower() < selectedTerritory.getRequiredPower()) {
        std::cout << "Not enough Power to unlock this territory.\n";
        return;
    }

    organization.addTerritory(selectedTerritory);
    availableTerritories.erase(availableTerritories.begin() + selectedIndex);

    std::cout << "Territory unlocked successfully.\n";
    markActionMade();
}

void Game::processNextWeek() {
    const int finishingWeek = organization.getWeek();
    const int startingMoney = organization.getMoney();
    const int startingGoods = organization.getGoods();
    const int startingHeat = organization.getHeat();
    const int startingPower = organization.getPower();
    const int startingPendingDeals = static_cast<int>(organization.getPendingDeals().size());
    const int totalSalaries = organization.getTotalWeeklySalaries();

    std::cout << "===== WEEK " << finishingWeek << " SUMMARY =====\n";

    if (!actionMadeThisWeek) {
        organization.reduceHeat(QUIET_WEEK_HEAT_REDUCTION);
        std::cout << "No actions this week. Heat reduced by "
                  << QUIET_WEEK_HEAT_REDUCTION << ".\n";
    } else {
        std::cout << "Actions were made this week. No quiet-week Heat reduction.\n";
    }

    std::cout << "Processing pending deals...\n";
    organization.processPendingDeals();
    organization.paySalaries();
    std::cout << "Pending deals processed: " << startingPendingDeals << ".\n";
    std::cout << "Salaries paid: " << totalSalaries << ".\n";
    handlePoliceEvents();

    if (checkWinLoseConditions()) {
        running = false;
        return;
    }

    organization.incrementWeek();
    actionMadeThisWeek = false;

    std::cout << "\nCurrent status:\n"
              << "Money: " << startingMoney << " -> " << organization.getMoney() << '\n'
              << "Goods: " << startingGoods << " -> " << organization.getGoods() << '\n'
              << "Heat: " << startingHeat << " -> " << organization.getHeat() << '\n'
              << "Power: " << startingPower << " -> " << organization.getPower() << '\n'
              << "Pending deals: " << startingPendingDeals << " -> "
              << organization.getPendingDeals().size() << '\n'
              << "\nWeek " << organization.getWeek() << " started.\n";
}

bool Game::checkWinLoseConditions() {
    if (organization.getMoney() < 0) {
        std::cout << "Game Over: The organization ran out of money.\n";
        return true;
    }

    return false;
}

void Game::markActionMade() {
    actionMadeThisWeek = true;
}

int Game::getPoliceEventChance() const {
    const int heat = organization.getHeat();
    if (heat < 20) {
        return 5;
    }
    if (heat < 40) {
        return 10;
    }
    if (heat < 60) {
        return 20;
    }
    if (heat < 80) {
        return 35;
    }
    if (heat < 100) {
        return 50;
    }
    return 100;
}

bool Game::rollChance(int percent) const {
    if (percent <= 0) {
        return false;
    }
    if (percent >= 100) {
        return true;
    }

    std::uniform_int_distribution<int> distribution(1, 100);
    return distribution(gameRng()) <= percent;
}

void Game::handlePoliceEvents() {
    auto applyAndPrintEvent = [this](GameEvent& event) {
        std::cout << "\nPolice event:\n"
                  << event.getName() << '\n'
                  << event.getDescription() << '\n';
        event.apply(organization);
    };

    if (organization.getHeat() >= CRACKDOWN_HEAT_THRESHOLD) {
        CrackdownEvent crackdown;
        applyAndPrintEvent(crackdown);
        return;
    }

    const int eventChance = getPoliceEventChance();
    if (!rollChance(eventChance)) {
        std::cout << "\nNo police event this week.\n";
        return;
    }

    std::unique_ptr<GameEvent> event;
    if (rollChance(50)) {
        event = std::make_unique<PolicePatrolEvent>();
    } else {
        event = std::make_unique<DealerQuestionedEvent>();
    }

    applyAndPrintEvent(*event);

    if (organization.getHeat() >= CRACKDOWN_HEAT_THRESHOLD) {
        CrackdownEvent crackdown;
        applyAndPrintEvent(crackdown);
    }
}

void Game::initializeTerritories() {
    availableTerritories.clear();
    availableTerritories.push_back(Territory("Harbor", 15, 2));
    availableTerritories.push_back(Territory("Downtown", 35, 3));
    availableTerritories.push_back(Territory("Industrial Zone", 55, 4));
}

int Game::readInt(const std::string& prompt) {
    while (true) {
        std::cout << prompt;

        int value = 0;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }

        std::cout << "Invalid input. Please enter a number.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

std::string Game::readLine(const std::string& prompt) {
    std::cout << prompt;

    std::string input;
    std::getline(std::cin, input);

    if (!std::cin) {
        std::cin.clear();
        return "";
    }

    return input;
}

void Game::waitForEnter() {
    std::cout << "\nPress Enter to continue...";
    std::string ignored;
    std::getline(std::cin, ignored);
}
