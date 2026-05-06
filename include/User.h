#pragma once
#include <string>

enum class UserRole { Buyer, Seller };

struct User {
    std::string name;
    UserRole    role;

    bool isSeller() const { return role == UserRole::Seller; }
};
