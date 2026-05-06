#include "DetailState.h"

void DetailState::handleEvent(sf::Event& event, StateManager& sm) {
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left) {

        sf::Vector2f mp(static_cast<float>(event.mouseButton.x),
                        static_cast<float>(event.mouseButton.y));

        if (m_btnBack.contains(mp)) { sm.pop(); return; }

        if (m_btnDelete.contains(mp) && m_ctx.currentUser.isSeller()) {
            m_ctx.inventory.removeCar(m_car.id);
            m_ctx.inventory.save();
            m_message = "Ogłoszenie usunięte.";
            m_deleted = true;
        }

        if (m_btnBuy.contains(mp) && !m_ctx.currentUser.isSeller() && !m_bought) {
            m_message = "Gratulacje! Zakup pojazdu " + m_car.brand + " " + m_car.model + " udany.";
            m_bought = true;
            // W pełnej implementacji: usuń z repo lub oznacz jako sprzedany
        }
    }
}

void DetailState::render(sf::RenderWindow& window) {
    window.clear(sf::Color(15, 15, 25));

    m_ctx.renderer.drawDetail(window, m_car, sf::FloatRect(10, 50, 780, 500));

    auto mp = sf::Vector2f(sf::Mouse::getPosition(window));
    UI::drawButton(window, m_ctx.font, m_btnBack, "< Wróć", m_btnBack.contains(mp));

    if (m_ctx.currentUser.isSeller() && !m_deleted)
        UI::drawButton(window, m_ctx.font, m_btnDelete, "Usuń", m_btnDelete.contains(mp));

    if (!m_ctx.currentUser.isSeller() && !m_bought)
        UI::drawButton(window, m_ctx.font, m_btnBuy, "Kup", m_btnBuy.contains(mp));

    if (!m_message.empty()) {
        sf::Text msg(m_message, m_ctx.font, 15);
        msg.setFillColor(sf::Color(80, 220, 100));
        msg.setPosition(10, 565);
        window.draw(msg);
    }
}
