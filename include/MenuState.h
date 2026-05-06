#pragma once
#include "IState.h"
#include "AppContext.h"
#include "UIHelper.h"

// MenuState – ekran powitalny z wyborem roli.
// SRP: odpowiada tylko za prezentację menu i wybór roli.
class MenuState : public IState {
public:
    explicit MenuState(AppContext& ctx) : m_ctx(ctx) {}

    void handleEvent(sf::Event& event, StateManager& sm) override;
    void update(float dt, StateManager& sm) override {}
    void render(sf::RenderWindow& window) override;

private:
    AppContext& m_ctx;

    sf::FloatRect m_btnBuyer  { 300, 280, 200, 50 };
    sf::FloatRect m_btnSeller { 300, 350, 200, 50 };
};
