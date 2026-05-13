#ifndef SHADOW_EMPIRE_PENDING_DEAL_H
#define SHADOW_EMPIRE_PENDING_DEAL_H

#include <string>

class PendingDeal {
private:
    int dealerId;
    std::string dealerName;
    std::string territoryName;
    int goodsAmount;
    int paymentAmount;
    int weeksRemaining;
    int heatRisk;

public:
    PendingDeal(int dealerId,
                const std::string& dealerName,
                const std::string& territoryName,
                int goodsAmount,
                int paymentAmount,
                int weeksRemaining,
                int heatRisk);

    bool processWeek();
    void delay(int weeks);

    int getDealerId() const;
    std::string getDealerName() const;
    std::string getTerritoryName() const;
    int getGoodsAmount() const;
    int getPayment() const;
    int getWeeksRemaining() const;
    int getHeatRisk() const;
};

#endif
