#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Card.hpp"
#include <vector>
#include <random>
#include <algorithm>

struct VisualPile {
    std::vector<Card> cards;
    sf::Vector2f position;
    float offsetY = 30.f;
    bool showOnlyTop = false;

    VisualPile(float x, float y, bool onlyTop = false) : position(x, y), showOnlyTop(onlyTop) {}

    void addCard(Card card) {
        cards.push_back(card);
        updatePositions();
    }

    void updatePositions() {
        for (size_t i = 0; i < cards.size(); ++i) {
            cards[i].setPosition(position.x, position.y + i * offsetY);
        }
    }

    void draw(sf::RenderWindow& window) {
        if (showOnlyTop && !cards.empty()) {
            cards.back().draw(window);
        } else {
            for (auto& card : cards) {
                card.draw(window);
            }
        }
    }
};

bool canPlaceOnFoundation(const Card& card, const VisualPile& foundation) {
    if (foundation.cards.empty()) {
        return card.getRank() == Rank::ACE;
    }
    const Card& top = foundation.cards.back();
    return card.getSuit() == top.getSuit() && static_cast<int>(card.getRank()) == static_cast<int>(top.getRank()) + 1;
}

bool canPlaceOnTableau(const Card& card, const VisualPile& pile) {
    if (pile.cards.empty()) {
        return card.getRank() == Rank::KING;
    }
    const Card& top = pile.cards.back();
    if (!top.faceUp()) return false;
    return static_cast<int>(card.getRank()) == static_cast<int>(top.getRank()) - 1 && card.isRed() != top.isRed();
}

bool isValidDescendingAlt(const std::vector<Card>& stack) {
    if (stack.empty()) return false;
    for (size_t i = 0; i + 1 < stack.size(); ++i) {
        const Card& lower = stack[i];
        const Card& upper = stack[i + 1];
        if (!lower.faceUp() || !upper.faceUp()) return false;
        if (static_cast<int>(upper.getRank()) != static_cast<int>(lower.getRank()) - 1) return false;
        if (upper.isRed() == lower.isRed()) return false;
    }
    return true;
}

std::vector<Card> createDeck(sf::Texture& spriteSheet, sf::Texture& backTexture) {
    std::vector<Card> deck;
    deck.reserve(52);
    for (int s = 0; s < 4; ++s) {
        for (int r = 1; r <= 13; ++r) {
            deck.emplace_back(static_cast<Suit>(s), static_cast<Rank>(r), spriteSheet, backTexture);
        }
    }
    return deck;
}

void flipTopIfNeeded(VisualPile& pile) {
    if (!pile.cards.empty() && !pile.cards.back().faceUp()) {
        pile.cards.back().flip();
    }
}

