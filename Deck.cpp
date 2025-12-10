#include "Deck.h"
#include <stdexcept>

// Constructor: Inicializa la baraja con 52 cartas
Deck::Deck() {
    // Itera sobre los 4 palos
    for (int s = (int)Suit::CLUBS; s <= (int)Suit::SPADES; ++s) {
        // Itera sobre los 13 valores
        for (int r = (int)Rank::ACE; r <= (int)Rank::KING; ++r) {
            cards.emplace_back((Suit)s, (Rank)r);
        }
    }
}

// Función para barajar
void Deck::shuffle() {
    std::random_device rd;
    std::mt19937 g(rd()); // Motor de generación aleatoria
    std::shuffle(cards.begin(), cards.end(), g);
}

// Función para repartir una carta
Card Deck::dealCard() {
    if (cards.empty()) {
        throw std::runtime_error("Error: El mazo está vacío y no se puede repartir.");
    }
    // Toma la carta superior
    Card topCard = cards.back();
    // La remueve del mazo
    cards.pop_back();
    return topCard;
}
