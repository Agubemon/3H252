#include "Card.hpp"
#include <iostream>

Card::Card(const std::string& frontPath, const std::string& backPath, float x, float y) {
    if (!frontTexture.loadFromFile(frontPath)) {
        std::cerr << "Error al cargar la textura frontal: " << frontPath << std::endl;
    }
    if (!backTexture.loadFromFile(backPath)) {
        std::cerr << "Error al cargar la textura trasera: " << backPath << std::endl;
    }

    sprite.setTexture(backTexture); // empieza mostrando el reverso
    sprite.setPosition(x, y);
    isFaceUp = false;
}

void Card::flip() {
    if (isFaceUp)
        sprite.setTexture(backTexture);
    else
        sprite.setTexture(frontTexture);

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

