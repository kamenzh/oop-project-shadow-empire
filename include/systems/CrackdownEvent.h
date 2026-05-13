#ifndef SHADOW_EMPIRE_CRACKDOWN_EVENT_H
#define SHADOW_EMPIRE_CRACKDOWN_EVENT_H

#include "systems/GameEvent.h"

class CrackdownEvent : public GameEvent {
public:
    CrackdownEvent();

    void apply(Organization& org) override;
};

#endif
