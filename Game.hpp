#pragma once

#include <random>
#include "Player.hpp"
/// @brief This class will handle all the GameLogic
class Game{

    private:
    int turnCounter;
    Player* currentTurn;
    std::vector<Player*> players;


    // helper from HomeScreen to center text in a rectangle:
    static void centerText(sf::Text& txt, const sf::RectangleShape& btn);

    public:

    Game():turnCounter(0), currentTurn(nullptr){}

    // Game(int playerCount):turnCounter(0), currentTurn(nullptr){

    // }
    

    ~Game(){
        for (Player* p : players)
        delete p;
    }

    /// Pop up a small window to name a single player.
    /// Returns true if user pressed “Accept”, false if “Cancel” (or closed).
    bool getsetName(Player* p, std::size_t index);

    /// Randomly pick & assign a Role, return it
    Role assignRole(Player* p);

    /// Build players, name them, assign roles, then launch the SFML game window
    void launchGame(const sf::VideoMode& videoMode, int playerCount);
};