#pragma once

#include "Player.hpp"

/// @brief Judge can block bribes and penalize sanctioners.
class Judge : public Player {
public:
    Judge(const std::string& name, int userid);
    Judge(const Player& other);
    Judge(const Judge& other);
    Judge& operator=(const Judge& other);
    virtual ~Judge();


};
