#include "CarRenderer.h"
#include <sstream>
#include <iomanip>

CarRenderer::CarRenderer(sf::Font& font) : m_font(font) {}

// Leniwe ładowanie tekstur z cache
sf::Texture& CarRenderer::getOrLoadTexture(const std::string& path) {
    auto it = m_textureCache.find(path);
    if (it != m_textureCache.end()) return it->second;

    sf::Texture tex;
    if (!tex.loadFromFile(path)) {
        // Tworzymy szarą teksturę zastępczą 1×1
        sf::Image img;
        img.create(1, 1, sf::Color(180, 180, 180));
        tex.loadFromImage(img);
    }
    m_textureCache[path] = std::move(tex);
    return m_textureCache[path];
}

void CarRenderer::drawPlaceholder(sf::RenderWindow& window, const sf::FloatRect& rect) {
    sf::RectangleShape bg(sf::Vector2f(rect.width, rect.height));
    bg.setPosition(rect.left, rect.top);
    bg.setFillColor(sf::Color(60, 60, 70));

    sf::Text txt("Brak\nzdjecia", m_font, 12);
    txt.setFillColor(sf::Color(180, 180, 180));
    txt.setPosition(rect.left + rect.width * 0.5f - 20, rect.top + rect.height * 0.5f - 12);

    window.draw(bg);
    window.draw(txt);
}

// ── Karta w galerii ───────────────────────────────────────────────────────────
void CarRenderer::drawCard(sf::RenderWindow& window, const Car& car,
                            const sf::FloatRect& bounds, bool selected) {
    // Tło karty
    sf::RectangleShape bg(sf::Vector2f(bounds.width, bounds.height));
    bg.setPosition(bounds.left, bounds.top);
    bg.setFillColor(selected ? sf::Color(40, 60, 90) : sf::Color(30, 30, 40));
    bg.setOutlineThickness(selected ? 2.f : 1.f);
    bg.setOutlineColor(selected ? sf::Color(80, 140, 220) : sf::Color(60, 60, 80));
    window.draw(bg);

    // Miniaturka
    float imgH = bounds.height * 0.55f;
    sf::FloatRect imgRect(bounds.left + 4, bounds.top + 4,
                          bounds.width - 8, imgH - 4);

    if (!car.imagePath.empty()) {
        sf::Texture& tex = getOrLoadTexture(car.imagePath);
        sf::Sprite sprite(tex);
        // Skalowanie zachowujące proporcje
        float scaleX = imgRect.width  / tex.getSize().x;
        float scaleY = imgRect.height / tex.getSize().y;
        float scale  = std::min(scaleX, scaleY);
        sprite.setScale(scale, scale);
        sprite.setPosition(imgRect.left, imgRect.top);
        window.draw(sprite);
    } else {
        drawPlaceholder(window, imgRect);
    }

    // Tekst
    float textY = bounds.top + imgH + 6;
    auto makeText = [&](const std::string& s, unsigned size, sf::Color col) {
        sf::Text t(s, m_font, size);
        t.setFillColor(col);
        return t;
    };

    sf::Text titleTxt = makeText(car.brand + " " + car.model, 13, sf::Color(220, 220, 240));
    titleTxt.setPosition(bounds.left + 6, textY);
    window.draw(titleTxt);

    std::ostringstream ss;
    ss << std::fixed << std::setprecision(0) << car.price << " PLN";
    sf::Text priceTxt = makeText(ss.str(), 12, sf::Color(80, 200, 120));
    priceTxt.setPosition(bounds.left + 6, textY + 16);
    window.draw(priceTxt);

    sf::Text yearTxt = makeText(std::to_string(car.year) + " | " +
                                std::to_string(car.mileage) + " km", 11,
                                sf::Color(160, 160, 180));
    yearTxt.setPosition(bounds.left + 6, textY + 30);
    window.draw(yearTxt);
}

// ── Widok szczegółowy ─────────────────────────────────────────────────────────
void CarRenderer::drawDetail(sf::RenderWindow& window, const Car& car,
                              const sf::FloatRect& bounds) {
    // Tło panelu
    sf::RectangleShape bg(sf::Vector2f(bounds.width, bounds.height));
    bg.setPosition(bounds.left, bounds.top);
    bg.setFillColor(sf::Color(25, 25, 35));
    bg.setOutlineThickness(1.f);
    bg.setOutlineColor(sf::Color(70, 70, 100));
    window.draw(bg);

    // Zdjęcie (lewa połowa)
    float imgW = bounds.width * 0.45f;
    sf::FloatRect imgRect(bounds.left + 10, bounds.top + 10, imgW, bounds.height - 20);

    if (!car.imagePath.empty()) {
        sf::Texture& tex = getOrLoadTexture(car.imagePath);
        sf::Sprite sprite(tex);
        float scale = std::min(imgRect.width  / tex.getSize().x,
                               imgRect.height / tex.getSize().y);
        sprite.setScale(scale, scale);
        sprite.setPosition(imgRect.left, imgRect.top);
        window.draw(sprite);
    } else {
        drawPlaceholder(window, imgRect);
    }

    // Tekst (prawa strona)
    float tx = bounds.left + imgW + 20;
    float ty = bounds.top + 15;

    auto line = [&](const std::string& txt, unsigned sz, sf::Color col, float& y) {
        sf::Text t(txt, m_font, sz);
        t.setFillColor(col);
        t.setPosition(tx, y);
        window.draw(t);
        y += sz + 6;
    };

    line(car.brand + " " + car.model, 20, sf::Color(230, 230, 255), ty);
    ty += 4;
    line("Rok: "      + std::to_string(car.year),    14, sf::Color(200,200,200), ty);
    line("Przebieg: " + std::to_string(car.mileage) + " km", 14, sf::Color(200,200,200), ty);

    std::ostringstream ss;
    ss << "Cena: " << std::fixed << std::setprecision(2) << car.price << " PLN";
    line(ss.str(), 16, sf::Color(80, 210, 120), ty);
    ty += 8;

    // Opis – zawijanie co ~40 znaków
    std::string desc = car.description;
    while (!desc.empty()) {
        size_t cut = std::min<size_t>(desc.size(), 40);
        // Szukamy ostatniej spacji
        if (cut < desc.size()) {
            size_t sp = desc.rfind(' ', cut);
            if (sp != std::string::npos) cut = sp;
        }
        line(desc.substr(0, cut), 12, sf::Color(170, 170, 190), ty);
        if (cut < desc.size()) desc = desc.substr(cut + 1);
        else desc.clear();
    }
}
