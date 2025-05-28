#include "Baron.hpp"

Baron::Baron(const std::string& name, int userid)
    : Player(name, userid, Role::baron)
{}

Baron::Baron(const Player& other)
    : Player(other)
{
    role = Role::baron;
}

Baron::Baron(const Baron& other)
    : Player(other)
{}

Baron& Baron::operator=(const Baron& other) {
    if (this != &other) {
        Player::operator=(other);
    }
    return *this;
}

Baron::~Baron() = default;

void Baron::invest() {
    if (coins >= 3) {
        coins -= 3;
        coins += 6;
    }
    endTurn = true;
}


