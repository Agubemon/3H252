#pragma once
#include "Card.hpp"
#include <vector>
#include <algorithm> // Para std::shuffle
#include <random>    // Para el motor de números aleatorios

class Deck {
private:
    std::vector<Card> cards;

public:
    Deck(); // Constructor que inicializa las 52 cartas
    
    void shuffle(); // Baraja las cartas
    
    Card dealCard(); // Reparte y remueve la carta superior
    
    bool isEmpty() const { return cards.empty(); }
};