#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Card.hpp"
#include <vector>
#include <random>
#include <algorithm>
#include <cmath>

// Estructura para una pila visual de cartas
struct VisualPile {
    std::vector<Card> cards;
    sf::Vector2f position;
    float offsetY = 30.f; // Separación vertical entre cartas
    bool showOnlyTop = false; // Si es true, solo muestra la carta superior
    
    VisualPile(float x, float y, bool onlyTop = false) : position(x, y), showOnlyTop(onlyTop) {}
    
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
        if (showOnlyTop && !cards.empty()) {
            // Solo dibujar la carta superior
            cards.back().draw(window);
        } else {
            // Dibujar todas las cartas
            for (auto& card : cards) {
                card.draw(window);
            }
        }
    }
};

int main() {
    // Crear ventana más grande para el tablero completo
    sf::RenderWindow window(sf::VideoMode(1150, 700), "Solitario Klondike");

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
    
    // Cargar sonido de victoria
    sf::SoundBuffer winBuffer;
    if (!winBuffer.loadFromFile("assets/Sounds/AudioGanar.wav")) {
        // Si no se carga, continúa sin sonido
    }
    sf::Sound winSound;
    winSound.setBuffer(winBuffer);
    
    bool gameWon = false; // Para reproducir el sonido solo una vez

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
    float foundationStartX = 520.f;
    float foundationY = 30.f;
    
    for (int i = 0; i < 4; ++i) {
        foundationPiles.emplace_back(foundationStartX + i * spacingX, foundationY, true);
        foundationPiles[i].offsetY = 0.f; // Sin separación, apiladas en la misma posición
    }
    
    // Mazo restante (stock) - con posición
    std::vector<Card*> stockPile;
    for (int i = cardIndex; i < allCards.size(); ++i) {
        allCards[i].setPosition(50.f, 30.f); // Posición del mazo
        stockPile.push_back(&allCards[i]);
    }
    
    // Pila de descarte (waste) - solo muestra la carta superior, sin separación
    VisualPile wastePile(200.f, 30.f, true);
    wastePile.offsetY = 0.f; // Sin separación vertical, todas en la misma posición

    // Variables para arrastrar cartas
    bool isDragging = false;
    int draggedPileIndex = -1;
    int draggedCardIndex = -1;
    sf::Vector2f originalPosition;
    sf::Vector2f dragOffset;
    
    // Variables para detectar doble clic
    sf::Clock doubleClock;
    sf::Time lastClickTime = sf::Time::Zero;
    const sf::Time doubleClickThreshold = sf::milliseconds(300);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Mouse presionado - iniciar arrastre o detectar doble clic
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                
                sf::Time currentTime = doubleClock.getElapsedTime();
                bool isDoubleClick = (currentTime - lastClickTime) < doubleClickThreshold;
                lastClickTime = currentTime;

                // Verificar si se hizo clic en el mazo (stock)
                sf::FloatRect stockBounds(50.f, 30.f, 142.f, 198.f);
                if (stockBounds.contains(mousePos)) {
                    if (!stockPile.empty()) {
                        // Voltear una carta del mazo al waste
                        Card* topCard = stockPile.back();
                        stockPile.pop_back();
                        topCard->flip(); // Voltear boca arriba
                        wastePile.addCard(*topCard);
                    } else if (!wastePile.cards.empty()) {
                        // Si el mazo está vacío pero hay cartas en waste, reciclar
                        // Mover todas las cartas del waste de vuelta al stock
                        while (!wastePile.cards.empty()) {
                            Card& card = wastePile.cards.back();
                            card.flip(); // Voltear boca abajo
                            card.setPosition(50.f, 30.f); // Posición del stock
                            stockPile.push_back(&card);
                            wastePile.cards.pop_back();
                        }
                        // Invertir el orden para que la última carta volteada sea la primera en salir
                        std::reverse(stockPile.begin(), stockPile.end());
                    }
                    continue; // No procesar arrastre
                }
                
                // Función para intentar mover carta a Foundation automáticamente
                auto tryAutoMoveToFoundation = [&](Card* card, int sourcePileIndex) {
                    // Buscar Foundation apropiada
                    for (int i = 0; i < 4; ++i) {
                        if (foundationPiles[i].cards.empty()) {
                            // Pila vacía: solo acepta As
                            if (card->getRank() == Rank::ACE) {
                                foundationPiles[i].addCard(*card);
                                
                                // Remover de origen
                                if (sourcePileIndex == -2) {
                                    wastePile.cards.pop_back();
                                } else {
                                    tableauPiles[sourcePileIndex].cards.pop_back();
                                    // Voltear siguiente carta
                                    if (!tableauPiles[sourcePileIndex].cards.empty()) {
                                        Card& nextCard = tableauPiles[sourcePileIndex].cards.back();
                                        if (!nextCard.faceUp()) nextCard.flip();
                                    }
                                }
                                return true;
                            }
                        } else {
                            // Pila con cartas: verificar si coincide
                            Card& topCard = foundationPiles[i].cards.back();
                            if (card->getSuit() == topCard.getSuit() &&
                                (int)card->getRank() == (int)topCard.getRank() + 1) {
                                foundationPiles[i].addCard(*card);
                                
                                // Remover de origen
                                if (sourcePileIndex == -2) {
                                    wastePile.cards.pop_back();
                                } else {
                                    tableauPiles[sourcePileIndex].cards.pop_back();
                                    // Voltear siguiente carta
                                    if (!tableauPiles[sourcePileIndex].cards.empty()) {
                                        Card& nextCard = tableauPiles[sourcePileIndex].cards.back();
                                        if (!nextCard.faceUp()) nextCard.flip();
                                    }
                                }
                                return true;
                            }
                        }
                    }
                    return false;
                };

                // Verificar doble clic en waste pile
                if (isDoubleClick && !wastePile.cards.empty()) {
                    sf::FloatRect wasteBounds(wastePile.cards.back().getPosition(), sf::Vector2f(142.f, 198.f));
                    if (wasteBounds.contains(mousePos)) {
                        tryAutoMoveToFoundation(&wastePile.cards.back(), -2);
                        continue;
                    }
                }
                
                // Verificar doble clic en tableau
                if (isDoubleClick) {
                    for (int i = 0; i < tableauPiles.size(); ++i) {
                        if (!tableauPiles[i].cards.empty()) {
                            Card& topCard = tableauPiles[i].cards.back();
                            if (topCard.faceUp()) {
                                sf::FloatRect bounds(topCard.getPosition(), sf::Vector2f(142.f, 198.f));
                                if (bounds.contains(mousePos)) {
                                    tryAutoMoveToFoundation(&topCard, i);
                                    continue;
                                }
                            }
                        }
                    }
                }

                // Verificar si se hizo clic en el waste pile (para arrastrar)
                if (!wastePile.cards.empty()) {
                    sf::FloatRect wasteBounds(wastePile.cards.back().getPosition(), sf::Vector2f(142.f, 198.f));
                    if (wasteBounds.contains(mousePos)) {
                        isDragging = true;
                        draggedPileIndex = -2; // Código especial para waste
                        draggedCardIndex = wastePile.cards.size() - 1;
                        originalPosition = wastePile.cards.back().getPosition();
                        dragOffset = mousePos - wastePile.cards.back().getPosition();
                        continue;
                    }
                }

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
                    int targetFoundationIndex = -1;
                    int targetTableauIndex = -1;
                    
                    sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                    
                    // Obtener la carta que se está arrastrando
                    Card* draggedCard = nullptr;
                    if (draggedPileIndex == -2) {
                        draggedCard = &wastePile.cards[draggedCardIndex];
                    } else {
                        draggedCard = &tableauPiles[draggedPileIndex].cards[draggedCardIndex];
                    }
                    
                    // Verificar si se soltó sobre una Foundation pile
                    for (int i = 0; i < 4; ++i) {
                        sf::FloatRect foundationBounds(foundationStartX + i * spacingX, foundationY, 142.f, 198.f);
                        if (foundationBounds.contains(mousePos)) {
                            targetFoundationIndex = i;
                            
                            // Validar reglas de Foundation
                            if (foundationPiles[i].cards.empty()) {
                                // Pila vacía: solo acepta As
                                if (draggedCard->getRank() == Rank::ACE) {
                                    isValidMove = true;
                                }
                            } else {
                                // Pila con cartas: debe ser mismo palo y rango consecutivo
                                Card& topCard = foundationPiles[i].cards.back();
                                if (draggedCard->getSuit() == topCard.getSuit() &&
                                    (int)draggedCard->getRank() == (int)topCard.getRank() + 1) {
                                    isValidMove = true;
                                }
                            }
                            break;
                        }
                    }
                    
                    // Si no es Foundation, verificar Tableau
                    if (!isValidMove && targetFoundationIndex == -1) {
                        for (int i = 0; i < tableauPiles.size(); ++i) {
                            // Calcular bounds del área de la pila
                            sf::FloatRect tableauBounds(startX + i * spacingX, startY, 142.f, 400.f);
                            if (tableauBounds.contains(mousePos)) {
                                targetTableauIndex = i;
                                
                                // No permitir mover carta a su misma pila
                                if (draggedPileIndex == i) {
                                    break;
                                }
                                
                                // Validar reglas de Tableau
                                if (tableauPiles[i].cards.empty()) {
                                    // Pila vacía: solo acepta Rey
                                    if (draggedCard->getRank() == Rank::KING) {
                                        isValidMove = true;
                                    }
                                } else {
                                    // Pila con cartas: debe ser rango menor en 1 y color diferente
                                    Card& topCard = tableauPiles[i].cards.back();
                                    if (topCard.faceUp()) { // Solo si la carta superior está boca arriba
                                        if ((int)draggedCard->getRank() == (int)topCard.getRank() - 1 &&
                                            draggedCard->isRed() != topCard.isRed()) {
                                            isValidMove = true;
                                        }
                                    }
                                }
                                break;
                            }
                        }
                    }

                    if (isValidMove) {
                        if (targetFoundationIndex != -1) {
                            // Mover la carta a la Foundation pile
                            foundationPiles[targetFoundationIndex].addCard(*draggedCard);
                        } else if (targetTableauIndex != -1) {
                            // Mover la carta al Tableau pile
                            tableauPiles[targetTableauIndex].addCard(*draggedCard);
                        }
                        
                        // Remover la carta de su origen
                        if (draggedPileIndex == -2) {
                            wastePile.cards.pop_back();
                        } else {
                            tableauPiles[draggedPileIndex].cards.pop_back();
                            // Voltear la siguiente carta si existe
                            if (!tableauPiles[draggedPileIndex].cards.empty()) {
                                Card& nextCard = tableauPiles[draggedPileIndex].cards.back();
                                if (!nextCard.faceUp()) {
                                    nextCard.flip();
                                }
                            }
                        }
                    } else {
                        // Verificar si hubo un movimiento significativo (más de 50 píxeles)
                        sf::Vector2f currentPos = draggedCard->getPosition();
                        float distance = std::sqrt(std::pow(currentPos.x - originalPosition.x, 2) + 
                                                   std::pow(currentPos.y - originalPosition.y, 2));
                        
                        if (distance > 50.f) {
                            // Solo reproducir sonido si realmente intentó mover la carta
                            errorSound.play();
                        }
                        
                        if (draggedPileIndex == -2) {
                            // Carta del waste pile
                            wastePile.cards[draggedCardIndex].setPosition(originalPosition.x, originalPosition.y);
                            wastePile.updatePositions();
                        } else {
                            // Carta del tableau
                            tableauPiles[draggedPileIndex].cards[draggedCardIndex].setPosition(originalPosition.x, originalPosition.y);
                            tableauPiles[draggedPileIndex].updatePositions();
                        }
                    }

                    isDragging = false;
                    draggedPileIndex = -1;
                    draggedCardIndex = -1;
                }
            }

            // Mouse moviéndose - arrastrar carta
            if (event.type == sf::Event::MouseMoved && isDragging) {
                sf::Vector2f mousePos(event.mouseMove.x, event.mouseMove.y);
                
                if (draggedPileIndex == -2) {
                    // Arrastrando carta del waste
                    wastePile.cards[draggedCardIndex].setPosition(mousePos.x - dragOffset.x, mousePos.y - dragOffset.y);
                } else {
                    // Arrastrando carta del tableau
                    tableauPiles[draggedPileIndex].cards[draggedCardIndex].setPosition(mousePos.x - dragOffset.x, mousePos.y - dragOffset.y);
                }
            }
        }

        window.clear(sf::Color(0, 100, 0)); // Verde oscuro
        
        // Verificar condición de victoria
        if (!gameWon) {
            bool allFoundationsFull = true;
            for (const auto& pile : foundationPiles) {
                if (pile.cards.size() != 13) {
                    allFoundationsFull = false;
                    break;
                }
            }
            if (allFoundationsFull) {
                gameWon = true;
                winSound.play();
            }
        }

        // Dibujar fondo para las posiciones del mazo y fundaciones
        sf::RectangleShape cardOutline(sf::Vector2f(142.f, 198.f));
        cardOutline.setFillColor(sf::Color(0, 80, 0));
        cardOutline.setOutlineColor(sf::Color::White);
        cardOutline.setOutlineThickness(2.f);
        
        // Dibujar mazo (stock) - si hay cartas, mostrar la de arriba boca abajo
        if (!stockPile.empty()) {
            stockPile.back()->draw(window);
        } else {
            // Si no hay cartas, mostrar espacio vacío
            cardOutline.setPosition(50.f, 30.f);
            window.draw(cardOutline);
        }
        
        // Espacio para waste pile (vacío si no hay cartas)
        if (wastePile.cards.empty()) {
            cardOutline.setPosition(200.f, 30.f);
            window.draw(cardOutline);
        }
        
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
