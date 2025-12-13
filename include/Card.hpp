#ifndef CARD_HPP
#define CARD_HPP

#include <SFML/Graphics.hpp>
#include <string>

// Enum para el PALO (Suit)
enum class Suit { HEARTS = 0, SPADES, DIAMONDS, CLUBS };

// Enum para el VALOR (Rank)
enum class Rank { ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING };

class Card {
private:
    sf::Sprite sprite;
    const sf::Texture* cardTexture;      // Puntero a textura del sprite sheet
    const sf::Texture* backTexture;      // Puntero a textura del reverso
    bool isFaceUp;
    Suit suit;
    Rank rank;

    // Calcula el rectángulo de la textura para esta carta específica
    sf::IntRect getTextureRect(Suit s, Rank r) const;

public:
    // Constructor: recibe Suit y Rank
    Card(Suit s, Rank r, sf::Texture& spriteSheet, sf::Texture& backTex, float x = 0.f, float y = 0.f);

    // Voltear carta
    void flip();

    // Dibujar la carta en la ventana
    void draw(sf::RenderWindow& window);

    // Establecer posición
    void setPosition(float x, float y);

    // Obtener posición
    sf::Vector2f getPosition() const;

    // Bounds globales para hit-testing
    sf::FloatRect getGlobalBounds() const { return sprite.getGlobalBounds(); }

    // Saber si está volteada
    bool faceUp() const;

    void forceFaceDown();
    
    // Obtener información de la carta
    Suit getSuit() const { return suit; }
    Rank getRank() const { return rank; }
    
    // Verificar color de la carta
    bool isRed() const { return suit == Suit::HEARTS || suit == Suit::DIAMONDS; }
    bool isBlack() const { return suit == Suit::SPADES || suit == Suit::CLUBS; }
};

#endif
