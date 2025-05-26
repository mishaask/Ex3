#pragma once
#include "Player.hpp"
/// @brief This class will handle all the GameLogic
class Game{

    private:
    int turnCounter;
    Player* currentTurn;
    std::vector<Player*> players;


    public:

    Game(){}

    Game(int playerCount){}

    void assignRole(){}//בתחילת המשחק כל שחקן שולף קלף מהערימה. כל קלף מתאר תפקיד אותו יכול לממש השחקן. במרכז השולחן ישנה קופה של מטבעות.
    //random number generator from 1-6/7 and assign accordingly. no need to remove picked option as there is no cap on number of roles


};