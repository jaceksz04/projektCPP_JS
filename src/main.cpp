#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#include <fstream>

#include "CsvCarRepository.h"
#include "CarInventory.h"
#include "CarRenderer.h"
#include "AppContext.h"
#include "IState.h"
#include "MenuState.h"
#include "User.h"

int main() {

    std::ofstream log("log.txt");
    log << "=== CarComis start ===\n"; log.flush();

    log << "Tworze okno...\n"; log.flush();
    sf::RenderWindow window(sf::VideoMode(800, 600), "Komis Samochodowy",
                            sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);
    log << "Okno OK\n"; log.flush();

    log << "Laduje czcionke...\n"; log.flush();
    sf::Font font;
    bool fontLoaded = false;
    const char* fontPaths[] = {
        "assets/fonts/DejaVuSans.ttf",
        "C:/Windows/Fonts/arial.ttf",
        "C:/Windows/Fonts/Arial.ttf",
        "C:/Windows/Fonts/verdana.ttf",
        nullptr
    };
    for (int i = 0; fontPaths[i] != nullptr; ++i) {
        if (font.loadFromFile(fontPaths[i])) {
            log << "Czcionka: " << fontPaths[i] << "\n";
            fontLoaded = true;
            break;
        }
    }
    if (!fontLoaded) {
        log << "BLAD: brak czcionki!\n";
        system("pause");
        return -1;
    }
    log.flush();

    log << "Inicjalizuje repozytorium...\n"; log.flush();
    system("mkdir data 2>nul");

    auto repo      = std::make_shared<CsvCarRepository>("data/cars.csv");
    auto inventory = std::make_shared<CarInventory>(repo);
    inventory->load();
    log << "Samochodow: " << inventory->getAllCars().size() << "\n"; log.flush();

    if (inventory->getAllCars().empty()) {
        inventory->addCar("Toyota",     "Corolla",  2019,  45000,  72000, "Jeden wlasciciel, stan bardzo dobry.", "");
        inventory->addCar("Volkswagen", "Golf",     2017,  98000,  58000, "Serwisowany w ASO, nowe opony.",       "");
        inventory->addCar("BMW",        "3 Series", 2020,  32000, 125000, "Wersja M-Sport, skorzana tapicerka.",  "");
        inventory->addCar("Ford",       "Focus",    2015, 145000,  34000, "Ekonomiczny, bogate wyposazenie.",     "");
        inventory->addCar("Audi",       "A4",       2018,  77000,  95000, "Quattro, LED, nawigacja.",             "");
        inventory->save();
        log << "Przykladowe dane dodane.\n"; log.flush();
    }

    log << "Tworze renderer...\n"; log.flush();
    CarRenderer renderer(font);

    log << "Tworze kontekst i stany...\n"; log.flush();
    User currentUser;
    AppContext ctx{ window, font, *inventory, renderer, currentUser };

    StateManager stateManager;
    stateManager.push(std::make_shared<MenuState>(ctx));
    log << "Wchodze do petli glownej.\n"; log.flush();

    sf::Clock clock;

    while (window.isOpen() && !stateManager.empty()) {

        float dt = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (auto* state = stateManager.current())
                state->handleEvent(event, stateManager);
        }

        if (auto* state = stateManager.current())
            state->update(dt, stateManager);

        stateManager.applyPending();

        if (auto* state = stateManager.current())
            state->render(window);

        window.display();
    }

    log << "Koniec.\n";
    inventory->save();
    return 0;
}
