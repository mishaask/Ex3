//mishaaskarov@gmail.com
#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>


#include <string>

const int Start_Coin_value = 0;

/**
 * @enum Role
 * @brief Enumerates all possible roles in Coup.
 */
enum class Role
{user=0, baron=1, general=2, governor=3,
             judge=4, merchant=5, spy=6, spectator =-1};//spectator - dead, user - main user

class Player {

    public:
        std::string name;
        int userid = -1; //will indicate turns, -1 indicates not playing yet
        Role role;
        int coins;
        bool sanctioned = false;    
        bool endTurn = false;
        bool blockedFromArrest = false;
        bool merchantPassiveTriggered = false;
    
        /**
         * @brief Default constructor initializes to null state.
         */
        Player()
            :name("null"),role(Role::user),coins(Start_Coin_value){}

        /**
         * @brief Construct a Player with name, ID, and role.
         * @param namee Name to assign.
         * @param useridd ID to assign.
         * @param rolee Role to assign.
         */
        Player(const std::string& namee,int useridd, Role rolee)
            :name(namee),userid(useridd),role(rolee),coins(Start_Coin_value){}

        /**
         * @brief Copy constructor.
         * @param other Player to copy.
         */
        Player( const Player& other)
            :name(other.name),userid(other.userid),role(other.role),coins(other.coins){}

        /**
         * @brief Copy assignment operator.
         * @param other Player to assign from.
         * @return Reference to this Player.
         */
        Player& operator=(const Player& other){
            if (this != &other){//check if were comparing the same person
                name = other.name;
                role = other.role;
                coins = other.coins;
            }
            return *this;
        }

        /** 
         * @brief Destroy the Player.
         */
        virtual ~Player() {};

        /**
         * @brief Set the player ID.
         * @param id New user ID.
         */
        void setId(int id){
            this->userid = id;
        }

        /**
         * @brief Gain one coin unless sanctioned.
         */
        void gather();//recieve 1 coin, no cost, can be stopped using sanction

        /**
         * @brief Gain coins based on role (2 or 3).
         */
        virtual void tax();
    
        /**
         * @brief Pay 4 coins to draw again.
         */
        void bribe();
    
        /**
         * @brief Arrest another player to steal a coin.
         * @param victim Player to arrest.
         * @param latestVictim Output parameter for last arrested.
         */
        void arrest(Player* victim, Player*& latestVictim);//choose a player, steal 1 coin, cant be used on same target twice in a row

        /**
         * @brief Sanction another player to block their gather/tax.
         * @param victim Player to sanction.
         */
        void sanction(Player* victim);//cost 3 coins, the player chooses another player and blocks him from using gather or tax untill his next turn.
        
        /**
         * @brief Coup another player out of the game.
         * @param victim Player to remove.
         */
        void coup(Player* victim);//costs 7 coins, can be blocked if certain condition are met, the player chooses another player and removes him from the game

        /**
         * @brief Helper to mark end-of-turn state and reset blocks.
         */
        void endTurnhelp();

        /**
         * @brief Convert a Role enum to a human-readable string.
         * @param r Role value to convert.
         * @return Corresponding role name.
         */
        std::string roleToString(Role r) const ;
     
        /**
         * @brief Format player info as string.
         * @return "#ID: Name Role" string.
         */
        std::string stringg() const;   



}; 