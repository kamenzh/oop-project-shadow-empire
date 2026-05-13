#include "systems/DealerQuestionedEvent.h"

#include "core/Organization.h"

DealerQuestionedEvent::DealerQuestionedEvent()
    : GameEvent("Dealer Questioned",
                "Police questioned one of the active dealers. A pending deal may be delayed.") {}

void DealerQuestionedEvent::apply(Organization& org) {
    org.delayRandomPendingDeal(1);
    org.addHeat(8);
}
