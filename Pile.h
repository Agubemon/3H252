#pragma once
#include "Card.h"
#include <vector>
#include <stdexcept>

class Pile {
protected:
    std::vector<Card> cards;

public:
    Pile() = default;

    // Obtiene una referencia a la carta superior (solo para lectura/voltear)
    // Se usa un índice para la lógica de apilamiento en el tablero.
    Card& getCard(size_t index);
    
    // Método para añadir una carta
    void addCard(const Card& card);

    // Método para remover la carta superior y devolverla
    Card removeTopCard();

    // Métodos de consulta
    bool isEmpty() const { return cards.empty(); }
    size_t size() const { return cards.size(); }
    
    // Virtual: Permite que las pilas específicas (Foundation, Tableau)
    // implementen sus propias reglas de movimiento.
    // Esta función revisa si es válido colocar una carta 'c' sobre esta pila.
    virtual bool canPlaceCard(const Card& c) const {
        // En la clase base, siempre devolvemos true (las clases hijas implementan la lógica)
        return true; 
    }
};