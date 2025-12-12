#include <SFML/Graphics.hpp>
#include "Card.hpp"
#include <vector>

int main() {
    // Crear ventana
    sf::RenderWindow window(sf::VideoMode(800, 600), "Solitario - Arrastrar Cartas");

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

    // Variables para arrastrar cartas
    bool isDragging = false;
    int draggedCardIndex = -1;
    sf::Vector2f originalPosition;
    sf::Vector2f dragOffset;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Mouse presionado - iniciar arrastre
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

                // Buscar si se clickeó una carta (de atrás hacia adelante para priorizar las de arriba)
                for (int i = cards.size() - 1; i >= 0; --i) {
                    sf::FloatRect bounds(cards[i].getPosition(), sf::Vector2f(142.f, 198.f));
                    if (bounds.contains(mousePos)) {
                        isDragging = true;
                        draggedCardIndex = i;
                        originalPosition = cards[i].getPosition();
                        dragOffset = mousePos - cards[i].getPosition();
                        break;
                    }
                }
            }

            // Mouse soltado - terminar arrastre
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                if (isDragging) {
                    // Aquí validarías si el movimiento es válido
                    // Por ahora, siempre regresamos la carta a su posición original
                    bool isValidMove = false; // TODO: Implementar validación con canPlaceCard()

                    if (!isValidMove) {
                        // Regresar a posición original
                        cards[draggedCardIndex].setPosition(originalPosition.x, originalPosition.y);
                    }

                    isDragging = false;
                    draggedCardIndex = -1;
                }
            }

            // Mouse moviéndose - arrastrar carta
            if (event.type == sf::Event::MouseMoved && isDragging) {
                sf::Vector2f mousePos(event.mouseMove.x, event.mouseMove.y);
                cards[draggedCardIndex].setPosition(mousePos.x - dragOffset.x, mousePos.y - dragOffset.y);
            }
        }

        window.clear(sf::Color::Green);

        for (auto& card : cards)
            card.draw(window);

        window.display();
    }

    return 0;
}
