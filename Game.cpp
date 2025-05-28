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

Player* Game::chooseVictim(const std::string& title) {
    std::vector<Player*> candidates;
    for (Player* p : players) {
        if (p->role != Role::spectator && p != players[currentPlayerIndex])
            candidates.push_back(p);
    }
    int n = static_cast<int>(candidates.size());
    sf::RenderWindow win(sf::VideoMode(300, 60 * n), title, sf::Style::Titlebar|sf::Style::Close);
    win.setFramerateLimit(60);

    std::vector<sf::RectangleShape> btns(n);
    std::vector<sf::Text>           texts(n);
    sf::Font font; font.loadFromFile("Font/PIXEARG_.ttf");

    for (int i = 0; i < n; ++i) {
        btns[i].setSize({280.f, 50.f});
        btns[i].setPosition(10.f, 10.f + i * 55.f);
        btns[i].setFillColor(sf::Color(100,100,250));
        texts[i].setFont(font);
        texts[i].setString(candidates[i]->name);
        texts[i].setCharacterSize(18);
        texts[i].setFillColor(sf::Color::White);
        centerText(texts[i], btns[i]);
    }

    while (win.isOpen()) {
        sf::Event e;
        while (win.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                win.close();
                return nullptr;
            }
            if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f m(e.mouseButton.x, e.mouseButton.y);
                for (int i = 0; i < n; ++i) {
                    if (btns[i].getGlobalBounds().contains(m)) {
                        win.close();
                        return candidates[i];
                    }
                }
            }
        }
        win.clear(sf::Color(50,50,50));
        for (int i = 0; i < n; ++i) {
            win.draw(btns[i]);
            win.draw(texts[i]);
        }
        win.display();
    }
    return nullptr;
}

void Game::showWinnerWindow() {

    sf::RenderWindow win(sf::VideoMode(400,200), "We Have a Winner!", sf::Style::Titlebar|sf::Style::Close);
    win.setFramerateLimit(60);
    sf::Font font; font.loadFromFile("Font/PIXEARG_.ttf");
    sf::Text txt("Winner: " + Winner->name, font, 24);
    txt.setFillColor(sf::Color::Yellow);
    txt.setPosition(50.f, 50.f);

    sf::RectangleShape btn({200.f, 40.f});
    btn.setPosition(100.f, 120.f);
    btn.setFillColor(sf::Color(100,100,250));
    sf::Text t2("Go Back to Menu", font, 18);
    t2.setFont(font);
    t2.setFillColor(sf::Color::White);
    centerText(t2, btn);

    while (win.isOpen()) {
        sf::Event e;
        while (win.pollEvent(e)) {
            if (e.type == sf::Event::Closed) win.close();
            if (e.type == sf::Event::MouseButtonPressed &&
                e.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f m(e.mouseButton.x, e.mouseButton.y);
                if (btn.getGlobalBounds().contains(m))
                    win.close();
            }
        }
        win.clear(sf::Color(20,20,20));
        win.draw(txt);
        win.draw(btn);
        win.draw(t2);
        win.display();
    }
}

