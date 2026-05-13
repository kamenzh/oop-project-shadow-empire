#include "systems/SaveManager.h"

#include "core/Organization.h"
#include "models/CrewMember.h"
#include "models/Dealer.h"
#include "models/Enforcer.h"
#include "models/PendingDeal.h"
#include "models/Territory.h"

#if defined(__has_include)
#if __has_include(<filesystem>)
#define SHADOW_EMPIRE_HAS_FILESYSTEM 1
#include <filesystem>
#endif
#endif

#ifndef SHADOW_EMPIRE_HAS_FILESYSTEM
#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif
#endif

#include <cerrno>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <vector>

namespace {
const std::string NONE_VALUE = "<NONE>";

std::string encodeOptionalString(const std::string& value) {
    return value.empty() ? NONE_VALUE : value;
}

std::string decodeOptionalString(const std::string& value) {
    return value == NONE_VALUE ? "" : value;
}

bool readLineStrict(std::istream& input, std::string& value) {
    return static_cast<bool>(std::getline(input, value));
}

bool createSaveDirectory(const std::string& filePath) {
#ifdef SHADOW_EMPIRE_HAS_FILESYSTEM
    const std::filesystem::path path(filePath);
    if (path.has_parent_path()) {
        std::error_code error;
        std::filesystem::create_directories(path.parent_path(), error);
        if (error) {
            std::cerr << "Failed to create save directory: "
                      << path.parent_path().string() << '\n';
            return false;
        }
    }
    return true;
#else
    const std::size_t separator = filePath.find_last_of("/\\");
    if (separator == std::string::npos) {
        return true;
    }

    const std::string directory = filePath.substr(0, separator);
    if (directory.empty()) {
        return true;
    }

#ifdef _WIN32
    _mkdir(directory.c_str());
#else
    mkdir(directory.c_str(), 0755);
#endif
    return true;
#endif
}
}

SaveManager::SaveManager()
    : filePath("data/save.txt") {}

SaveManager::SaveManager(const std::string& filePath)
    : filePath(filePath) {}

bool SaveManager::save(const Organization& org) const {
    if (!createSaveDirectory(filePath)) {
        return false;
    }

    errno = 0;
    std::ofstream output(filePath, std::ios::out | std::ios::trunc);
    if (!output) {
        std::cerr << "Failed to open save file: " << filePath;
        if (errno != 0) {
            std::cerr << " (" << std::strerror(errno) << ")";
        }
        std::cerr << '\n';
        return false;
    }

    output << "ORG\n";
    output << org.getName() << '\n';
    output << org.getWeek() << ' '
           << org.getMoney() << ' '
           << org.getGoods() << ' '
           << org.getHeat() << ' '
           << org.getPower() << ' '
           << org.getNextCrewId() << '\n';

    const auto& territories = org.getTerritories();
    output << "TERRITORIES " << territories.size() << '\n';
    for (const Territory& territory : territories) {
        output << territory.getName() << '\n';
        output << territory.getRequiredPower() << ' '
               << territory.getDealerCapacity() << '\n';
    }

    const auto& crew = org.getCrew();
    output << "CREW " << crew.size() << '\n';
    for (const auto& member : crew) {
        if (member->getRole() == "Dealer") {
            const Dealer* dealer = dynamic_cast<const Dealer*>(member.get());
            if (!dealer) {
                std::cerr << "Failed to save crew member: Dealer cast failed.\n";
                return false;
            }

            output << "Dealer\n";
            output << dealer->getId() << '\n';
            output << dealer->getName() << '\n';
            output << dealer->getHirePrice() << ' '
                   << dealer->getWeeklySalary() << ' '
                   << dealer->getCapacity() << ' '
                   << dealer->getRisk() << '\n';
            output << encodeOptionalString(dealer->getAssignedTerritoryName()) << '\n';
            output << (dealer->isBusy() ? 1 : 0) << '\n';
        } else if (member->getRole() == "Enforcer") {
            const Enforcer* enforcer = dynamic_cast<const Enforcer*>(member.get());
            if (!enforcer) {
                std::cerr << "Failed to save crew member: Enforcer cast failed.\n";
                return false;
            }

            output << "Enforcer\n";
            output << enforcer->getId() << '\n';
            output << enforcer->getName() << '\n';
            output << enforcer->getHirePrice() << ' '
                   << enforcer->getWeeklySalary() << ' '
                   << enforcer->getPowerBonus() << ' '
                   << enforcer->getHireHeat() << '\n';
        } else {
            std::cerr << "Failed to save crew member: unknown role " << member->getRole() << '\n';
            return false;
        }
    }

    const auto& pendingDeals = org.getPendingDeals();
    output << "DEALS " << pendingDeals.size() << '\n';
    for (const PendingDeal& deal : pendingDeals) {
        output << deal.getDealerId() << '\n';
        output << deal.getDealerName() << '\n';
        output << deal.getTerritoryName() << '\n';
        output << deal.getGoodsAmount() << ' '
               << deal.getPayment() << ' '
               << deal.getWeeksRemaining() << ' '
               << deal.getHeatRisk() << '\n';
    }

    if (!output) {
        std::cerr << "Failed while writing save file: " << filePath << '\n';
        return false;
    }

    return true;
}

