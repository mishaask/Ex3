//mishaaskarov@gmail.com
#pragma once

#include "Player.hpp"

/// @brief A Spy can steal 1 coin from another player when using tax.
class Spy : public Player {
public:
    // Default, copy, move
    Spy(const std::string& name, int userid);
    Spy(const Player& other);
    Spy(const Spy& other);
    Spy& operator=(const Spy& other);
    virtual ~Spy();

};
