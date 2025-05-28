#pragma once

#include "Player.hpp"

/// @brief Baron can invest coins for profit and gains compensation when sanctioned.
class Baron : public Player {
public:
    Baron(const std::string& name, int userid);
    Baron(const Player& other);
    Baron(const Baron& other);
    Baron& operator=(const Baron& other);
    virtual ~Baron();

    void invest();
};