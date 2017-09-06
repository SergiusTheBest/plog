#pragma once
#include <string>
#include <iostream>

struct Customer
{
    int id;
    std::string name;
};

inline std::ostream& operator<<(std::ostream& os, const Customer& obj)
{
    os << "Customer (id: " << obj.id << ", name: " << obj.name << ")";
    return os;
}
