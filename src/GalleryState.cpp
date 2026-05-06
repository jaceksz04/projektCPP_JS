#include "GalleryState.h"
#include <cmath>
#include "DetailState.h"
#include "AddAdState.h"
#include "MenuState.h"

GalleryState::GalleryState(AppContext& ctx) : m_ctx(ctx) {
    m_filterBrand    = { {10, 60, 140, 24}, "Marka", "", false };
    m_filterMinPrice = { {10, 100, 140, 24}, "Cena min", "", false };
    m_filterMaxPrice = { {10, 140, 140, 24}, "Cena max", "", false };
    applyFilters();
}

void GalleryState::applyFilters() {
    CompositeFilter cf;

    if (!m_filterBrand.value.empty())
        cf.add(std::make_shared<BrandFilter>(m_filterBrand.value));

    double mn = 0, mx = 1e9;
    try { if (!m_filterMinPrice.value.empty()) mn = std::stod(m_filterMinPrice.value); } catch(...) {}
    try { if (!m_filterMaxPrice.value.empty()) mx = std::stod(m_filterMaxPrice.value); } catch(...) {}
    if (mn > 0 || mx < 1e9)
        cf.add(std::make_shared<PriceFilter>(mn, mx));

    m_visible = m_ctx.inventory.filterAndSort(cf, m_sortField, m_sortOrder);
    m_scrollOffset = 0;
    m_selected = -1;
}

void GalleryState::handleEvent(sf::Event& event, StateManager& sm) {
    if (m_focusedField == 0) m_filterBrand.handleEvent(event);
    else if (m_focusedField == 1) m_filterMinPrice.handleEvent(event);
    else if (m_focusedField == 2) m_filterMaxPrice.handleEvent(event);

    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left) {

        sf::Vector2f mp(static_cast<float>(event.mouseButton.x),
                        static_cast<float>(event.mouseButton.y));

        auto activateField = [&](UI::InputField& f, int idx) {
            if (f.bounds.contains(mp)) {
                if (m_focusedField == 0) m_filterBrand.focused = false;
                else if (m_focusedField == 1) m_filterMinPrice.focused = false;
                else if (m_focusedField == 2) m_filterMaxPrice.focused = false;
                f.focused = true; m_focusedField = idx; return true;
            }
            return false;
        };
        activateField(m_filterBrand, 0) ||
        activateField(m_filterMinPrice, 1) ||
        activateField(m_filterMaxPrice, 2);

        if (m_btnSortP.contains(mp)) {
            m_sortField = SortField::Price;
            m_sortOrder = (m_sortOrder == SortOrder::Asc) ? SortOrder::Desc : SortOrder::Asc;
            applyFilters();
        }
        if (m_btnSortY.contains(mp)) {
            m_sortField = SortField::Year;
            m_sortOrder = (m_sortOrder == SortOrder::Asc) ? SortOrder::Desc : SortOrder::Asc;
            applyFilters();
        }

        if (m_btnApply.contains(mp))  applyFilters();
        if (m_btnBack.contains(mp))   sm.replaceDeferred(std::make_shared<MenuState>(m_ctx));

        if (m_btnDetail.contains(mp) && m_selected >= 0 && m_selected < (int)m_visible.size())
            sm.pushDeferred(std::make_shared<DetailState>(m_ctx, m_visible[m_selected]));

        if (m_btnAddCar.contains(mp) && m_ctx.currentUser.isSeller())
            sm.pushDeferred(std::make_shared<AddAdState>(m_ctx));

        // Klik na karte
        float gx = GRID_X + 165;
        for (int i = 0; i < (int)m_visible.size(); ++i) {
            int col = i % COLS;
            int row = i / COLS - m_scrollOffset;
            if (row < 0 || row >= 3) continue;
            float cx = gx + col * (CARD_W + GAP);
            float cy = GRID_Y + row * (CARD_H + GAP);
            sf::FloatRect cardRect(cx, cy, CARD_W, CARD_H);
            if (cardRect.contains(mp)) { m_selected = i; break; }
        }
    }

    if (event.type == sf::Event::MouseWheelScrolled) {
        m_scrollOffset -= static_cast<int>(event.mouseWheelScroll.delta);
        int maxOff = std::max(0, (int)std::ceil(m_visible.size() / (float)COLS) - 3);
        m_scrollOffset = std::max(0, std::min(m_scrollOffset, maxOff));
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return)
        applyFilters();
}

