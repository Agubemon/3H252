#ifndef CARD_HPP
#define CARD_HPP

#include <SFML/Graphics.hpp>
#include <string>

class Card {
private:
    sf::Sprite sprite;
    sf::Texture frontTexture;
    sf::Texture backTexture;
    bool isFaceUp;

public:
    // Constructor: recibe ruta de las texturas
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
};

#endif
