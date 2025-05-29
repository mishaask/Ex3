//mishaaskarov@gmail.com
#include "Judge.hpp"

Judge::Judge(const std::string& name, int userid)
    : Player(name, userid, Role::judge)
{}

Judge::Judge(const Player& other)
    : Player(other)
{
    role = Role::judge;
}

Judge::Judge(const Judge& other)
    : Player(other)
{}

Judge& Judge::operator=(const Judge& other) {
    if (this != &other) {
        Player::operator=(other);
    }
    return *this;
}

Judge::~Judge() = default;

