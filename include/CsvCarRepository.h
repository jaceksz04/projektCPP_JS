#pragma once
#include "ICarRepository.h"
#include <string>

// Konkretna implementacja repozytorium CSV.
// OCP: chc¹c dodaæ np. SqliteRepository, implementujemy ICarRepository bez zmiany kodu klienta.
class CsvCarRepository : public ICarRepository {
public:
    explicit CsvCarRepository(const std::string& filePath);

    void             add(const Car& car)    override;
    void             remove(int id)         override;
    std::vector<Car> getAll() const         override;
    bool             load()                 override;
    bool             save() const           override;
    int              nextId() const         override;

private:
    std::string      m_filePath;
    std::vector<Car> m_cars;
    int              m_nextId = 1;
};
