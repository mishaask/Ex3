//mishaaskarov@gmail.com
#include <filesystem>//debuggin
#include "HomeScreen.hpp"

void HomeScreen::initVariables()
{
    this->window = nullptr;
}

void HomeScreen::initWindow()
{

    this->videoMode.height = 600;
    this->videoMode.width = 800;
    this->window = new sf::RenderWindow(this->videoMode,"Coup",sf::Style::Titlebar|sf::Style::Close);

    this->window->setFramerateLimit(60);
}

void HomeScreen::initButtons()
{
    // Example: 3 buttons stacked vertically
    const float btnW = 200.f;
    const float btnH = 50.f;
    const float startX = 300.f;
    const float startY = 400.f;
    const float padding = 20.f;

    for (std::size_t i = 0; i < 3; ++i)
    {
        this->button.setSize({ btnW, btnH });
        this->button.setPosition({ startX, startY + i * (btnH + padding) });
        this->button.setFillColor(sf::Color(100, 100, 250));      // light blue
        this->button.setOutlineColor(sf::Color::White);
        this->button.setOutlineThickness(2.f);
        this->buttons.push_back(this->button);
    }
}

void HomeScreen::initButtonLabels()
{
    // Labels for each button:
    std::vector<std::string> labels = {
        "Start Game",
        "Options",
        "Exit"
    };

    for (std::size_t i = 0; i < this->buttons.size(); ++i)
    {
        sf::Text txt;
        txt.setFont(this->font);
        txt.setString(labels[i]);
        txt.setCharacterSize(24);                    // in pixels
        txt.setFillColor(sf::Color::White);

        // center text in the button:
        sf::FloatRect btnBounds = this->buttons[i].getGlobalBounds();
        sf::FloatRect textBounds = txt.getLocalBounds();
        txt.setOrigin(textBounds.left + textBounds.width  / 2.0f,
                      textBounds.top  + textBounds.height / 2.0f);
        txt.setPosition(
            btnBounds.left + btnBounds.width  / 2.0f,
            btnBounds.top  + btnBounds.height / 2.0f
        );

        this->buttonTexts.push_back(txt);
    }
}

void HomeScreen::initEnemies()
{
    this->enemy.setPosition(sf::Vector2f(100.f,100.f));
    this->enemy.setSize(sf::Vector2f(80.f, 50.f));
    this->enemy.setFillColor(sf::Color::Magenta);
    this->enemy.setOutlineColor(sf::Color::Black);
    this->enemy.setOutlineThickness(5.f);
}

HomeScreen::HomeScreen()
{
    this->initVariables();
    this->initWindow();
    
    if (!this->font.loadFromFile("Font/PIXEARG_.ttf")) {
        std::cerr << "ERROR: could not load font\n";
        // handle failure…
    }
    
    this->initEnemies();
    this->initButtons();
    this->initButtonLabels(); 
}

HomeScreen::~HomeScreen()
{
    delete this->window;
}

//Accessors
bool HomeScreen::running() const
{
    return this->window->isOpen();
}

//Event handler handles user input every frame
//Functions
void HomeScreen::pollEvents()
{
        //Event polling
        while(this->window->pollEvent(this->ev))
        {
            switch(this->ev.type)
            {
                case sf::Event::Closed:
                {
                    this->window->close();
                    break;
                }
                
                //Event = Mouse click
                case sf::Event::MouseButtonPressed:
                {
                    //Event = Left mouse click
                    if (ev.mouseButton.button == sf::Mouse::Left)
                    {
                        //Get mouse = mouse click position
                        sf::Vector2i mouse = sf::Mouse::getPosition(*window);
                        for (std::size_t i = 0; i < this->buttons.size(); ++i)
                        {
                            //Check if one of the buttons.contains(mouse.position)
                            if (this->buttons[i].getGlobalBounds().contains(
                                    static_cast<float>(mouse.x),
                                    static_cast<float>(mouse.y)))
                            {
                                //cmd print which button pressed
                                std::cout << "Button " << i << " clicked!\n";
                                
                                //Start button pressed
                                if (i == 0){
                                    //Open "Choose number of players" window
                                    int numPlayers = this->openPlayerCountWindow();
                                    
                                    //if number of players changed from zero(succesfull exec) launch game
                                    if (numPlayers > 0)
                                    {
                                        //launch game with number of players and video settings
                                        //launchGame implemented in Game class
                                        this->game.launchGame(videoMode, numPlayers);
                                    }
                                }

                                //Exit button pressed
                                if (i == 2) {this->window->close();}
                            }
                        }
                    }
                    break;
                }
                
                //Event = Key pressed
                case sf::Event::KeyPressed:
                {
                    //Event = Escape key pressed
                    if (this->ev.key.code == sf::Keyboard::Escape)
                        this->window->close();
                    break;
                }

                default:
                    break;

            }
        }

}



/// @brief updates mouse position
void HomeScreen::updateMousePositions()
{
    this->mousePosWindow = sf::Mouse::getPosition(*this->window);
}


