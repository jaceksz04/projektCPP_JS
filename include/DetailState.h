#pragma once
#include "IState.h"
#include "AppContext.h"
#include "UIHelper.h"
#include "Car.h"

// DetailState – pełny widok szczegółowy pojazdu
// SRP: odpowiada tylko za prezentację i akcję zakupu/usunięcia.
class DetailState : public IState {
public:
    DetailState(AppContext& ctx, const Car& car)
        : m_ctx(ctx), m_car(car) {}

    void handleEvent(sf::Event& event, StateManager& sm) override;
    void update(float dt, StateManager& sm) override {}
    void render(sf::RenderWindow& window) override;

private:
    AppContext& m_ctx;
    Car         m_car;

    sf::FloatRect m_btnBack   { 10, 10, 80, 28 };
    sf::FloatRect m_btnDelete { 700, 10, 90, 28 };
    sf::FloatRect m_btnBuy    { 700, 48, 90, 28 };

    bool m_bought  = false;
    bool m_deleted = false;
    std::string m_message;
};
