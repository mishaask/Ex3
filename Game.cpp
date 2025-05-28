#include "Game.hpp"



void Game::centerText(sf::Text& txt, const sf::RectangleShape& btn) {
    sf::FloatRect tb = txt.getLocalBounds();
    txt.setOrigin(tb.left + tb.width/2.f, tb.top + tb.height/2.f);
    sf::FloatRect bb = btn.getGlobalBounds();
    txt.setPosition(bb.left + bb.width/2.f, bb.top + bb.height/2.f);
}

Role Game::assignRole(Player* p) {
    static std::random_device rd;
    static std::mt19937       gen(rd());

    // list out the roles we want to choose from:
    static const Role options[] = {
        Role::baron,
        Role::general,
        Role::governor,
        Role::judge,
        Role::merchant,
        Role::spy
    };

    // pick a random index into our options array
    std::uniform_int_distribution<int> dist(
        0,
        sizeof(options) / sizeof(options[0]) - 1
    );

    int index = dist(gen);
    Role r = options[index];
    p->role = r;
    return r;
}

void Game::launchGame(const sf::VideoMode& videoMode,int playerCount) {
    // 1) delete old, rebuild players list
    for (std::vector<Player*>::iterator it = players.begin(); it != players.end(); ++it) {
        delete *it;
    }
    players.clear();
    players.reserve(playerCount);
    for (std::size_t i = 0; i < playerCount; ++i) {
        players.push_back(new Player());
    }

    // 2) name & assign
    for (std::size_t i = 0; i < players.size(); ++i) {
        players[i]->setId((int)i);
        bool ok = getsetName(players[i], i);
        if (!ok) {
            // user hit Cancel → abort
            // for (std::vector<Player*>::iterator pit = players.begin(); pit != players.end(); ++pit) {
            //     (*pit)->name = "null";
            // }
            return;
        }
        assignRole(players[i]);
    }

    // 3) …now you have fully named, role-assigned players.
    //     Launch your main SFML game window here.
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




