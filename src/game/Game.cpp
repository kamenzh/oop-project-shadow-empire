#include "game/Game.h"

#include "utils/Constants.h"

#include <iostream>
#include <limits>
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
              << "4. Next Week\n"
              << "5. Exit\n";

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
        processNextWeek();
        break;
    case 5:
        std::cout << "Exiting Shadow Empire.\n";
        running = false;
        return;
    default:
        std::cout << "Invalid menu option.\n";
        break;
    }

    if (choice == 5) {
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

void Game::processNextWeek() {
    const int startingMoney = organization.getMoney();
    const int startingGoods = organization.getGoods();
    const int startingHeat = organization.getHeat();
    const int previousWeek = organization.getWeek();

    if (!actionMadeThisWeek) {
        organization.reduceHeat(QUIET_WEEK_HEAT_REDUCTION);
        std::cout << "No actions this week. Heat reduced by "
                  << QUIET_WEEK_HEAT_REDUCTION << ".\n";
    }

    organization.paySalaries();
    organization.incrementWeek();
    actionMadeThisWeek = false;

    std::cout << "\nWeek " << previousWeek << " summary:\n"
              << "Money: " << startingMoney << " -> " << organization.getMoney() << '\n'
              << "Goods: " << startingGoods << " -> " << organization.getGoods() << '\n'
              << "Heat: " << startingHeat << " -> " << organization.getHeat() << '\n'
              << "Now entering week " << organization.getWeek() << ".\n";
}

bool Game::checkWinLoseConditions() {
    return false;
}

void Game::markActionMade() {
    actionMadeThisWeek = true;
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
