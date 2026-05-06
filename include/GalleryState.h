#pragma once
#include "IState.h"
#include "AppContext.h"
#include "UIHelper.h"
#include "FilterStrategy.h"
#include <vector>
#include <string>

// GalleryState – galeria aut z filtrami i sortowaniem.
// SRP: odpowiada za prezentację listy i obsługę filtrów.
class GalleryState : public IState {
public:
    explicit GalleryState(AppContext& ctx);

    void handleEvent(sf::Event& event, StateManager& sm) override;
    void update(float dt, StateManager& sm) override {}
    void render(sf::RenderWindow& window) override;

private:
    AppContext& m_ctx;

    // Widoczne samochody (po filtrowaniu/sortowaniu)
    std::vector<Car> m_visible;
    int              m_selected = -1;
    int              m_scrollOffset = 0;

    // Filtry
    UI::InputField m_filterBrand;
    UI::InputField m_filterMinPrice;
    UI::InputField m_filterMaxPrice;
    int            m_focusedField = -1; // indeks aktywnego pola

    SortField m_sortField = SortField::Price;
    SortOrder m_sortOrder = SortOrder::Asc;

    // Przyciski
    sf::FloatRect m_btnApply    { 760, 60,  100, 28 };
    sf::FloatRect m_btnAddCar   { 760, 98,  100, 28 };
    sf::FloatRect m_btnDetail   { 760, 136, 100, 28 };
    sf::FloatRect m_btnBack     { 10,  10,  80,  28 };
    sf::FloatRect m_btnSortP    { 600, 60,  80,  28 };
    sf::FloatRect m_btnSortY    { 690, 60,  60,  28 };

    void applyFilters();
    void drawSidebar(sf::RenderWindow& window);
    void drawCarGrid(sf::RenderWindow& window);

    // Geometria siatki kart
    static constexpr int   COLS     = 3;
    static constexpr float CARD_W   = 220.f;
    static constexpr float CARD_H   = 170.f;
    static constexpr float GRID_X   = 10.f;
    static constexpr float GRID_Y   = 10.f;
    static constexpr float GAP      = 10.f;
};
