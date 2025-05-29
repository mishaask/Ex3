//mishaaskarov@gmail.com
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
    Player* Winner;
    Player* latestVictim;
    std::vector<Player*> players;
    bool showAllCoins;
    bool coins10Check = false;


    /**
     * @brief Center text within a button rectangle (UI helper).
     * @param txt Text object to center.
     * @param btn Rectangle shape to center within.
     */
    static void centerText(sf::Text& txt, const sf::RectangleShape& btn);

    /**
     * @brief Popup dialog to choose a victim player.
     * @param title Window title for the dialog.
     * @return Pointer to selected victim, or nullptr if cancelled.
     */
    Player* chooseVictim(const std::string& title);

    /**
     * @brief Prompt governor to block another player's tax.
     * @param governorName Name of the blocking governor.
     * @return True if action is blocked, false otherwise.
     */
    bool governorBlockPrompt(const std::string& governorName);

    /**
     * @brief Prompt general to block a coup action.
     * @param generalName Name of the blocking general.
     * @return True if coup is blocked, false otherwise.
     */
    bool blockCoupPrompt(const std::string &generalName);

    /**
     * @brief Prompt judge to block a bribe action.
     * @param judgeName Name of the blocking judge.
     * @return True if bribe is blocked, false otherwise.
     */
    bool blockBribePrompt(const std::string &judgeName);

    /**
     * @brief Display the winner in a separate window.
     */
    void showWinnerWindow();

    public:

    /**
     * @brief Construct a new Game object.
     */
    Game():turnCounter(0), Winner(nullptr){}

    /**
     * @brief Destroy the Game and free all player objects.
     */
    ~Game(){
        for (Player* p : players)
        delete p;
    }

    /**
     * @brief Prompt for and set a player's name in a dialog.
     * @param p Player pointer to set name for.
     * @param index Player index for display.
     * @return True if accepted, false if cancelled.
     */
    bool getsetName(Player* p, std::size_t index);

    /**
     * @brief Main loop driving the SFML game window.
     * @param videoMode The video mode (resolution) for the window.
     */
    void runningGame(const sf::VideoMode &videoMode);

    /**
     * @brief Randomly pick and assign a role to a player.
     * @param players Vector of player pointers.
     * @param idx Index of player in vector to assign.
     * @return The picked Role enum.
     */
    Role assignRole(std::vector<Player *> &players, std::size_t idx);

    /**
     * @brief Initialize players, assign roles, and launch game UI.
     * @param videoMode The resolution for the game window.
     * @param playerCount Number of players to include.
     */
    void launchGame(const sf::VideoMode& videoMode, int playerCount);

    /**
     * @brief Get the winner's name if the game is over.
     * @return Name of the winning player.
     * @throws std::runtime_error if more than one player remains.
     */
    std::string winnerIs() const;

    /**
     * @brief List names of all active (non-spectator) players.
     * @return Vector of active player names.
     */
    std::vector<std::string> alivePlayers() const;


    
};