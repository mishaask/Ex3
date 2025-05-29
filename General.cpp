//mishaaskarov@gmail.com
#include "General.hpp"

General::General(const std::string& name, int userid)
    : Player(name, userid, Role::general)
{}

General::General(const Player& other)
    : Player(other)
{
    role = Role::general;
}

General::General(const General& other)
    : Player(other)
{}

General& General::operator=(const General& other) {
    if (this != &other) {
        Player::operator=(other);
    }
    return *this;
}

General::~General() = default;



void General::onArrest() {
    coins += 1;
}