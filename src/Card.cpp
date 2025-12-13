#include "Card.hpp"
#include <iostream>

// Calcula el rectángulo de textura basado en Suit y Rank
// Asume un sprite sheet con 13 columnas (ACE..KING) y 4 filas (HEARTS, SPADES, DIAMONDS, CLUBS)
sf::IntRect Card::getTextureRect(Suit s, Rank r) const {
    // Detectar tamaño de celda dinamicamente a partir de la textura
    const sf::Vector2u texSize = cardTexture->getSize();
    const int cols = 13;
    const int rows = 4;
    const int cellW = (int)(texSize.x / cols);
    const int cellH = (int)(texSize.y / rows);

    int col = (int)r - 1; // 0..12
    int row = (int)s;     // 0..3
    int x = col * cellW;
    int y = row * cellH;
    return sf::IntRect(x, y, cellW, cellH);
}

// Nuevo constructor: usa sprite sheet
Card::Card(Suit s, Rank r, sf::Texture& spriteSheet, sf::Texture& backTex, float x, float y) 
    : suit(s), rank(r), isFaceUp(false), cardTexture(&spriteSheet), backTexture(&backTex) {
    
    sprite.setTexture(backTex); // Empieza mostrando el reverso completo
    sprite.setPosition(x, y);
}

void Card::flip() {
    if (isFaceUp) {
        // Mostrar reverso sin recorte
        sprite.setTexture(*backTexture, true);
        sprite.setTextureRect(sf::IntRect());
    } else {
        // Mostrar frente usando rect calculado
        sprite.setTexture(*cardTexture, true);
        sprite.setTextureRect(getTextureRect(suit, rank));
    }
    isFaceUp = !isFaceUp;
}

void Card::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

void Card::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

sf::Vector2f Card::getPosition() const {
    return sprite.getPosition();
}

bool Card::faceUp() const {
    return isFaceUp;
}