bool SaveManager::load(Organization& org) const {
    std::ifstream input(filePath);
    if (!input) {
        return false;
    }

    std::string section;
    std::string name;
    int week = 0;
    int money = 0;
    int goods = 0;
    int heat = 0;
    int power = 0;
    int nextCrewId = 0;

    if (!readLineStrict(input, section) || section != "ORG") {
        return false;
    }

    if (!readLineStrict(input, name)) {
        return false;
    }

    if (!(input >> week >> money >> goods >> heat >> power >> nextCrewId)) {
        return false;
    }
    input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::size_t territoryCount = 0;
    if (!(input >> section >> territoryCount) || section != "TERRITORIES") {
        return false;
    }
    input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::vector<Territory> territories;
    territories.reserve(territoryCount);
    for (std::size_t i = 0; i < territoryCount; ++i) {
        std::string territoryName;
        int requiredPower = 0;
        int dealerCapacity = 0;

        if (!readLineStrict(input, territoryName)) {
            return false;
        }
        if (!(input >> requiredPower >> dealerCapacity)) {
            return false;
        }
        input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        territories.push_back(Territory(territoryName, requiredPower, dealerCapacity));
    }

    std::size_t crewCount = 0;
    if (!(input >> section >> crewCount) || section != "CREW") {
        return false;
    }
    input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::vector<std::unique_ptr<CrewMember>> crew;
    crew.reserve(crewCount);
    for (std::size_t i = 0; i < crewCount; ++i) {
        std::string role;
        int id = 0;
        std::string memberName;
        int hirePrice = 0;
        int weeklySalary = 0;

        if (!readLineStrict(input, role)) {
            return false;
        }
        if (!(input >> id)) {
            return false;
        }
        input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (!readLineStrict(input, memberName)) {
            return false;
        }

        if (role == "Dealer") {
            int capacity = 0;
            int risk = 0;
            std::string assignedTerritoryName;
            int busy = 0;

            if (!(input >> hirePrice >> weeklySalary >> capacity >> risk)) {
                return false;
            }
            input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (!readLineStrict(input, assignedTerritoryName)) {
                return false;
            }
            if (!(input >> busy)) {
                return false;
            }
            input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            auto dealer = std::make_unique<Dealer>(id, memberName, hirePrice, weeklySalary, capacity, risk);
            const std::string decodedTerritory = decodeOptionalString(assignedTerritoryName);
            if (!decodedTerritory.empty()) {
                dealer->assign(decodedTerritory);
            }
            dealer->setBusy(busy != 0);
            crew.push_back(std::move(dealer));
        } else if (role == "Enforcer") {
            int powerBonus = 0;
            int hireHeat = 0;

            if (!(input >> hirePrice >> weeklySalary >> powerBonus >> hireHeat)) {
                return false;
            }
            input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            crew.push_back(std::make_unique<Enforcer>(id, memberName, hirePrice, weeklySalary, powerBonus, hireHeat));
        } else {
            return false;
        }
    }

    std::size_t dealCount = 0;
    if (!(input >> section >> dealCount) || section != "DEALS") {
        return false;
    }
    input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::vector<PendingDeal> pendingDeals;
    pendingDeals.reserve(dealCount);
    for (std::size_t i = 0; i < dealCount; ++i) {
        int dealerId = 0;
        std::string dealerName;
        std::string territoryName;
        int goodsAmount = 0;
        int paymentAmount = 0;
        int weeksRemaining = 0;
        int heatRisk = 0;

        if (!(input >> dealerId)) {
            return false;
        }
        input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (!readLineStrict(input, dealerName) || !readLineStrict(input, territoryName)) {
            return false;
        }
        if (!(input >> goodsAmount >> paymentAmount >> weeksRemaining >> heatRisk)) {
            return false;
        }
        input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        pendingDeals.push_back(PendingDeal(dealerId,
                                           dealerName,
                                           territoryName,
                                           goodsAmount,
                                           paymentAmount,
                                           weeksRemaining,
                                           heatRisk));
    }

    org.loadState(name,
                  week,
                  money,
                  goods,
                  heat,
                  power,
                  nextCrewId,
                  std::move(crew),
                  std::move(territories),
                  std::move(pendingDeals));
    return true;
}
