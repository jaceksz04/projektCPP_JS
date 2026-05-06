#pragma once
#include <SFML/Graphics.hpp>
#include <string>

// Drobne narzędzia UI wielokrotnego użytku
namespace UI {

inline void drawButton(sf::RenderWindow& window, sf::Font& font,
                       const sf::FloatRect& rect, const std::string& label,
                       bool hovered = false, bool active = false) {
    sf::RectangleShape btn(sf::Vector2f(rect.width, rect.height));
    btn.setPosition(rect.left, rect.top);

    if (active)        btn.setFillColor(sf::Color(50, 120, 200));
    else if (hovered)  btn.setFillColor(sf::Color(60, 70, 100));
    else               btn.setFillColor(sf::Color(40, 45, 65));

    btn.setOutlineThickness(1.f);
    btn.setOutlineColor(active ? sf::Color(100, 180, 255) : sf::Color(80, 80, 110));
    window.draw(btn);

    sf::Text txt(label, font, 14);
    txt.setFillColor(sf::Color(220, 220, 240));
    sf::FloatRect tb = txt.getLocalBounds();
    txt.setPosition(rect.left + (rect.width - tb.width) * 0.5f - tb.left,
                    rect.top  + (rect.height - tb.height) * 0.5f - tb.top);
    window.draw(txt);
}

inline bool isHovered(const sf::FloatRect& rect, sf::RenderWindow& window) {
    auto mp = sf::Mouse::getPosition(window);
    return rect.contains(static_cast<float>(mp.x), static_cast<float>(mp.y));
}

// Prymitywne jednoliniowe pole tekstowe
struct InputField {
    sf::FloatRect bounds;
    std::string   label;
    std::string   value;
    bool          focused = false;

    void draw(sf::RenderWindow& window, sf::Font& font) const {
        sf::RectangleShape bg(sf::Vector2f(bounds.width, bounds.height));
        bg.setPosition(bounds.left, bounds.top);
        bg.setFillColor(sf::Color(20, 20, 30));
        bg.setOutlineThickness(focused ? 2.f : 1.f);
        bg.setOutlineColor(focused ? sf::Color(80, 150, 255) : sf::Color(70, 70, 100));
        window.draw(bg);

        sf::Text lbl(label + ":", font, 12);
        lbl.setFillColor(sf::Color(160, 160, 200));
        lbl.setPosition(bounds.left, bounds.top - 16);
        window.draw(lbl);

        sf::Text val(value + (focused ? "|" : ""), font, 13);
        val.setFillColor(sf::Color(230, 230, 250));
        val.setPosition(bounds.left + 6, bounds.top + (bounds.height - 13) * 0.5f);
        window.draw(val);
    }

    void handleEvent(const sf::Event& e) {
        if (!focused) return;
        if (e.type == sf::Event::TextEntered) {
            if (e.text.unicode == 8) {  // backspace
                if (!value.empty()) value.pop_back();
            } else if (e.text.unicode >= 32) {
                value += static_cast<char>(e.text.unicode);
            }
        }
    }
};

} // namespace UI
