#pragma once
#include "Deck.h"
#include "Pile.h"
#include <vector>

// Necesitamos pilas con reglas específicas, las definiremos dentro de GameManager.h
class TableauPile : public Pile {
public:
    // La lógica de Tableau: Orden descendente y colores alternados.
    // Ej: Un 7 rojo solo puede ir sobre un 8 negro.
    bool canPlaceCard(const Card& c) const override;
};

class FoundationPile : public Pile {
public:
    // La lógica de Foundation: Mismo palo, orden ascendente (As, 2, 3...).
    bool canPlaceCard(const Card& c) const override;
};

class GameManager {
private:
    Deck deck; // El mazo completo de 52 cartas
    std::vector<TableauPile> tableauPiles; // Las 7 pilas de la mesa (Tableau)
    std::vector<FoundationPile> foundationPiles; // Las 4 pilas de fundación (Foundation)
    Pile wastePile; // La pila de descarte (Waste)

public:
    GameManager();
    
    // Función CRUCIAL: Reparte las 28 cartas iniciales
    void initializeBoard();

    // Lógica para mover cartas (será la más compleja de implementar)
    // Ejemplo: moveCard(origen, destino, numCartas)
    // bool moveCard(int fromPileIndex, int toPileIndex, int numCards);
    
    // Otras funciones de juego
    void flipWasteCard(); // Voltear carta del mazo de descarte
    bool checkWinCondition() const; // Revisar si ganamos
};