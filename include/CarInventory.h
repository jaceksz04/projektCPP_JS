#pragma once
#include "ICarRepository.h"
#include "FilterStrategy.h"
#include <memory>

// SRP: CarInventory zawiera TYLKO logikę biznesową komisu.
// Nie wie nic o GUI ani o tym, jak dane są serializowane.
// DIP: zależy od ICarRepository (abstrakcja), nie od CsvCarRepository.
class CarInventory {
public:
    explicit CarInventory(std::shared_ptr<ICarRepository> repo);

    // CRUD
    void addCar(const std::string& brand, const std::string& model,
                int year, int mileage, double price,
                const std::string& description, const std::string& imagePath);
    void removeCar(int id);

    // Zapytania
    std::vector<Car> getAllCars() const;
    std::vector<Car> filter(const IFilterStrategy& strategy) const;
    std::vector<Car> filterAndSort(const IFilterStrategy& strategy,
                                   SortField field, SortOrder order) const;

    // Trwałość
    bool load();
    bool save() const;

private:
    std::shared_ptr<ICarRepository> m_repo;
};