bool Game::governorBlockPrompt(const std::string& governorName) {
    sf::RenderWindow win(
        sf::VideoMode(300,150),
        "Governor Block?",
        sf::Style::Titlebar|sf::Style::Close
    );
    win.setFramerateLimit(60);

    sf::Font font;  font.loadFromFile("Font/PIXEARG_.ttf");
    sf::Text prompt(
        governorName + " block this move?",
        font, 18
    );
    prompt.setFillColor(sf::Color::White);
    prompt.setPosition(20.f, 20.f);

    sf::RectangleShape yesBtn({100.f,40.f});
    yesBtn.setPosition(30.f, 80.f);
    yesBtn.setFillColor(sf::Color(100,200,100));
    sf::Text yesTxt("Yes", font, 18);
    yesTxt.setFillColor(sf::Color::White);
    centerText(yesTxt, yesBtn);

    sf::RectangleShape noBtn({100.f,40.f});
    noBtn.setPosition(170.f, 80.f);
    noBtn.setFillColor(sf::Color(200,100,100));
    sf::Text noTxt("No", font, 18);
    noTxt.setFillColor(sf::Color::White);
    centerText(noTxt, noBtn);

    while (win.isOpen()) {
        sf::Event e;
        while (win.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                win.close();
                return false;
            }
            if (e.type == sf::Event::MouseButtonPressed &&
                e.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f m(e.mouseButton.x, e.mouseButton.y);
                if (yesBtn.getGlobalBounds().contains(m)) {
                    win.close();
                    return true;
                }
                if (noBtn.getGlobalBounds().contains(m)) {
                    win.close();
                    return false;
                }
            }
        }
        win.clear(sf::Color(50,50,50));
        win.draw(prompt);
        win.draw(yesBtn); win.draw(yesTxt);
        win.draw(noBtn);  win.draw(noTxt);
        win.display();
    }
    return false;
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

    sf::Font fontt;
    fontt.loadFromFile("Font/PIXEARG_.ttf"); 

    // 1) Player info boxes (right side)
    std::vector<sf::RectangleShape> playerBoxes;
    std::vector<sf::Text> playerInfo;
    const float boxW = 180.f, boxH = 60.f, boxX = videoMode.width - boxW - 10.f;
    for (std::size_t i = 0; i < players.size(); ++i) {
        sf::RectangleShape box({ boxW, boxH });
        box.setPosition(boxX, 10.f + i * (boxH + 10.f));
        box.setFillColor(sf::Color(50,50,50));
        box.setOutlineColor(sf::Color::White);
        box.setOutlineThickness(2.f);
        playerBoxes.push_back(box);

        sf::Text info("", fontt, 14);///////sf::Text info("", /* your sf::Font& */ * /* your loaded font ptr */, 14);
        info.setFillColor(sf::Color::White);
        playerInfo.push_back(info);
    }

    // 2) View coins button
    sf::RectangleShape viewCoinsBtn;
    viewCoinsBtn.setSize({160.f, 30.f});
    viewCoinsBtn.setPosition(10.f, 10.f);
    viewCoinsBtn.setFillColor(sf::Color(100,100,250));
    sf::Text viewCoinsText;
    viewCoinsText.setFont(fontt);
    viewCoinsText.setString("View coins: Only for Spies");
    viewCoinsText.setCharacterSize(14);
    viewCoinsText.setFillColor(sf::Color::White);
    centerText(viewCoinsText, viewCoinsBtn);

    // 3) Action buttons (2×3)
    const std::vector<std::string> labels = {
        "Gather", "Tax", "Bribe",
        "Arrest", "Sanction", "Coup"
    };

    // Spy‐only “Block Arrest” button
    sf::RectangleShape spyBlockBtn;
    spyBlockBtn.setSize({160.f, 30.f});
    spyBlockBtn.setPosition(300.f, 100.f);
    spyBlockBtn.setFillColor(sf::Color(150,150,50));
    sf::Text spyBlockText;
    spyBlockText.setFont(fontt);
    spyBlockText.setString("Block Arrest");
    spyBlockText.setCharacterSize(14);
    spyBlockText.setFillColor(sf::Color::White);
    centerText(spyBlockText, spyBlockBtn);

    std::vector<sf::RectangleShape> actionBtns;
    std::vector<sf::Text> actionBtnText;
    float btnW = 120.f, btnH = 40.f, startX = 10.f, startY = 60.f, padX = 10.f, padY = 10.f;
    for (int row = 0; row < 2; ++row) {
        for (int col = 0; col < 3; ++col) {
            sf::RectangleShape b({btnW,btnH});
            b.setPosition(startX + col*(btnW+padX), startY + row*(btnH+padY));
            b.setFillColor(sf::Color(100,100,250));
            actionBtns.push_back(b);

            sf::Text t(labels[row*3+col],fontt, 14);
            t.setFillColor(sf::Color::White);
            centerText(t, b);
            actionBtnText.push_back(t);
        }
    }

    // 4) End-turn button
    sf::RectangleShape endTurnBtn;
    sf::Text endTurnText;
    endTurnBtn.setSize({120.f,30.f});
    endTurnBtn.setPosition(10.f, videoMode.height - 40.f);
    endTurnBtn.setFillColor(sf::Color(150,50,50));
    endTurnText.setFont(fontt);
    endTurnText.setString("End Turn");
    endTurnText.setCharacterSize(14);
    endTurnText.setFillColor(sf::Color::White);
    centerText(endTurnText, endTurnBtn);

    // Main loop
    while (gameWindow.isOpen()) {
        sf::Event e;
        while (gameWindow.pollEvent(e)) {
            if (e.type == sf::Event::Closed)
                gameWindow.close();

            // View coins pressed
            else if (e.type == sf::Event::MouseButtonPressed &&
                     e.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f m(e.mouseButton.x,e.mouseButton.y);

                if (viewCoinsBtn.getGlobalBounds().contains(m)) {
                    // only spies may toggle
                    if (players[currentPlayerIndex]->role == Role::spy)
                        showAllCoins = !showAllCoins;
                }
                    // Spy Block Arrest
                    if (players[currentPlayerIndex]->role == Role::spy &&
                        spyBlockBtn.getGlobalBounds().contains(m))
                    {
                        Player* victim = chooseVictim("Choose player to block arrest");
                        if (victim) {
                            victim->blockedFromArrest = true;
                        }
                    }
                // action buttons
                for (int i = 0; i < 6; ++i) {
                    if (actionBtns[i].getGlobalBounds().contains(m)) {
                        Player* me = players[currentPlayerIndex];
                        switch (i) {
                          case 0: // Gather
                            if (!me->sanctioned && !me->endTurn) me->gather();
                            me->endTurn = true;
                            break;
                          case 1: { // Tax
                            if (!me->sanctioned && !me->endTurn) {
                            bool blocked = false;
                            for (Player* p : players) {
                            if (p != me && p->role == Role::governor) {
                                blocked = governorBlockPrompt(p->name);
                                break;
                            }
                            }
                            if (!blocked) me->tax();
                            me->endTurn = true;

                            }
                            break;
                          }
                          case 2: // Bribe
                            if (me->coins >= 4 && !me->endTurn) {
                              me->bribe();
                              me->arrestBlocked = false;
                              me->endTurn = true;
                            }
                            break;
                          case 3: // Arrest
                          if (!me->endTurn && !me->arrestBlocked) {
                                Player* victim = chooseVictim("Choose Arrest Target");
                                if (victim && victim != latestVictim) {
                                    me->arrest(victim, latestVictim);
                                    latestVictim = victim;
                                    me->endTurn = true;
                                }
                            }
                            break;
                          case 4: // Sanction
                                if (!me->endTurn && me->coins >= 3) {
                                Player* victim = chooseVictim("Choose Sanction Target");
                                if (victim) {
                                    me->sanction(victim);
                                    me->arrestBlocked = false;
                                    me->endTurn = true;
                                }
                            }
                            break;
                          case 5: // Coup
                                if (!me->endTurn && me->coins >= 7) {
                                Player* victim = chooseVictim("Choose Coup Target");
                                if (victim) {
                                    me->coup(victim);
                                    me->arrestBlocked = false;
                                    me->endTurn = true;
                                    int alive = 0;
                                    for (Player* p : players)
                                        if (p->role != Role::spectator) {
                                            alive++;
                                            Winner = p;
                                        }
                                    if (alive == 1) {
                                        gameWindow.close();
                                        showWinnerWindow();
                                    }
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
        gameWindow.clear(sf::Color(20,20,20));

        // draw player info boxes
        for (std::size_t i = 0; i < players.size(); ++i) {
            gameWindow.draw(playerBoxes[i]);
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
            gameWindow.draw(playerInfo[i]);
        }

        // buttons
        gameWindow.draw(viewCoinsBtn);
        gameWindow.draw(viewCoinsText);
        for (int i = 0; i < 6; ++i) {
            gameWindow.draw(actionBtns[i]);
            gameWindow.draw(actionBtnText[i]);
        }
        gameWindow.draw(endTurnBtn);
        gameWindow.draw(endTurnText);

        if (players[currentPlayerIndex]->role == Role::spy) {
        gameWindow.draw(spyBlockBtn);
        gameWindow.draw(spyBlockText);
    }

        gameWindow.display();
    }

    gameWindow.close();
}




