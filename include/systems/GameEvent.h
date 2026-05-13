#ifndef SHADOW_EMPIRE_GAME_EVENT_H
#define SHADOW_EMPIRE_GAME_EVENT_H

#include <string>

class Organization;

class GameEvent {
protected:
    std::string name;
    std::string description;

public:
    GameEvent(const std::string& name, const std::string& description);
    virtual ~GameEvent() = default;

    virtual void apply(Organization& org) = 0;

    std::string getName() const;
    std::string getDescription() const;
};

#endif
