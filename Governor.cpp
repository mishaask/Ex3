#include "Governor.hpp"

Governor::Governor(const std::string& name, int userid)
    : Player(name, userid, Role::governor)
{}

Governor::Governor(const Player& other)
    : Player(other)
{
    role = Role::governor;
}

Governor::Governor(const Governor& other)
    : Player(other)
{}

Governor& Governor::operator=(const Governor& other) {
    if (this != &other) {
        Player::operator=(other);
    }
    return *this;
}

Governor::~Governor() = default;

void Governor::tax() {
    coins += 3;
    endTurn = true;
}