int main() {
    const float VIRTUAL_W = 1150.f;
    const float VIRTUAL_H = 700.f;
    sf::RenderWindow window(sf::VideoMode(static_cast<unsigned>(VIRTUAL_W), static_cast<unsigned>(VIRTUAL_H)), "Solitario Navide\u00f1o");
    sf::View view(sf::FloatRect(0.f, 0.f, VIRTUAL_W, VIRTUAL_H));
    window.setView(view);

    enum class GameState { MENU, PLAYING };
    GameState currentState = GameState::MENU;

    sf::Texture menuTexture;
    menuTexture.loadFromFile("assets/textures/Pantalladeinicio.jpg");
    sf::Sprite menuSprite(menuTexture);
    menuSprite.setScale(1150.f / menuTexture.getSize().x, 700.f / menuTexture.getSize().y);

    sf::Texture speakerOnTexture;
    speakerOnTexture.loadFromFile("assets/textures/ImagenBocina.png");
    sf::Texture speakerOffTexture;
    speakerOffTexture.loadFromFile("assets/textures/ImagenBocinaPausa.png");

    sf::Font font;
    font.loadFromFile("assets/font/MarysonRough_PERSONAL_USE_ONLY.otf");

    sf::RectangleShape startButton(sf::Vector2f(300.f, 80.f));
    startButton.setPosition(105.f, 570.f);
    startButton.setFillColor(sf::Color::Transparent);
    sf::Text startText("START", font, 50);
    startText.setPosition(160.f, 585.f);
    startText.setFillColor(sf::Color::White);
    startText.setStyle(sf::Text::Bold);

    sf::RectangleShape closeButton(sf::Vector2f(300.f, 30.f));
    closeButton.setPosition(0.f, 620.f);
    closeButton.setFillColor(sf::Color::Transparent);
    sf::Text closeText("CLOSE", font, 25);
    closeText.setPosition(55.f, 622.f);
    closeText.setFillColor(sf::Color::White);
    closeText.setStyle(sf::Text::Bold);

    sf::RectangleShape backButton(sf::Vector2f(300.f, 30.f));
    backButton.setPosition(0.f, 589.f);
    backButton.setFillColor(sf::Color::Transparent);
    sf::Text backText("BACK", font, 25);
    backText.setPosition(55.f, 591.f);
    backText.setFillColor(sf::Color::White);
    backText.setStyle(sf::Text::Bold);

    sf::RectangleShape pauseButton(sf::Vector2f(30.f, 30.f));
    pauseButton.setPosition(0.f, 558.f);
    pauseButton.setFillColor(sf::Color::Transparent);

    sf::Sprite speakerOnSprite(speakerOnTexture);
    speakerOnSprite.setPosition(5.f, 560.f);
    float scaleOn = 25.f / speakerOnTexture.getSize().x;
    speakerOnSprite.setScale(scaleOn, scaleOn);
    sf::Sprite speakerOffSprite(speakerOffTexture);
    speakerOffSprite.setPosition(5.f, 560.f);
    float scaleOff = 25.f / speakerOffTexture.getSize().x;
    speakerOffSprite.setScale(scaleOff, scaleOff);

    sf::Texture spriteSheet;
    if (!spriteSheet.loadFromFile("assets/textures/sprite.png")) return -1;
    sf::Texture backTexture;
    if (!backTexture.loadFromFile("assets/textures/back.jpg")) return -1;
    spriteSheet.setSmooth(false);
    backTexture.setSmooth(false);

    sf::SoundBuffer errorBuffer;
    errorBuffer.loadFromFile("assets/Sounds/AudioError.wav");
    sf::Sound errorSound(errorBuffer);
    sf::SoundBuffer winBuffer;
    winBuffer.loadFromFile("assets/Sounds/AudioGanar.wav");
    sf::Sound winSound(winBuffer);

    sf::Music menuMusic;
    menuMusic.openFromFile("assets/Sounds/AudioPantalladeInicio.wav");
    menuMusic.setLoop(true);
    menuMusic.play();
    bool musicPaused = false;
    sf::Music gameMusic;
    gameMusic.openFromFile("assets/Sounds/AudioPantalladeIjuego.wav");
    gameMusic.setLoop(true);

    std::vector<Card> deck = createDeck(spriteSheet, backTexture);
    std::random_device rd;
    std::mt19937 rng(rd());
    std::shuffle(deck.begin(), deck.end(), rng);

    const float cardW = 142.f;
    const float cardH = 198.f;
    const float tableauStartX = 50.f;
    const float tableauStartY = 250.f;
    const float tableauSpacing = 150.f;
    const float foundationStartX = 500.f;
    const float foundationSpacing = 150.f;
    const float foundationY = 30.f;
    const sf::Vector2f stockPos(50.f, 30.f);
    const sf::Vector2f wastePos(200.f, 30.f);

    std::vector<VisualPile> tableauPiles;
    for (int i = 0; i < 7; ++i) {
        tableauPiles.emplace_back(tableauStartX + i * tableauSpacing, tableauStartY);
    }

    std::vector<VisualPile> foundationPiles;
    for (int i = 0; i < 4; ++i) {
        foundationPiles.emplace_back(foundationStartX + i * foundationSpacing, foundationY, true);
        foundationPiles.back().offsetY = 0.f;
    }

    VisualPile wastePile(wastePos.x, wastePos.y, true);
    wastePile.offsetY = 0.f;

    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j <= i; ++j) {
            Card c = deck.back();
            deck.pop_back();
            tableauPiles[i].cards.push_back(c);
        }
        tableauPiles[i].updatePositions();
        tableauPiles[i].cards.back().flip();
    }

    std::vector<Card> stockPile;
    while (!deck.empty()) {
        Card c = deck.back();
        deck.pop_back();
        c.setPosition(stockPos.x, stockPos.y);
        stockPile.push_back(c);
    }

    bool gameWon = false;
    bool gameLost = false;

    bool isDragging = false;
    int draggedPileIndex = -1; // -2 waste, 0..6 tableau
    int draggedCardIndex = -1;
    std::vector<Card> draggedStack;
    sf::Vector2f dragOffset;

    sf::Clock doubleClock;
    sf::Time lastClickTime = sf::Time::Zero;
    const sf::Time doubleClickThreshold = sf::milliseconds(300);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Resized) {
                window.setView(view);
            }
            if (event.type == sf::Event::Closed && currentState == GameState::MENU) {
                window.close();
            }

            if (currentState == GameState::MENU) {
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                    if (pauseButton.getGlobalBounds().contains(mousePos)) {
                        if (musicPaused) {
                            menuMusic.play();
                        } else {
                            menuMusic.pause();
                        }
                        musicPaused = !musicPaused;
                        continue;
                    }
                    if (startButton.getGlobalBounds().contains(mousePos)) {
                        menuMusic.stop();
                        gameMusic.play();
                        currentState = GameState::PLAYING;
                    }
                }
            } else if (currentState == GameState::PLAYING) {
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

                    sf::FloatRect closeBounds(0.f, 620.f, 120.f, 30.f);
                    if (closeBounds.contains(mousePos)) {
                        window.close();
                        continue;
                    }

                    sf::FloatRect backBounds(0.f, 589.f, 120.f, 30.f);
                    if (backBounds.contains(mousePos)) {
                        gameMusic.stop();
                        menuMusic.play();
                        currentState = GameState::MENU;
                        continue;
                    }

                    if (pauseButton.getGlobalBounds().contains(mousePos)) {
                        if (gameMusic.getStatus() == sf::Music::Playing) {
                            gameMusic.pause();
                        } else {
                            gameMusic.play();
                        }
                        continue;
                    }

                    sf::Time currentTime = doubleClock.getElapsedTime();
                    bool isDoubleClick = (currentTime - lastClickTime) < doubleClickThreshold;
                    lastClickTime = currentTime;

                    sf::FloatRect stockBounds(stockPos.x, stockPos.y, cardW, cardH);
                    if (stockBounds.contains(mousePos)) {
                        if (!stockPile.empty()) {
                            Card top = stockPile.back();
                            stockPile.pop_back();
                            top.flip();
                            top.setPosition(wastePos.x, wastePos.y);
                            wastePile.addCard(top);
                        } else if (!wastePile.cards.empty()) {
                            std::vector<Card> temp;
                            for (int i = static_cast<int>(wastePile.cards.size()) - 1; i >= 0; --i) {
                                Card c = wastePile.cards[i];
                                c.forceFaceDown();
                                c.setPosition(stockPos.x, stockPos.y);
                                temp.push_back(c);
                            }
                            std::shuffle(temp.begin(), temp.end(), rng); // barajar al reciclar
                            stockPile = temp;
                            wastePile.cards.clear();
                        } else {
                            gameLost = true;
                        }
                        continue;
                    }

                    auto tryAutoMoveToFoundation = [&](Card* card, int sourcePileIndex) {
                        for (int i = 0; i < 4; ++i) {
                            if (canPlaceOnFoundation(*card, foundationPiles[i])) {
                                foundationPiles[i].cards.push_back(*card);
                                foundationPiles[i].updatePositions();
                                if (sourcePileIndex == -2) {
                                    wastePile.cards.pop_back();
                                    wastePile.updatePositions();
                                } else {
                                    tableauPiles[sourcePileIndex].cards.pop_back();
                                    flipTopIfNeeded(tableauPiles[sourcePileIndex]);
                                    tableauPiles[sourcePileIndex].updatePositions();
                                }
                                return true;
                            }
                        }
                        return false;
                    };

                    if (isDoubleClick && !wastePile.cards.empty()) {
                        if (wastePile.cards.back().getGlobalBounds().contains(mousePos)) {
                            if (tryAutoMoveToFoundation(&wastePile.cards.back(), -2)) continue;
                        }
                    }

                    if (isDoubleClick) {
                        bool handled = false;
                        for (int i = 0; i < static_cast<int>(tableauPiles.size()); ++i) {
                            if (!tableauPiles[i].cards.empty()) {
                                Card& top = tableauPiles[i].cards.back();
                                if (top.faceUp() && top.getGlobalBounds().contains(mousePos)) {
                                    if (tryAutoMoveToFoundation(&top, i)) {
                                        handled = true;
                                        break;
                                    }
                                }
                            }
                        }
                        if (handled) continue;
                    }

                    if (!wastePile.cards.empty() && wastePile.cards.back().getGlobalBounds().contains(mousePos)) {
                        isDragging = true;
                        draggedPileIndex = -2;
                        draggedCardIndex = static_cast<int>(wastePile.cards.size()) - 1;
                        sf::Vector2f pos = wastePile.cards.back().getPosition();
                        dragOffset = mousePos - pos;
                        draggedStack.clear();
                        draggedStack.push_back(wastePile.cards.back());
                        wastePile.cards.pop_back();
                        sf::Vector2f basePos = mousePos - dragOffset;
                        for (size_t idx = 0; idx < draggedStack.size(); ++idx) {
                            draggedStack[idx].setPosition(basePos.x, basePos.y + static_cast<float>(idx) * 30.f);
                        }
                        continue;
                    }

                    for (int i = 0; i < static_cast<int>(tableauPiles.size()); ++i) {
                        for (int j = static_cast<int>(tableauPiles[i].cards.size()) - 1; j >= 0; --j) {
                            if (!tableauPiles[i].cards[j].faceUp()) continue;

                            sf::Vector2f pos = tableauPiles[i].cards[j].getPosition();
                            float h = (j == static_cast<int>(tableauPiles[i].cards.size()) - 1) ? cardH : tableauPiles[i].offsetY;
                            sf::FloatRect clickArea(pos.x, pos.y, cardW, h);

                            if (clickArea.contains(mousePos)) {
                                isDragging = true;
                                draggedPileIndex = i;
                                draggedCardIndex = j;
                                dragOffset = mousePos - pos;
                                draggedStack.clear();
                                for (int k = j; k < static_cast<int>(tableauPiles[i].cards.size()); ++k) {
                                    draggedStack.push_back(tableauPiles[i].cards[k]);
                                }
                                tableauPiles[i].cards.erase(tableauPiles[i].cards.begin() + j, tableauPiles[i].cards.end());
                                tableauPiles[i].updatePositions();
                                sf::Vector2f basePos = mousePos - dragOffset;
                                for (size_t idx = 0; idx < draggedStack.size(); ++idx) {
                                    draggedStack[idx].setPosition(basePos.x, basePos.y + static_cast<float>(idx) * 30.f);
                                }
                                break;
                            }
                        }
                        if (isDragging) break;
                    }
                }

                if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left && isDragging) {
                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                    bool placed = false;
                    bool stackValid = draggedStack.size() == 1 || isValidDescendingAlt(draggedStack);

                    if (draggedStack.size() == 1) {
                        for (int i = 0; i < 4 && !placed; ++i) {
                            sf::FloatRect bounds(foundationStartX + i * foundationSpacing, foundationY, cardW, cardH);
                            if (bounds.contains(mousePos) && canPlaceOnFoundation(draggedStack[0], foundationPiles[i])) {
                                foundationPiles[i].cards.push_back(draggedStack[0]);
                                foundationPiles[i].updatePositions();
                                placed = true;
                            }
                        }
                    }

                    if (!placed) {
                        for (int i = 0; i < static_cast<int>(tableauPiles.size()) && !placed; ++i) {
                            sf::FloatRect bounds(tableauStartX + i * tableauSpacing, tableauStartY, cardW, VIRTUAL_H - tableauStartY);
                            if (bounds.contains(mousePos) && stackValid) {
                                if (canPlaceOnTableau(draggedStack.front(), tableauPiles[i])) {
                                    auto& dest = tableauPiles[i].cards;
                                    dest.insert(dest.end(), draggedStack.begin(), draggedStack.end());
                                    tableauPiles[i].updatePositions();
                                    placed = true;
                                }
                            }
                        }
                    }

                    if (placed) {
                        if (draggedPileIndex >= 0 && draggedPileIndex < static_cast<int>(tableauPiles.size())) {
                            flipTopIfNeeded(tableauPiles[draggedPileIndex]);
                            tableauPiles[draggedPileIndex].updatePositions();
                        }
                    } else {
                        if (draggedPileIndex == -2) {
                            for (auto& c : draggedStack) {
                                c.setPosition(wastePos.x, wastePos.y);
                                wastePile.cards.push_back(c);
                            }
                            wastePile.updatePositions();
                        } else if (draggedPileIndex >= 0) {
                            auto& pile = tableauPiles[draggedPileIndex].cards;
                            pile.insert(pile.begin() + draggedCardIndex, draggedStack.begin(), draggedStack.end());
                            tableauPiles[draggedPileIndex].updatePositions();
                        }
                    }

                    draggedStack.clear();
                    draggedPileIndex = -1;
                    draggedCardIndex = -1;
                    isDragging = false;
                }

                if (event.type == sf::Event::MouseMoved && isDragging) {
                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
                    sf::Vector2f basePos = mousePos - dragOffset;
                    for (size_t idx = 0; idx < draggedStack.size(); ++idx) {
                        draggedStack[idx].setPosition(basePos.x, basePos.y + static_cast<float>(idx) * 30.f);
                    }
                }
            }
        }

        if (currentState == GameState::MENU) {
            window.clear();
            window.draw(menuSprite);
            window.draw(startText);
            if (menuMusic.getStatus() == sf::Music::Playing) {
                window.draw(speakerOnSprite);
            } else {
                window.draw(speakerOffSprite);
            }
            window.display();
            continue;
        }

        window.clear(sf::Color(0, 100, 0));

        if (!gameWon) {
            bool allFull = true;
            for (const auto& pile : foundationPiles) {
                if (pile.cards.size() != 13) {
                    allFull = false;
                    break;
                }
            }
            if (allFull) {
                gameWon = true;
                winSound.play();
            }
        }

        sf::RectangleShape cardOutline(sf::Vector2f(cardW, cardH));
        cardOutline.setFillColor(sf::Color(0, 80, 0));
        cardOutline.setOutlineColor(sf::Color::White);
        cardOutline.setOutlineThickness(2.f);

        if (!stockPile.empty()) {
            stockPile.back().draw(window);
        } else if (!wastePile.cards.empty()) {
            cardOutline.setPosition(stockPos);
            cardOutline.setFillColor(sf::Color(20, 100, 20));
            window.draw(cardOutline);
            sf::Text recycle;
            recycle.setFont(font);
            recycle.setString("\u267b");
            recycle.setCharacterSize(60);
            recycle.setFillColor(sf::Color::White);
            sf::FloatRect tb = recycle.getLocalBounds();
            recycle.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
            recycle.setPosition(stockPos.x + cardW / 2.f, stockPos.y + cardH / 2.f);
            window.draw(recycle);
            cardOutline.setFillColor(sf::Color(0, 80, 0));
        } else {
            cardOutline.setPosition(stockPos);
            window.draw(cardOutline);
        }

        if (wastePile.cards.empty()) {
            cardOutline.setPosition(wastePos);
            window.draw(cardOutline);
        }

        wastePile.draw(window);

        for (int i = 0; i < 4; ++i) {
            cardOutline.setPosition(foundationStartX + i * foundationSpacing, foundationY);
            window.draw(cardOutline);
            foundationPiles[i].draw(window);
        }

        for (auto& pile : tableauPiles) {
            pile.draw(window);
        }

        window.draw(closeButton);
        window.draw(closeText);
        window.draw(backButton);
        window.draw(backText);
        if (gameMusic.getStatus() == sf::Music::Playing) {
            window.draw(speakerOnSprite);
        } else {
            window.draw(speakerOffSprite);
        }

        if (isDragging) {
            for (auto& c : draggedStack) {
                c.draw(window);
            }
        }

        sf::Text resultText;
        resultText.setFont(font);
        resultText.setCharacterSize(80);
        resultText.setStyle(sf::Text::Bold);
        sf::FloatRect tb = resultText.getLocalBounds();

        if (gameWon) {
            resultText.setString("\u00a1GANASTE!");
            resultText.setFillColor(sf::Color::Yellow);
            tb = resultText.getLocalBounds();
            resultText.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
            resultText.setPosition(VIRTUAL_W / 2.f, VIRTUAL_H / 2.f);
            window.draw(resultText);
        } else if (gameLost) {
            resultText.setString("\u00a1PERDISTE!");
            resultText.setFillColor(sf::Color::Red);
            tb = resultText.getLocalBounds();
            resultText.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
            resultText.setPosition(VIRTUAL_W / 2.f, VIRTUAL_H / 2.f);
            window.draw(resultText);
        }

        window.display();
    }

    return 0;
}
