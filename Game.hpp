#pragma once

#include <random>
#include "Player.hpp"
#include "Baron.hpp"
#include "General.hpp"
#include "Governor.hpp"
#include "Judge.hpp"
#include "Spy.hpp"
#include "Merchant.hpp"

/// @brief This class will handle all the GameLogic
class Game{

    int turnCounter;
    int currentPlayerIndex;
    Player* currentTurn;
    Player* Winner;
    Player* latestVictim;
    std::vector<Player*> players;

    //sf::RenderWindow* gameWindow;
    //std::vector<sf::RectangleShape> playerBoxes;
    //std::vector<sf::Text> playerInfo;
    //sf::RectangleShape viewCoinsBtn;
    //sf::Text viewCoinsText;
    bool showAllCoins;
    //std::vector<sf::RectangleShape> actionBtns;
    //std::vector<sf::Text> actionBtnText;
    //sf::RectangleShape endTurnBtn;
    //sf::Text endTurnText;
    //sf::RectangleShape spyBlockBtn;
    //sf::Text spyBlockText;


    // helper from HomeScreen to center text in a rectangle:
    static void centerText(sf::Text& txt, const sf::RectangleShape& btn);

    Player* chooseVictim(const std::string& title);

    bool governorBlockPrompt(const std::string& governorName);

    void showWinnerWindow();

    public:

    Game():turnCounter(0), currentTurn(nullptr), Winner(nullptr){}

    // Game(int playerCount):turnCounter(0), currentTurn(nullptr){

    // }
    ~Game(){
        for (Player* p : players)
        delete p;
    }

    /// Pop up a small window to name a single player.
    /// Returns true if user pressed “Accept”, false if “Cancel” (or closed).
    bool getsetName(Player* p, std::size_t index);

    void runningGame(const sf::VideoMode &videoMode);

    /// Randomly pick & assign a Role, return it
    Role assignRole(std::vector<Player *> &players, std::size_t idx);

    /// Build players, name them, assign roles, then launch the SFML game window
    void launchGame(const sf::VideoMode& videoMode, int playerCount);

    std::string winnerIs() const;

    std::vector<std::string> alivePlayers() const;


    
};