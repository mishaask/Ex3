#include "Game.hpp"



void Game::centerText(sf::Text& txt, const sf::RectangleShape& btn) {
    sf::FloatRect tb = txt.getLocalBounds();
    txt.setOrigin(tb.left + tb.width/2.f, tb.top + tb.height/2.f);
    sf::FloatRect bb = btn.getGlobalBounds();
    txt.setPosition(bb.left + bb.width/2.f, bb.top + bb.height/2.f);
}

/// Pick & assign a new Role object, replacing *p with a new derived instance.
/// Returns the Role that was assigned.
Role Game::assignRole(std::vector<Player*>& players, std::size_t idx) {
    static std::random_device rd;
    static std::mt19937       gen(rd());
    static const Role options[] = {
        Role::baron, Role::general, Role::governor,
        Role::judge, Role::merchant, Role::spy
    };
    std::uniform_int_distribution<int> dist(
        0, sizeof(options)/sizeof(options[0]) - 1
    );
    Role picked = options[dist(gen)];

    Player* oldP = players[idx];
    Player* newP = nullptr;

    // build the correct subclass
    switch (picked) {
      case Role::baron:
        newP = new Baron(*oldP);
        break;
      case Role::governor:
        newP = new Governor(*oldP);
        break;
      case Role::spy:
        newP = new Spy(*oldP);
        break;
      case Role::general:
        newP = new General(*oldP);
        break;
      case Role::judge:
        newP = new Judge(*oldP);
        break;
      case Role::merchant:
        newP = new Merchant(*oldP);
        break;
      default:
        // fall back to a plain Player
        newP = new Player(*oldP);
        break;
    }

    delete oldP;                    // free the old base object
    players[idx] = newP;            // store the new polymorphic object
    return picked;
}

void Game::launchGame(const sf::VideoMode& videoMode,int playerCount) {
    // delete old, rebuild players list
    for (std::vector<Player*>::iterator it = players.begin(); it != players.end(); ++it) {
        delete *it;
    }
    players.clear();
    players.reserve(playerCount);
    for (std::size_t i = 0; i < playerCount; ++i) {
        players.push_back(new Player());
    }

    // name & assign
    for (std::size_t i = 0; i < players.size(); ++i) {
        players[i]->setId(int(i));
        if (!getsetName(players[i], i))
            return;   // cancelled
        assignRole(players, i);
        
    }

    for (Player* p: players)
        std::cout<<p->stringg()<<"\n";

    //Launch main SFML game window 
    runningGame(videoMode);
}

std::string Game::winnerIs() const
{
    return this->Winner->name;
}

std::vector<std::string> Game::alivePlayers() const
{
    std::vector<std::string> aliveplayers;
    for(Player* p: this->players){
        if (p->role != Role::spectator)
            aliveplayers.push_back(p->name);
    }

    return aliveplayers;
}


