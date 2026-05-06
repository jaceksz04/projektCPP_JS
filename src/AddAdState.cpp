#include "AddAdState.h"
#include <stdexcept>

AddAdState::AddAdState(AppContext& ctx) : m_ctx(ctx) {
    // Definicja pól formularza
    float startY = 80;
    float stepY  = 60;
    std::array<std::string, FIELD_COUNT> labels =
        {"Marka", "Model", "Rok", "Przebieg (km)", "Cena (PLN)", "Opis", "Sciezka do zdjecia"};

    for (int i = 0; i < FIELD_COUNT; ++i) {
        m_fields[i] = { {250.f, startY + i * stepY, 300.f, 28.f}, labels[i], "", false };
    }
    m_fields[0].focused = true;
}

void AddAdState::save(StateManager& sm) {
    try {
        std::string brand  = m_fields[0].value;
        std::string model  = m_fields[1].value;
        int    year    = std::stoi(m_fields[2].value);
        int    mileage = std::stoi(m_fields[3].value);
        double price   = std::stod(m_fields[4].value);
        std::string desc   = m_fields[5].value;
        std::string img    = m_fields[6].value;

        if (brand.empty() || model.empty())
            throw std::invalid_argument("Marka i model są wymagane.");

        m_ctx.inventory.addCar(brand, model, year, mileage, price, desc, img);
        m_ctx.inventory.save();
        m_message = "Ogłoszenie dodane pomyślnie!";
        m_saved = true;
    }
    catch (const std::exception& e) {
        m_message = std::string("Błąd: ") + e.what();
    }
}

void AddAdState::handleEvent(sf::Event& event, StateManager& sm) {
    // Przekaż do aktywnego pola
    m_fields[m_focused].handleEvent(event);

    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left) {

        sf::Vector2f mp(static_cast<float>(event.mouseButton.x),
                        static_cast<float>(event.mouseButton.y));

        if (m_btnBack.contains(mp)) { sm.pop(); return; }
        if (m_btnSave.contains(mp)) { save(sm); return; }

        // Aktywacja pola kliknięciem
        for (int i = 0; i < FIELD_COUNT; ++i) {
            if (m_fields[i].bounds.contains(mp)) {
                m_fields[m_focused].focused = false;
                m_focused = i;
                m_fields[m_focused].focused = true;
                break;
            }
        }
    }

    // Tab – następne pole
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Tab) {
        m_fields[m_focused].focused = false;
        m_focused = (m_focused + 1) % FIELD_COUNT;
        m_fields[m_focused].focused = true;
    }
}

void AddAdState::render(sf::RenderWindow& window) {
    window.clear(sf::Color(15, 15, 25));

    sf::Text title("Dodaj ogłoszenie", m_ctx.font, 22);
    title.setFillColor(sf::Color(80, 160, 255));
    title.setPosition(260, 20);
    window.draw(title);

    for (const auto& f : m_fields) f.draw(window, m_ctx.font);

    auto mp = sf::Vector2f(sf::Mouse::getPosition(window));
    UI::drawButton(window, m_ctx.font, m_btnBack, "< Wróć",    m_btnBack.contains(mp));
    UI::drawButton(window, m_ctx.font, m_btnSave, "Zapisz",    m_btnSave.contains(mp));

    if (!m_message.empty()) {
        sf::Text msg(m_message, m_ctx.font, 14);
        msg.setFillColor(m_saved ? sf::Color(80, 220, 100) : sf::Color(220, 80, 80));
        msg.setPosition(200, 565);
        window.draw(msg);
    }

    // Informacja o Tab
    sf::Text hint("Tab = nastepne pole", m_ctx.font, 11);
    hint.setFillColor(sf::Color(100, 100, 120));
    hint.setPosition(10, 580);
    window.draw(hint);
}
