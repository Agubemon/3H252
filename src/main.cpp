#include <SFML/Graphics.hpp>
#include "Card.hpp"
#include <vector>

int main() {
    // Crear ventana
    sf::RenderWindow window(sf::VideoMode(800, 600), "Solitario - Sprite Sheet");

    // Cargar texturas (solo una vez)
    sf::Texture spriteSheet;
    if (!spriteSheet.loadFromFile("assets/textures/sprite.png")) {
        return -1; // Error al cargar
    }
    
    sf::Texture backTexture;
    if (!backTexture.loadFromFile("assets/textures/back.png")) {
        return -1; // Error al cargar
    }

    // Crear cartas usando el nuevo sistema
    std::vector<Card> cards;
    
    // Ejemplo: As de Corazones, Rey de Picas, Cinco de Diamantes
    cards.emplace_back(Suit::HEARTS, Rank::ACE, spriteSheet, backTexture, 100.f, 100.f);
    cards.emplace_back(Suit::SPADES, Rank::KING, spriteSheet, backTexture, 250.f, 100.f);
    cards.emplace_back(Suit::DIAMONDS, Rank::FIVE, spriteSheet, backTexture, 400.f, 100.f);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Click para voltear
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

                for (auto& card : cards) {
                    sf::FloatRect bounds(card.getPosition(), sf::Vector2f(142.f, 198.f));
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
