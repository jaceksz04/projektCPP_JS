#pragma once
#include "Car.h"
#include <vector>
#include <functional>
#include <algorithm>
#include <string>

// ── Strategy / Predicate – OCP ────────────────────────────────────────────────
// Nowe kryterium filtrowania = nowa klasa implementująca IFilterStrategy.
// Kod CarInventory::filter() nigdy się nie zmienia.

struct IFilterStrategy {
    virtual ~IFilterStrategy() = default;
    virtual bool matches(const Car& car) const = 0;
};

// Filtr po cenie (przedział)
struct PriceFilter : IFilterStrategy {
    double minPrice, maxPrice;
    PriceFilter(double mn, double mx) : minPrice(mn), maxPrice(mx) {}
    bool matches(const Car& car) const override {
        return car.price >= minPrice && car.price <= maxPrice;
    }
};

// Filtr po marce (case-insensitive contains)
struct BrandFilter : IFilterStrategy {
    std::string query;
    explicit BrandFilter(const std::string& q) : query(q) {}
    bool matches(const Car& car) const override {
        std::string b = car.brand, q = query;
        std::transform(b.begin(), b.end(), b.begin(), ::tolower);
        std::transform(q.begin(), q.end(), q.begin(), ::tolower);
        return b.find(q) != std::string::npos;
    }
};

// Filtr rocznikowy
struct YearFilter : IFilterStrategy {
    int minYear, maxYear;
    YearFilter(int mn, int mx) : minYear(mn), maxYear(mx) {}
    bool matches(const Car& car) const override {
        return car.year >= minYear && car.year <= maxYear;
    }
};

// Filtr złożony (AND) – kompozyt
struct CompositeFilter : IFilterStrategy {
    std::vector<std::shared_ptr<IFilterStrategy>> filters;
    void add(std::shared_ptr<IFilterStrategy> f) { filters.push_back(std::move(f)); }
    bool matches(const Car& car) const override {
        for (const auto& f : filters)
            if (!f->matches(car)) return false;
        return true;
    }
};

// ── Sort strategy ─────────────────────────────────────────────────────────────
enum class SortField { Price, Year };
enum class SortOrder { Asc, Desc };

inline void sortCars(std::vector<Car>& cars, SortField field, SortOrder order) {
    std::sort(cars.begin(), cars.end(), [field, order](const Car& a, const Car& b){
        bool less;
        if (field == SortField::Price) less = a.price < b.price;
        else                           less = a.year  < b.year;
        return order == SortOrder::Asc ? less : !less;
    });
}
