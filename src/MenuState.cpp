#include "MenuState.h"
#include "GalleryState.h"

void MenuState::handleEvent(sf::Event& event, StateManager& sm) {
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left) {

        auto mp = sf::Vector2f(static_cast<float>(event.mouseButton.x),
                               static_cast<float>(event.mouseButton.y));

        if (m_btnBuyer.contains(mp)) {
            m_ctx.currentUser = {"Kupujacy", UserRole::Buyer};
            sm.replaceDeferred(std::make_shared<GalleryState>(m_ctx));
        }
        else if (m_btnSeller.contains(mp)) {
            m_ctx.currentUser = {"Sprzedawca", UserRole::Seller};
            sm.replaceDeferred(std::make_shared<GalleryState>(m_ctx));
        }
    }
}

void MenuState::render(sf::RenderWindow& window) {
    window.clear(sf::Color(15, 15, 25));

    sf::Text title("Komis Samochodowy", m_ctx.font, 36);
    title.setFillColor(sf::Color(80, 160, 255));
    title.setPosition(210, 160);
    window.draw(title);

    sf::Text sub("Wybierz role:", m_ctx.font, 18);
    sub.setFillColor(sf::Color(180, 180, 200));
    sub.setPosition(340, 240);
    window.draw(sub);

    auto mp = sf::Mouse::getPosition(window);
    sf::Vector2f mf(static_cast<float>(mp.x), static_cast<float>(mp.y));
    UI::drawButton(window, m_ctx.font, m_btnBuyer,  "Kupujacy",   m_btnBuyer.contains(mf));
    UI::drawButton(window, m_ctx.font, m_btnSeller, "Sprzedawca", m_btnSeller.contains(mf));
}
