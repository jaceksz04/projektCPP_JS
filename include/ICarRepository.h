#pragma once
#include <vector>
#include <memory>
#include "Car.h"

// ISP + DIP: abstrakcja repozytorium – GUI i logika biznesowa zależą tylko od tego interfejsu,
// nie od konkretnej implementacji (plik CSV, baza danych, itp.)
class ICarRepository {
public:
    virtual ~ICarRepository() = default;

    virtual void             add(const Car& car)    = 0;
    virtual void             remove(int id)         = 0;
    virtual std::vector<Car> getAll() const         = 0;
    virtual bool             load()                 = 0;   // odczyt z nośnika
    virtual bool             save() const           = 0;   // zapis na nośnik
    virtual int              nextId() const         = 0;
};
