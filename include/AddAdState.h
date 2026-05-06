#pragma once
#include "IState.h"
#include "AppContext.h"
#include "UIHelper.h"
#include <array>

// AddAdState – formularz dodawania ogłoszenia (tylko Sprzedawca).
// SRP: odpowiada wyłącznie za zbieranie danych i wywołanie inventory.addCar().
class AddAdState : public IState {
public:
    explicit AddAdState(AppContext& ctx);

    void handleEvent(sf::Event& event, StateManager& sm) override;
    void update(float dt, StateManager& sm) override {}
    void render(sf::RenderWindow& window) override;

private:
    AppContext& m_ctx;

    static constexpr int FIELD_COUNT = 7;
    std::array<UI::InputField, FIELD_COUNT> m_fields;
    int m_focused = 0;

    sf::FloatRect m_btnSave { 300, 520, 100, 32 };
    sf::FloatRect m_btnBack { 10,  10,  80,  28 };

    std::string m_message;
    bool m_saved = false;

    void save(StateManager& sm);
};
