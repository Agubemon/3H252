#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Card.hpp"
#include <vector>
#include <random>
#include <algorithm>

// Estructura para una pila visual de cartas
struct VisualPile {
    std::vector<Card> cards;
    sf::Vector2f position;
    float offsetY = 30.f; // Separación vertical entre cartas
    
    VisualPile(float x, float y) : position(x, y) {}
    
    void addCard(Card& card) {
        cards.push_back(card);
        updatePositions();
    }
    
    void updatePositions() {
        for (size_t i = 0; i < cards.size(); ++i) {
            cards[i].setPosition(position.x, position.y + i * offsetY);
        }
    }
    
    void draw(sf::RenderWindow& window) {
        for (auto& card : cards) {
            card.draw(window);
        }
    }
};

int main() {
    // Crear ventana más grande para el tablero completo
    sf::RenderWindow window(sf::VideoMode(1000, 700), "Solitario Klondike");

    // Cargar texturas (solo una vez)
    sf::Texture spriteSheet;
    if (!spriteSheet.loadFromFile("assets/textures/sprite.png")) {
        return -1;
    }
    
    sf::Texture backTexture;
    if (!backTexture.loadFromFile("assets/textures/back.jpg")) {
        return -1;
    }

    // Cargar sonido de error
    sf::SoundBuffer errorBuffer;
    if (!errorBuffer.loadFromFile("assets/Sounds/AudioError.wav")) {
        // Si no se carga, continúa sin sonido
    }
    sf::Sound errorSound;
    errorSound.setBuffer(errorBuffer);

    // Crear y barajar el mazo (52 cartas)
    std::vector<Card> allCards;
    for (int s = 0; s <= 3; ++s) {  // 4 palos
        for (int r = 1; r <= 13; ++r) {  // 13 valores
            allCards.emplace_back((Suit)s, (Rank)r, spriteSheet, backTexture, 0.f, 0.f);
        }
    }
    
    // Barajar
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(allCards.begin(), allCards.end(), g);
    
    // Crear las 7 pilas del tableau (mesa)
    std::vector<VisualPile> tableauPiles;
    float startX = 50.f;
    float startY = 250.f;
    float spacingX = 140.f;
    
    for (int i = 0; i < 7; ++i) {
        tableauPiles.emplace_back(startX + i * spacingX, startY);
    }
    
    // Repartir las 28 cartas en el tableau
    int cardIndex = 0;
    for (int i = 0; i < 7; ++i) {  // Para cada pila
        for (int j = 0; j <= i; ++j) {  // Número de cartas en la pila
            if (cardIndex < allCards.size()) {
                Card& card = allCards[cardIndex];
                
                // Solo la última carta de cada pila está boca arriba
                if (j == i) {
                    card.flip();
                }
                
                tableauPiles[i].addCard(card);
                cardIndex++;
            }
        }
    }
    
    // Crear espacios para Foundation (4 pilas superiores derecha)
    std::vector<VisualPile> foundationPiles;
    float foundationStartX = 600.f;
    float foundationY = 30.f;
    
    for (int i = 0; i < 4; ++i) {
        foundationPiles.emplace_back(foundationStartX + i * spacingX, foundationY);
    }
    
    // Mazo restante (stock)
    std::vector<Card> stockPile;
    for (int i = cardIndex; i < allCards.size(); ++i) {
        stockPile.push_back(allCards[i]);
    }
    
    // Pila de descarte (waste)
    VisualPile wastePile(200.f, 30.f);

    // Variables para arrastrar cartas
    bool isDragging = false;
    int draggedPileIndex = -1;
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

                // Buscar en las pilas del tableau
                for (int i = 0; i < tableauPiles.size(); ++i) {
                    for (int j = tableauPiles[i].cards.size() - 1; j >= 0; --j) {
                        sf::FloatRect bounds(tableauPiles[i].cards[j].getPosition(), sf::Vector2f(142.f, 198.f));
                        if (bounds.contains(mousePos) && tableauPiles[i].cards[j].faceUp()) {
                            isDragging = true;
                            draggedPileIndex = i;
                            draggedCardIndex = j;
                            originalPosition = tableauPiles[i].cards[j].getPosition();
                            dragOffset = mousePos - tableauPiles[i].cards[j].getPosition();
                            break;
                        }
                    }
                    if (isDragging) break;
                }
            }

            // Mouse soltado - terminar arrastre
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                if (isDragging) {
                    bool isValidMove = false;
                    
                    // Por ahora, todos los movimientos son inválidos
                    // TODO: Implementar validación con canPlaceCard()

                    if (!isValidMove) {
                        errorSound.play();
                        tableauPiles[draggedPileIndex].cards[draggedCardIndex].setPosition(originalPosition.x, originalPosition.y);
                        tableauPiles[draggedPileIndex].updatePositions();
                    }

                    isDragging = false;
                    draggedPileIndex = -1;
                    draggedCardIndex = -1;
                }
            }

            // Mouse moviéndose - arrastrar carta
            if (event.type == sf::Event::MouseMoved && isDragging) {
                sf::Vector2f mousePos(event.mouseMove.x, event.mouseMove.y);
                tableauPiles[draggedPileIndex].cards[draggedCardIndex].setPosition(mousePos.x - dragOffset.x, mousePos.y - dragOffset.y);
            }
        }

        window.clear(sf::Color(0, 100, 0)); // Verde oscuro

        // Dibujar fondo para las posiciones del mazo y fundaciones
        sf::RectangleShape cardOutline(sf::Vector2f(142.f, 198.f));
        cardOutline.setFillColor(sf::Color(0, 80, 0));
        cardOutline.setOutlineColor(sf::Color::White);
        cardOutline.setOutlineThickness(2.f);
        
        // Mazo (stock) - esquina superior izquierda
        cardOutline.setPosition(50.f, 30.f);
        window.draw(cardOutline);
        
        // Pila de descarte (waste)
        wastePile.draw(window);
        
        // Fundaciones (4 espacios)
        for (int i = 0; i < 4; ++i) {
            cardOutline.setPosition(foundationStartX + i * spacingX, foundationY);
            window.draw(cardOutline);
            foundationPiles[i].draw(window);
        }

        // Dibujar las 7 pilas del tableau
        for (auto& pile : tableauPiles) {
            pile.draw(window);
        }

        window.display();
    }

    return 0;
}