// Helper function to center text in a rectangle
static void centerText(sf::Text &txt, const sf::RectangleShape &btn)
{
    // measure the text's local bounds (its untransformed size)
    sf::FloatRect tb = txt.getLocalBounds();
    // shift its origin to its own center
    txt.setOrigin(
        tb.left   + tb.width  / 2.0f,
        tb.top    + tb.height / 2.0f
    );
    // measure the button's global bounds (position + size)
    sf::FloatRect bb = btn.getGlobalBounds();
    // place the text so that its center (=origin) lands at the button's center
    txt.setPosition(
        bb.left   + bb.width  / 2.0f,
        bb.top    + bb.height / 2.0f
    );
}

int HomeScreen::openPlayerCountWindow()
{
    // 1) Create the small window
    sf::RenderWindow cwindow(sf::VideoMode(400, 200),"Pick Number of Players",sf::Style::Titlebar | sf::Style::Close);
    cwindow.setFramerateLimit(60);

    // 2) UI state
    int count = 2;           // start at 2
    const int MIN = 2, MAX = 6;

    // 3) Shapes for '–', '+', 'Start', 'Exit'
    sf::RectangleShape decBtn({40,40}), incBtn({40,40}), startBtn({100,40}), exitBtn({100,30});
    decBtn.setPosition( 50,  80);
    incBtn.setPosition(310,  80);
    startBtn.setPosition( 80, 140);
    exitBtn .setPosition(220, 140);
    for (sf::RectangleShape* b : { &decBtn,&incBtn,&startBtn,&exitBtn })
    {
        b->setFillColor(sf::Color(100,100,250));
        b->setOutlineColor(sf::Color::White);
        b->setOutlineThickness(2.f);
    }

    // 4) Text labels
    sf::Text title, numText, tDec, tInc, tStart, tExit;
    title.setFont(this->font);
    title.setString("Pick number of players");
    title.setCharacterSize(20);
    title.setFillColor(sf::Color::White);
    title.setPosition( 40, 20);

    // “–” and “+”
    tDec.setFont(this->font);  tDec.setString("-");  tDec.setCharacterSize(30);  centerText(tDec, decBtn);
    tInc.setFont(this->font);  tInc.setString("+");  tInc.setCharacterSize(30);  centerText(tInc, incBtn);

    // number display
    numText.setFont(this->font);
    numText.setCharacterSize(28);
    numText.setFillColor(sf::Color::White);

    // “Start” & “Exit” labels
    tStart.setFont(this->font); tStart.setString("Start"); tStart.setCharacterSize(20); centerText(tStart, startBtn);
    tExit .setFont(this->font); tExit .setString("Back");  tExit .setCharacterSize(18); centerText(tExit,  exitBtn);

    // 5) Loop until they click Start or Back
    while (cwindow.isOpen())
    {
        // update the number text each frame
        {
            std::ostringstream ss; ss << count;
            numText.setString(ss.str());
            // position it between the buttons:
            sf::FloatRect ntb = numText.getLocalBounds();
            numText.setOrigin(ntb.left + ntb.width/2, ntb.top + ntb.height/2);
            numText.setPosition(200, 100);
        }

        sf::Event e;
        while (cwindow.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
            {
                cwindow.close();
                return 0;
            }
            else if (e.type == sf::Event::MouseButtonPressed
                  && e.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f m( e.mouseButton.x, e.mouseButton.y);

                if (decBtn.getGlobalBounds().contains(m) && count > MIN){
                    --count;
                    std::cout<<"count is "<<count<< "\n";}
                else if (incBtn.getGlobalBounds().contains(m) && count < MAX){
                    ++count;
                    std::cout<<"count is "<<count<< "\n";}
                else if (startBtn.getGlobalBounds().contains(m))
                {
                    cwindow.close();
                    std::cout<<"count is "<<count<< "\n";
                    return count;
                }
                else if (exitBtn.getGlobalBounds().contains(m))
                {
                    cwindow.close();
                    return 0;
                }
            }
            else if(e.type == sf::Event::KeyPressed){
                if(e.key.code == sf::Keyboard::Enter){
                    cwindow.close();
                    return count;
                }
            }
        }

        // Draw everything
        cwindow.clear(sf::Color(50,50,50));
        cwindow.draw(title);
        cwindow.draw(decBtn); cwindow.draw(incBtn);
        cwindow.draw(numText);
        cwindow.draw(startBtn); cwindow.draw(exitBtn);
        cwindow.draw(tDec);  cwindow.draw(tInc);
        cwindow.draw(tStart);cwindow.draw(tExit);
        cwindow.display();
    }

    return 0;
}

void HomeScreen::update()
{

    //check whats happening in current frame
    this->pollEvents();

    //Update mouse position
    updateMousePositions();

    // //relative to screen
    // // std::cout << "Mouse pos: " 
    // // << sf::Mouse::getPosition().x << " " 
    // // << sf::Mouse::getPosition().y << std::endl;

    // //relative to window
    // std::cout << "Mouse pos: " 
    //     << sf::Mouse::getPosition(*this->window).x << " " 
    //     << sf::Mouse::getPosition(*this->window).y << std::endl;
}

/**
 * clear old frame
 * render objects
 * display frame in window
 * 
 * Renders the game objects
 */
void HomeScreen::render()
{
    this->window->clear(sf::Color::Blue);

    //Draw game object
    this->window->draw(this->enemy);

    // Draw buttons
    for(sf::RectangleShape& button : this->buttons)
        this->window->draw(button);
    
    //Draw button text
    for (sf::Text& txt : this->buttonTexts)
        this->window->draw(txt);

    this->window->display();
}