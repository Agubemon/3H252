#include "Card.hpp"
#include <iostream>

// Calcula el rectángulo de textura basado en Suit y Rank
sf::IntRect Card::getTextureRect(Suit s, Rank r) const {
    int x = ((int)r - 1) * 142;  // rank va de 1 a 13
    int y = (int)s * 198;         // suit va de 0 a 3
    return sf::IntRect(x, y, 142, 198);
}

// Nuevo constructor: usa sprite sheet
Card::Card(Suit s, Rank r, sf::Texture& spriteSheet, sf::Texture& backTex, float x, float y) 
    : suit(s), rank(r), isFaceUp(false) {
    
    sprite.setTexture(backTex); // Empieza mostrando el reverso
    sprite.setPosition(x, y);
    
    // Guardamos referencias a las texturas (ya están cargadas externamente)
    cardTexture = spriteSheet;
    backTexture = backTex;
}

// Constructor anterior (compatibilidad con código viejo)
Card::Card(const std::string& frontPath, const std::string& backPath, float x, float y) 
    : suit(Suit::CLUBS), rank(Rank::ACE), isFaceUp(false) {
    
    if (!cardTexture.loadFromFile(frontPath)) {
        std::cerr << "Error al cargar la textura frontal: " << frontPath << std::endl;
    }
    if (!backTexture.loadFromFile(backPath)) {
        std::cerr << "Error al cargar la textura trasera: " << backPath << std::endl;
    }

    sprite.setTexture(backTexture);
    sprite.setPosition(x, y);
}

void Card::flip() {
    if (isFaceUp) {
        sprite.setTexture(backTexture);
    } else {
        sprite.setTexture(cardTexture);
        sprite.setTextureRect(getTextureRect(suit, rank)); // Selecciona la carta correcta
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

