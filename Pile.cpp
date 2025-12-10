#include "Pile.h"

// Obtiene una carta por su índice
Card& Pile::getCard(size_t index) {
    if (index >= cards.size()) {
        throw std::out_of_range("Índice de carta fuera de rango.");
    }
    return cards[index];
}

// Añade una carta al final de la pila
void Pile::addCard(const Card& card) {
    cards.push_back(card);
}

// Remueve la carta superior (la última) y la devuelve
Card Pile::removeTopCard() {
    if (cards.empty()) {
        throw std::runtime_error("La pila está vacía.");
    }
    Card topCard = cards.back();
    cards.pop_back();
    return topCard;
}
