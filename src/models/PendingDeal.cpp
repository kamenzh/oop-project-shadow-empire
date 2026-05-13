#include "models/PendingDeal.h"

PendingDeal::PendingDeal(int dealerId,
                         const std::string& dealerName,
                         const std::string& territoryName,
                         int goodsAmount,
                         int paymentAmount,
                         int weeksRemaining,
                         int heatRisk)
    : dealerId(dealerId),
      dealerName(dealerName),
      territoryName(territoryName),
      goodsAmount(goodsAmount),
      paymentAmount(paymentAmount),
      weeksRemaining(weeksRemaining),
      heatRisk(heatRisk) {}

bool PendingDeal::processWeek() {
    if (weeksRemaining > 0) {
        --weeksRemaining;
    }

    return weeksRemaining == 0;
}

void PendingDeal::delay(int weeks) {
    if (weeks > 0) {
        weeksRemaining += weeks;
    }
}

int PendingDeal::getDealerId() const {
    return dealerId;
}

std::string PendingDeal::getDealerName() const {
    return dealerName;
}

std::string PendingDeal::getTerritoryName() const {
    return territoryName;
}

int PendingDeal::getGoodsAmount() const {
    return goodsAmount;
}

int PendingDeal::getPayment() const {
    return paymentAmount;
}

int PendingDeal::getWeeksRemaining() const {
    return weeksRemaining;
}

int PendingDeal::getHeatRisk() const {
    return heatRisk;
}
