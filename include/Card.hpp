#ifndef CARD_HPP
#define CARD_HPP

#include <SFML/Graphics.hpp>
#include <string>

// Enum para el PALO (Suit)
enum class Suit { CLUBS = 0, DIAMONDS, HEARTS, SPADES };

// Enum para el VALOR (Rank)
enum class Rank { ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING };

class Card {
private:
    sf::Sprite sprite;
    sf::Texture cardTexture;      // Textura del sprite sheet
    sf::Texture backTexture;      // Textura del reverso
    bool isFaceUp;
    Suit suit;
    Rank rank;

    // Calcula el rectángulo de la textura para esta carta específica
    sf::IntRect getTextureRect(Suit s, Rank r) const;

public:
    // Constructor: recibe Suit y Rank
    Card(Suit s, Rank r, sf::Texture& spriteSheet, sf::Texture& backTex, float x = 0.f, float y = 0.f);
    
    // Constructor anterior (mantener compatibilidad)
    Card(const std::string& frontPath, const std::string& backPath, float x = 0.f, float y = 0.f);

    // Voltear carta
    void flip();

    // Dibujar la carta en la ventana
    void draw(sf::RenderWindow& window);

    // Establecer posición
    void setPosition(float x, float y);

    // Obtener posición
    sf::Vector2f getPosition() const;

    // Saber si está volteada
    bool faceUp() const;
    
    // Obtener información de la carta
    Suit getSuit() const { return suit; }
    Rank getRank() const { return rank; }
};

#endif
