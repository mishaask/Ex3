#pragma once

#include "Player.hpp"

/// @brief Merchant gets a bonus if rich and pays higher penalty when arrested.
class Merchant : public Player {
public:
    Merchant(const std::string& name, int userid);
    Merchant(const Player& other);
    Merchant(const Merchant& other);
    Merchant& operator=(const Merchant& other);
    virtual ~Merchant();

    // void startTurnBonus();
    // void onArrestPenalty(Player* origin);
};