bool Game::getsetName(Player* p, std::size_t index) {
    sf::RenderWindow window(
        sf::VideoMode(400,200),
        "Enter Player Name",
        sf::Style::Titlebar | sf::Style::Close
    );
    window.setFramerateLimit(60);

    sf::Font font;
    font.loadFromFile("Font/PIXEARG_.ttf");  // assume success

    // prompt text
    sf::Text prompt(
        "Name for Player " + std::to_string(index + 1) + ":",
        font,
        20
    );
    prompt.setFillColor(sf::Color::White);
    prompt.setPosition(20.f, 20.f);

    // input display
    std::string input;
    sf::Text textInput("", font, 24);
    textInput.setFillColor(sf::Color::White);
    textInput.setPosition(20.f, 80.f);

    // Accept button
    sf::RectangleShape acceptBtn(sf::Vector2f(100.f, 40.f));
    acceptBtn.setPosition(220.f, 150.f);
    acceptBtn.setFillColor(sf::Color(100, 100, 250));
    sf::Text acceptText("Accept", font, 18);
    acceptText.setFillColor(sf::Color::White);
    centerText(acceptText, acceptBtn);

    // Cancel button
    sf::RectangleShape cancelBtn(sf::Vector2f(100.f, 40.f));
    cancelBtn.setPosition(50.f, 150.f);
    cancelBtn.setFillColor(sf::Color(100, 100, 250));
    sf::Text cancelText("Cancel", font, 18);
    cancelText.setFillColor(sf::Color::White);
    centerText(cancelText, cancelBtn);

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                window.close();
                return false;
            }
            else if (e.type == sf::Event::TextEntered) {
                unsigned int code = e.text.unicode;
                if (code == 8) {  // backspace
                    if (!input.empty()) {
                        input.pop_back();
                    }
                }
                else if (code >= 32 && code < 128) {
                    input.push_back(static_cast<char>(code));
                }
                textInput.setString(input);
            }
            else if (e.type == sf::Event::MouseButtonPressed &&
                     e.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(
                    static_cast<float>(e.mouseButton.x),
                    static_cast<float>(e.mouseButton.y)
                );
                if (acceptBtn.getGlobalBounds().contains(mousePos)) {
                    if(!input.empty()){
                    p->name = input;
                    window.close();
                    return true;}
                }
                if (cancelBtn.getGlobalBounds().contains(mousePos)) {
                    window.close();
                    return false;
                }
            }

            else if(e.type == sf::Event::KeyPressed){
                if(e.key.code == sf::Keyboard::Enter){
                    if(!input.empty()){
                    p->name = input;
                    window.close();
                    return true;}
                }
            }
        }

        window.clear(sf::Color(50, 50, 50));
        window.draw(prompt);
        window.draw(textInput);
        window.draw(acceptBtn);
        window.draw(acceptText);
        window.draw(cancelBtn);
        window.draw(cancelText);
        window.display();
    }

    return false;
}


