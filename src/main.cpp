#include <SFML/Graphics.hpp>
#include "Card.hpp"
#include <vector>

int main() {
    // Crear ventana
    sf::RenderWindow window(sf::VideoMode(800, 600), "Fase 4 - Voltear Cartas");

    // Crear cartas
    std::vector<Card> cards;
    cards.emplace_back("assets/textures/front.png", "assets/textures/back.png", 100.f, 100.f);
    cards.emplace_back("assets/textures/front.png", "assets/textures/back.png", 250.f, 100.f);
    cards.emplace_back("assets/textures/front.png", "assets/textures/back.png", 400.f, 100.f);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Click para voltear
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

                for (auto& card : cards) {
                    sf::FloatRect bounds(card.getPosition(), sf::Vector2f(100.f, 150.f)); // tamaño aproximado de la carta
                    if (bounds.contains(mousePos)) {
                        card.flip();
                    }
                }
            }
        }

        window.clear(sf::Color::Green);

        for (auto& card : cards)
            card.draw(window);

        window.display();
    }

    return 0;
}
