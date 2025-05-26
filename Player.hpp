#pragma once

#include <string>

const int Start_Coin_value = 0;

enum class Role
{user=0, baron=1, general=2, governor=3,
             judge=4, merchant=5, spy=6, spectator =-1};

class Player {

    private:
        int userid;
    public:
        std::string name;
        Role role;
        int coins;
        //default constructor
        Player()
            :name("null"),role(Role::user),coins(Start_Coin_value){}

        //constructor with parameters: instead of passing name by value(copying the original inputed string into std::string name) we will pass it by refrence (const T&) 
        //to assure we dont make unnecesary copies wasting memory and we assure no changes happen to the original by using const
        Player(const std::string& namee, Role rolee)
            :name(namee),role(rolee),coins(Start_Coin_value){}

        //copy constructor
        Player( const Player& other)
            :name(other.name),role(other.role),coins(other.coins){}
        
        //copy assignment operator
        Player& operator=(const Player& other){
            if (this != &other){//check if were comparing the same person
                name = other.name;
                role = other.role;
                coins = other.coins;
            }
            return *this;
        }

        virtual ~Player(){}

        void gather(){}//recieve 1 coin, no cost, can be stopped using sanction

        virtual void tax(){}//recieve 2 coins, no cost, can be blocked
        //tax's implementation is also role dependent so its better that its inheritors get advantage in the inheritance
    
        void bribe(){}//pay 4 coins, make another action.
        //number of actions per turn can change

        void arrest(){}//choose a player, steal 1 coin, cant be used on same target twice in a row

        void sanction(){}//cost 3 coins, the player chooses another player and blocks him from using gather or tax untill his next turn.
        
        void coup(){}//costs 7 coins, can be blocked if certain condition are met, the player chooses another player and removes him from the game
        



}; 