void GalleryState::render(sf::RenderWindow& window) {
    window.clear(sf::Color(15, 15, 25));
    drawSidebar(window);
    drawCarGrid(window);
}

void GalleryState::drawSidebar(sf::RenderWindow& window) {
    sf::RectangleShape side(sf::Vector2f(155, 600));
    side.setFillColor(sf::Color(20, 20, 32));
    side.setOutlineColor(sf::Color(50, 50, 80));
    side.setOutlineThickness(1);
    window.draw(side);

    sf::Text t("Filtry", m_ctx.font, 14);
    t.setFillColor(sf::Color(120, 160, 230));
    t.setPosition(10, 10);
    window.draw(t);

    m_filterBrand.draw(window, m_ctx.font);
    m_filterMinPrice.draw(window, m_ctx.font);
    m_filterMaxPrice.draw(window, m_ctx.font);

    auto mp = sf::Vector2f(sf::Mouse::getPosition(window));
    UI::drawButton(window, m_ctx.font, m_btnApply, "Szukaj", m_btnApply.contains(mp));

    if (m_ctx.currentUser.isSeller())
        UI::drawButton(window, m_ctx.font, m_btnAddCar, "+ Dodaj", m_btnAddCar.contains(mp));

    bool hasSelection = m_selected >= 0 && m_selected < (int)m_visible.size();
    UI::drawButton(window, m_ctx.font, m_btnDetail, "Szczegoly",
                   m_btnDetail.contains(mp), hasSelection);

    UI::drawButton(window, m_ctx.font, m_btnSortP, "Cena", m_btnSortP.contains(mp),
                   m_sortField == SortField::Price);
    UI::drawButton(window, m_ctx.font, m_btnSortY, "Rok",  m_btnSortY.contains(mp),
                   m_sortField == SortField::Year);

    UI::drawButton(window, m_ctx.font, m_btnBack, "< Menu", m_btnBack.contains(mp));

    sf::Text cnt("Wyniki: " + std::to_string(m_visible.size()), m_ctx.font, 12);
    cnt.setFillColor(sf::Color(140, 140, 160));
    cnt.setPosition(600, 10);
    window.draw(cnt);

    sf::Text role("Rola: " + m_ctx.currentUser.name, m_ctx.font, 12);
    role.setFillColor(sf::Color(100, 200, 100));
    role.setPosition(10, 550);
    window.draw(role);
}

void GalleryState::drawCarGrid(sf::RenderWindow& window) {
    float gx = GRID_X + 165;
    int visRows = 3;

    for (int i = 0; i < (int)m_visible.size(); ++i) {
        int col = i % COLS;
        int row = i / COLS - m_scrollOffset;
        if (row < 0 || row >= visRows) continue;

        float cx = gx + col * (CARD_W + GAP);
        float cy = GRID_Y + row * (CARD_H + GAP);
        m_ctx.renderer.drawCard(window, m_visible[i],
                                sf::FloatRect(cx, cy, CARD_W, CARD_H),
                                i == m_selected);
    }

    if (m_visible.empty()) {
        sf::Text empty("Brak ofert spelniajacych kryteria.", m_ctx.font, 16);
        empty.setFillColor(sf::Color(140, 140, 160));
        empty.setPosition(GRID_X + 165 + 40, 250);
        window.draw(empty);
    }
}
