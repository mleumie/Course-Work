#pragma once
#include <string>

enum class Role { Buyer, Seller, Admin };

inline std::string roleToString(Role role) {
    switch (role) {
    case Role::Buyer:  return "buyer";
    case Role::Seller: return "seller";
    case Role::Admin:  return "admin";
    }
    return "buyer";
}

inline Role stringToRole(const std::string& s) {
    if (s == "buyer")  return Role::Buyer;
    if (s == "seller") return Role::Seller;
    if (s == "admin")  return Role::Admin;
    return Role::Buyer;
}