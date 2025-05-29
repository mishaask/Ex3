//mishaaskarov@gmail.com
#include "Merchant.hpp"

Merchant::Merchant(const std::string& name, int userid)
    : Player(name, userid, Role::merchant)
{}

Merchant::Merchant(const Player& other)
    : Player(other)
{
    role = Role::merchant;
}

Merchant::Merchant(const Merchant& other)
    : Player(other)
{}

Merchant& Merchant::operator=(const Merchant& other) {
    if (this != &other) {
        Player::operator=(other);
    }
    return *this;
}

Merchant::~Merchant() = default;
