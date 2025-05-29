//mishaaskarov@gmail.com
#pragma once

#include "Player.hpp"

/// @brief General can block coups and recovers coin if arrested.
class General : public Player {
public:
    General(const std::string& name, int userid);
    General(const Player& other);
    General(const General& other);
    General& operator=(const General& other);
    virtual ~General();

    //void blockCoup(Player* target);
    void onArrest();
};
