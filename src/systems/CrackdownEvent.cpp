#include "systems/CrackdownEvent.h"

#include "core/Organization.h"

CrackdownEvent::CrackdownEvent()
    : GameEvent("Crackdown", "A major police crackdown hit the organization.") {}

void CrackdownEvent::apply(Organization& org) {
    org.reduceMoneyPercent(25);
    org.reduceGoodsPercent(30);
    org.delayAllPendingDeals(1);
    org.setHeat(50);
}
