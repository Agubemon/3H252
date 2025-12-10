#include "GameManager.h"
#include <iostream>

// Constructor: Crea las pilas y baraja el mazo.
GameManager::GameManager() {
    // 1. Crear las 7 pilas del Tableau y las 4 de Foundation
    tableauPiles.resize(7);
    foundationPiles.resize(4);

    // 2. Barajar el mazo (Deck)
    deck.shuffle();
}

// ==========================================================
// LÓGICA DE LAS PILAS ESPECÍFICAS
// ==========================================================

// Regla para Tableau (Pila del Tablero)
bool TableauPile::canPlaceCard(const Card& c) const {
    if (cards.empty()) {
        // En una pila vacía del Tableau, SÓLO se puede colocar un Rey (KING).
        return c.getRank() == Rank::KING;
    }
    
    const Card& topCard = cards.back(); // La carta que está arriba
    
    // Regla 1: Debe ser de rango menor a la carta superior (Descendente)
    if ((int)c.getRank() != (int)topCard.getRank() - 1) {
        return false;
    }
    
    // Regla 2: Debe ser de color diferente (Alternando colores)
    if (c.isBlack() == topCard.isBlack()) {
        return false;
    }

    return true; // Si cumple ambas, es válido.
}

// Regla para Foundation (Pila de Fundación)
bool FoundationPile::canPlaceCard(const Card& c) const {
    if (cards.empty()) {
        // En una pila de Fundación vacía, SÓLO se puede colocar un As (ACE).
        return c.getRank() == Rank::ACE;
    }
    
    const Card& topCard = cards.back(); // La carta que está arriba

    // Regla 1: Debe ser del mismo palo (Suit)
    if (c.getSuit() != topCard.getSuit()) {
        return false;
    }
    
    // Regla 2: Debe ser de rango mayor por uno (Ascendente)
    if ((int)c.getRank() != (int)topCard.getRank() + 1) {
        return false;
    }

    return true; // Si cumple ambas, es válido.
}

// ==========================================================
// INICIALIZACIÓN DEL JUEGO
// ==========================================================

void GameManager::initializeBoard() {
    // Repartir las 28 cartas en las 7 pilas del Tableau
    for (int i = 0; i < 7; ++i) { // i es el índice de la pila (0 a 6)
        for (int j = 0; j <= i; ++j) { // j es el número de carta en la pila
            
            Card newCard = deck.dealCard();
            
            if (j == i) {
                // La última carta de cada pila (j == i) debe estar boca arriba
                newCard.flip(); 
            }
            
            tableauPiles[i].addCard(newCard);
        }
    }
    // El resto de la baraja queda en 'deck' para ser volteada al mazo de descarte.
    std::cout << "Tablero inicializado. Cartas restantes en el mazo: " << deck.isEmpty() << std::endl;
}

// Lógica de Ganar (simple por ahora)
bool GameManager::checkWinCondition() const {
    // Si las 4 pilas de fundación tienen 13 cartas (del As al Rey), ganas.
    for (const auto& pile : foundationPiles) {
        if (pile.size() != 13) {
            return false;
        }
    }
    return true;
}

// Nota: La lógica completa de movimiento de cartas (moveCard) es muy compleja y la dejaremos para después de tener la configuración 3D.