#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>


#include <string>

const int Start_Coin_value = 0;

enum class Role
{user=0, baron=1, general=2, governor=3,
             judge=4, merchant=5, spy=6, spectator =-1};//spectator - dead, user - main user

class Player {

    public:
        std::string name;
        int userid = -1;//will indicate turns, -1 indicates not playing yet
        Role role;
        int coins;    
    
        //default constructor
        Player()
            :name("null"),role(Role::user),coins(Start_Coin_value){}

        //constructor with parameters: instead of passing name by value(copying the original inputed string into std::string name) we will pass it by refrence (const T&) 
        //to assure we dont make unnecesary copies wasting memory and we assure no changes happen to the original by using const
        Player(const std::string& namee,int useridd, Role rolee)
            :name(namee),userid(useridd),role(rolee),coins(Start_Coin_value){}

        //copy constructor
        Player( const Player& other)
            :name(other.name),userid(other.userid),role(other.role),coins(other.coins){}

        // //userid constructor
        // Player(int userid)
        //     :name("null"),role(Role::user),coins(Start_Coin_value){}
        
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

        void setId(int id){
            this->userid = id;
        }

        void gather(){}//recieve 1 coin, no cost, can be stopped using sanction

        virtual void tax(){}//recieve 2 coins, no cost, can be blocked
        //tax's implementation is also role dependent so its better that its inheritors get advantage in the inheritance
    
        void bribe(){}//pay 4 coins, make another action.
        //number of actions per turn can change

        void arrest(){}//choose a player, steal 1 coin, cant be used on same target twice in a row

        void sanction(){}//cost 3 coins, the player chooses another player and blocks him from using gather or tax untill his next turn.
        
        void coup(){}//costs 7 coins, can be blocked if certain condition are met, the player chooses another player and removes him from the game
        



}; 