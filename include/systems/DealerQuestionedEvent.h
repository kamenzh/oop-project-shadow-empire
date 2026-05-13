#ifndef SHADOW_EMPIRE_DEALER_QUESTIONED_EVENT_H
#define SHADOW_EMPIRE_DEALER_QUESTIONED_EVENT_H

#include "systems/GameEvent.h"

class DealerQuestionedEvent : public GameEvent {
public:
    DealerQuestionedEvent();

    void apply(Organization& org) override;
};

#endif
