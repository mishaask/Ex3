//mishaaskarov@gmail.com
#include "Spy.hpp"

Spy::Spy(const std::string& name, int userid)
    : Player(name, userid, Role::spy)
{}

Spy::Spy(const Player& other)
    : Player(other)
{
    role = Role::spy;
}

Spy::Spy(const Spy& other)
    : Player(other)
{}

Spy& Spy::operator=(const Spy& other) {
    if (this != &other) {
        Player::operator=(other);
        // role copied via base, coins etc.
    }
    return *this;
}

Spy::~Spy() = default;