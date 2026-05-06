#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "CarInventory.h"
#include "CarRenderer.h"
#include "User.h"

// AppContext jest wstrzyknięciem zależności dla wszystkich State'ów (DIP).
// Stany nie tworzą sami zależności – dostają je przez referencję.
struct AppContext {
    sf::RenderWindow& window;
    sf::Font&         font;
    CarInventory&     inventory;
    CarRenderer&      renderer;
    User&             currentUser;
};
