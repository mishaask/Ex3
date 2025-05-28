#pragma once

#include <iostream>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include "Game.hpp"

class HomeScreen{
    private:
        //Variables
        //Window
        sf::RenderWindow* window;
        sf::Event ev;
        sf::VideoMode videoMode;

        //Mouse positions
        sf::Vector2i mousePosWindow;

        //Game objects
        std::vector<sf::RectangleShape> enemies;
        sf::RectangleShape enemy;

        // Buttons
        std::vector<sf::RectangleShape> buttons;
        sf::RectangleShape button;
        
        //Text for buttons
        sf::Font font;
        std::vector<sf::Text> buttonTexts;


        Game game;

        void initVariables();
        void initWindow();
        void initButtons();
        void initButtonLabels();
        void initEnemies();

    public:


        //Constructor
        HomeScreen();
        
        //Destructor
        virtual ~HomeScreen();

        //Accesors
        bool running() const;
        //Functions
        void pollEvents();
        void updateMousePositions();

        //Open player counter
        int openPlayerCountWindow();

        // //launch game window
        // void launchGame(sf::VideoMode videMode, int playercount);

        void update();
        void render();

        

};