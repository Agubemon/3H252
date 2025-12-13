#include "Card.hpp"
#include <iostream>
#include <cmath>

// Calcula el rectángulo de textura basado en Suit y Rank
// Asume un sprite sheet con 13 columnas (ACE..KING) y 4 filas (HEARTS, SPADES, DIAMONDS, CLUBS)
sf::IntRect Card::getTextureRect(Suit s, Rank r) const {
    // Calcular celdas con redondeo para evitar errores de división y bleeding
    sf::Vector2u texSize = cardTexture->getSize();
    const float cols = 13.f;
    const float rows = 4.f;
    float cellWf = texSize.x / cols;
    float cellHf = texSize.y / rows;

    int col = static_cast<int>(r) - 1; // 0..12
    int row = static_cast<int>(s);     // 0..3

    int x0 = static_cast<int>(std::round(col * cellWf));
    int x1 = static_cast<int>(std::round((col + 1) * cellWf));
    int y0 = static_cast<int>(std::round(row * cellHf));
    int y1 = static_cast<int>(std::round((row + 1) * cellHf));

    int w = x1 - x0;
    int h = y1 - y0;

    const int pad = 0; // sin recorte adicional
    // sin modificar w/h para conservar bordes completos
    return sf::IntRect(x0 + pad, y0 + pad, w, h);
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

void Card::forceFaceDown() {
    isFaceUp = false;
    sprite.setTexture(*backTexture, true);
    sprite.setTextureRect(sf::IntRect());
}

