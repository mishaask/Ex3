//mishaaskarov@gmail.com
#include "Player.hpp"

void Player::gather(){
    if (!this->sanctioned)
        this->coins += 1; 
    this->endTurnhelp();
}

void Player::tax()
{
    if(this->role == Role::governor){
    this->coins += 3;
    } else {this->coins += 2;}
    this->endTurnhelp();
}

void Player::bribe()
{
    this->coins -= 4;
    this->endTurn = false;
}

void Player::arrest(Player *victim, Player*& latestVictim)
{

    if(victim->role == Role::merchant){
        victim->coins -=2;  
     } else{
    this->coins += 1;
    victim->coins -=1;}
    latestVictim = victim;
    if(victim->role == Role::general){
        victim->coins +=1;
        this->coins -=1;
        }
    this->endTurnhelp();

}

void Player::sanction(Player *victim)
{
    this->coins -= 3;
    victim->sanctioned = true;
    if(victim->role == Role::baron)
        victim->coins +=1;
    if(victim->role == Role::judge && this->coins > 0)
        this->coins -=1;
    this->endTurnhelp();
}

void Player::coup(Player *victim)
{
    //this->coins -= 7;
    victim->role = Role::spectator;
    this->endTurnhelp();
}

void Player::endTurnhelp()
{
    this->endTurn = true;
    if(this->sanctioned)
        this->sanctioned = false;
    if(this->blockedFromArrest)
    this->blockedFromArrest = false;
}

std::string Player::roleToString(Role r) const {
    switch (r) {
        case Role::user:      return "User";
        case Role::baron:     return "Baron";
        case Role::general:   return "General";
        case Role::governor:  return "Governor";
        case Role::judge:     return "Judge";
        case Role::merchant:  return "Merchant";
        case Role::spy:       return "Spy";
        case Role::spectator: return "Spectator";
    }
    return "Unknown";
}


std::string Player::stringg() const
{
    std::string prnt = "#" + std::to_string(this->userid) + ": " + this->name +" " + roleToString(this->role);
    return prnt;
}