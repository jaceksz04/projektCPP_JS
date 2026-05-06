#include "CarInventory.h"

CarInventory::CarInventory(std::shared_ptr<ICarRepository> repo)
    : m_repo(std::move(repo)) {}

void CarInventory::addCar(const std::string& brand, const std::string& model,
                           int year, int mileage, double price,
                           const std::string& description, const std::string& imagePath) {
    Car c(m_repo->nextId(), brand, model, year, mileage, price, description, imagePath);
    m_repo->add(c);
}

void CarInventory::removeCar(int id) {
    m_repo->remove(id);
}

std::vector<Car> CarInventory::getAllCars() const {
    return m_repo->getAll();
}

std::vector<Car> CarInventory::filter(const IFilterStrategy& strategy) const {
    std::vector<Car> result;
    for (const auto& car : m_repo->getAll())
        if (strategy.matches(car))
            result.push_back(car);
    return result;
}

std::vector<Car> CarInventory::filterAndSort(const IFilterStrategy& strategy,
                                              SortField field, SortOrder order) const {
    auto result = filter(strategy);
    sortCars(result, field, order);
    return result;
}

bool CarInventory::load()        { return m_repo->load(); }
bool CarInventory::save() const  { return m_repo->save(); }