void Game::runningGame(const sf::VideoMode& videoMode) {
    // Initialize window
    sf::RenderWindow gameWindow(videoMode,"Coup - Main Game",sf::Style::Titlebar | sf::Style::Close);
    gameWindow.setFramerateLimit(60);

    // Init state
    turnCounter = 0;
    currentPlayerIndex = 0;
    for (Player* p : players) p->endTurn = false;
    showAllCoins = false;

    // Prepare UI elements…

    // 1) Player info boxes (right side)
    const float boxW = 180.f, boxH = 60.f, boxX = videoMode.width - boxW - 10.f;
    for (std::size_t i = 0; i < players.size(); ++i) {
        sf::RectangleShape box({ boxW, boxH });
        box.setPosition(boxX, 10.f + i * (boxH + 10.f));
        box.setFillColor(sf::Color(50,50,50));
        box.setOutlineColor(sf::Color::White);
        box.setOutlineThickness(2.f);
        playerBoxes.push_back(box);

        sf::Text info("", /* your sf::Font& */ * /* your loaded font ptr */, 14);
        info.setFillColor(sf::Color::White);
        playerInfo.push_back(info);
    }

    // 2) View coins button
    viewCoinsBtn.setSize({160.f, 30.f});
    viewCoinsBtn.setPosition(10.f, 10.f);
    viewCoinsBtn.setFillColor(sf::Color(100,100,250));
    viewCoinsText.setFont(*/*font*/nullptr);
    viewCoinsText.setString("View coins: Only for Spies");
    viewCoinsText.setCharacterSize(14);
    viewCoinsText.setFillColor(sf::Color::White);
    centerText(viewCoinsText, viewCoinsBtn);

    // 3) Action buttons (2×3)
    const std::vector<std::string> labels = {
        "Gather", "Tax", "Bribe",
        "Arrest", "Sanction", "Coup"
    };
    float btnW = 120.f, btnH = 40.f, startX = 10.f, startY = 60.f, padX = 10.f, padY = 10.f;
    for (int row = 0; row < 2; ++row) {
        for (int col = 0; col < 3; ++col) {
            sf::RectangleShape b({btnW,btnH});
            b.setPosition(startX + col*(btnW+padX), startY + row*(btnH+padY));
            b.setFillColor(sf::Color(100,100,250));
            actionBtns.push_back(b);

            sf::Text t(labels[row*3+col], */*font*/nullptr, 14);
            t.setFillColor(sf::Color::White);
            centerText(t, b);
            actionBtnText.push_back(t);
        }
    }

    // 4) End-turn button
    endTurnBtn.setSize({120.f,30.f});
    endTurnBtn.setPosition(10.f, videoMode.height - 40.f);
    endTurnBtn.setFillColor(sf::Color(150,50,50));
    endTurnText.setFont(*/*font*/nullptr);
    endTurnText.setString("End Turn");
    endTurnText.setCharacterSize(14);
    endTurnText.setFillColor(sf::Color::White);
    centerText(endTurnText, endTurnBtn);

    // Main loop
    while (gameWindow->isOpen()) {
        sf::Event e;
        while (gameWindow->pollEvent(e)) {
            if (e.type == sf::Event::Closed)
                gameWindow->close();

            // View coins pressed
            else if (e.type == sf::Event::MouseButtonPressed &&
                     e.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f m(e.mouseButton.x,e.mouseButton.y);

                if (viewCoinsBtn.getGlobalBounds().contains(m)) {
                    // only spies may toggle
                    if (players[currentPlayerIndex]->role == Role::spy)
                        showAllCoins = !showAllCoins;
                }
                // action buttons
                for (int i = 0; i < 6; ++i) {
                    if (actionBtns[i].getGlobalBounds().contains(m)) {
                        Player* me = players[currentPlayerIndex];
                        if (me->endTurn) break;
                        switch (i) {
                          case 0: // Gather
                            if (!me->sanctioned) me->gather();
                            me->endTurn = true;
                            break;
                          case 1: { // Tax
                            if (!me->sanctioned) {
                              // popup “Governor block?” …
                              bool blocked = false; 
                              if (!blocked) me->tax();
                              me->endTurn = true;
                            }
                            break;
                          }
                          case 2: // Bribe
                            if (me->coins >= 4) {
                              me->bribe();
                              me->endTurn = true;
                            }
                            break;
                          case 3: // Arrest
                            // choose victim window …
                            /* Player* victim = … */;
                            /* if (victim != latestVictim) me->arrest(victim, latestVictim); */
                            /* me->endTurn = true; */
                            break;
                          case 4: // Sanction
                            if (me->coins >= 3) {
                              /* select victim */
                              /* me->sanction(victim); me->endTurn = true;*/
                            }
                            break;
                          case 5: // Coup
                            if (me->coins >= 7) {
                              /* select victim */
                              /* me->coup(victim); me->endTurn = true;*/
                              // check for winner:
                              int alive = 0; Player* last = nullptr;
                              for (Player* p : players)
                                if (p->role != Role::spectator)
                                  { alive++; last = p; }
                              if (alive == 1) {
                                Winner = last;
                                // popup winner window …
                                gameWindow->close();
                              }
                            }
                            break;
                        }
                    }
                }
                // end-turn button
                if (endTurnBtn.getGlobalBounds().contains(m)) {
                    // advance turn
                    do {
                      currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
                    } while (players[currentPlayerIndex]->role == Role::spectator);
                    players[currentPlayerIndex]->endTurn = false;
                    showAllCoins = false;
                }
            }
        }

        // draw everything
        gameWindow->clear(sf::Color(20,20,20));

        // draw player info boxes
        for (std::size_t i = 0; i < players.size(); ++i) {
            gameWindow->draw(playerBoxes[i]);
            Player* p = players[i];
            std::string s = p->name + "\n" + p->roleToString(p->role) + "\n";
            if (showAllCoins || i == currentPlayerIndex)
                s += "Coins: " + std::to_string(p->coins);
            else
                s += "Coins: ??";
            playerInfo[i].setString(s);
            playerInfo[i].setPosition(
              playerBoxes[i].getPosition().x+5,
              playerBoxes[i].getPosition().y+5
            );
            gameWindow->draw(playerInfo[i]);
        }

        // buttons
        gameWindow->draw(viewCoinsBtn);
        gameWindow->draw(viewCoinsText);
        for (int i = 0; i < 6; ++i) {
            gameWindow->draw(actionBtns[i]);
            gameWindow->draw(actionBtnText[i]);
        }
        gameWindow->draw(endTurnBtn);
        gameWindow->draw(endTurnText);

        gameWindow->display();
    }

    delete gameWindow;
}




