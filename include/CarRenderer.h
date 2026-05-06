#pragma once
#include <SFML/Graphics.hpp>
#include "Car.h"
#include <unordered_map>
#include <string>

// SRP: CarRenderer odpowiada WYŁĄCZNIE za renderowanie danych pojazdu.
// Nie zna logiki biznesowej. Oddziela widok od modelu.
class CarRenderer {
public:
    explicit CarRenderer(sf::Font& font);

    // Rysuje kartę pojazdu (miniaturka + podstawowe dane) w podanym prostokącie
    void drawCard(sf::RenderWindow& window, const Car& car,
                  const sf::FloatRect& bounds, bool selected = false);

    // Rysuje pełny widok szczegółowy
    void drawDetail(sf::RenderWindow& window, const Car& car,
                    const sf::FloatRect& bounds);

private:
    sf::Font&                                        m_font;
    std::unordered_map<std::string, sf::Texture>     m_textureCache;

    sf::Texture& getOrLoadTexture(const std::string& path);
    void         drawPlaceholder(sf::RenderWindow& window,
                                 const sf::FloatRect& rect);
};
