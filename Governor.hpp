//mishaaskarov@gmail.com
#pragma once

#include "Player.hpp"

/// @brief Governor collects 3 coins on tax and can block other tax actions.
class Governor : public Player {
public:
    Governor(const std::string& name, int userid);
    Governor(const Player& other);
    Governor(const Governor& other);
    Governor& operator=(const Governor& other);
    virtual ~Governor();

    void tax() override;
};