#include <SFML/Graphics.hpp>
int main() {
    sf::RenderWindow win({200, 200}, "SFML Test");
    sf::CircleShape c(100.f);
    c.setFillColor(sf::Color::Green);
    while (win.isOpen()) {
        sf::Event e;
        while (win.pollEvent(e))
          if (e.type == sf::Event::Closed) win.close();
        win.clear();
        win.draw(c);
        win.display();
    }
}