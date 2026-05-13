#ifndef SHADOW_EMPIRE_POLICE_PATROL_EVENT_H
#define SHADOW_EMPIRE_POLICE_PATROL_EVENT_H

#include "systems/GameEvent.h"

class PolicePatrolEvent : public GameEvent {
public:
    PolicePatrolEvent();

    void apply(Organization& org) override;
};

#endif
