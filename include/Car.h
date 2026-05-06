#pragma once
#include <string>

// Pojedyncza encja danych – odpowiedzialność: tylko przechowywanie danych pojazdu (SRP)
struct Car {
    int         id;
    std::string brand;
    std::string model;
    int         year;
    int         mileage;
    double      price;
    std::string description;
    std::string imagePath;

    Car() : id(0), year(0), mileage(0), price(0.0) {}

    Car(int id, std::string brand, std::string model,
        int year, int mileage, double price,
        std::string description, std::string imagePath)
        : id(id), brand(std::move(brand)), model(std::move(model)),
          year(year), mileage(mileage), price(price),
          description(std::move(description)), imagePath(std::move(imagePath)) {}
};
