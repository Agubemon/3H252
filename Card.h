#pragma once
#include <string>

// Enum para el PALO (Suit): CLUBS, DIAMONDS, HEARTS, SPADES
enum class Suit { CLUBS = 0, DIAMONDS, HEARTS, SPADES };

// Enum para el VALOR (Rank): ACE=1, TWO... KING=13
enum class Rank { ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING };

class Card {
private:
    Suit suit;
    Rank rank;
    bool isFaceUp; // true si está boca arriba, false si está boca abajo

public:
    // Constructor de la carta
    Card(Suit s, Rank r) : suit(s), rank(r), isFaceUp(false) {}

    // Voltea la carta (cambia su estado)
    void flip() {
        isFaceUp = !isFaceUp;
    }

    // Funciones para obtener información
    Suit getSuit() const { return suit; }
    Rank getRank() const { return rank; }
    bool isFaceUpState() const { return isFaceUp; }
    
    // Devuelve true si la carta es ROJA (DIAMONDS o HEARTS)
    bool isRed() const {
        return suit == Suit::DIAMONDS || suit == Suit::HEARTS;
    }
    
    // Devuelve true si la carta es NEGRA (CLUBS o SPADES)
    bool isBlack() const {
        return suit == Suit::CLUBS || suit == Suit::SPADES;
    }
};