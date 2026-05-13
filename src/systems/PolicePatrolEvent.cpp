#include "systems/PolicePatrolEvent.h"

#include "core/Organization.h"

GameEvent::GameEvent(const std::string& name, const std::string& description)
    : name(name),
      description(description) {}

std::string GameEvent::getName() const {
    return name;
}

std::string GameEvent::getDescription() const {
    return description;
}

PolicePatrolEvent::PolicePatrolEvent()
    : GameEvent("Police Patrol", "Police patrols increased pressure on the organization.") {}

void PolicePatrolEvent::apply(Organization& org) {
    org.addHeat(5);
}
