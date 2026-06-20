#include "User.h"

#include <iostream>
#include <iomanip>
#include <string>

static void printUserDivider() {
    std::cout << "  " << std::string(50, '-') << "\n";
}

User::User()
    : userId(""),
      name(""),
      email(""),
      region(""),
      role("viewer")
{}

User::User(std::string userId,
           std::string name,
           std::string email,
           std::string region,
           std::string role)
    : userId(userId),
      name(name),
      email(email),
      region(region),
      role(role)
{}

void User::displaySummary() const {
    std::cout << "  ["
              << std::left << std::setw(7)  << userId << "] "
              << std::left << std::setw(22) << name
              << " | " << std::left << std::setw(12) << region
              << " | " << role
              << "\n";
}

void User::displayFull() const {
    printUserDivider();
    std::cout << "  \xF0\x9F\x91\xA4  " << name << "\n";
    printUserDivider();
    std::cout << std::left;
    std::cout << "  " << std::setw(12) << "User ID"  << ": " << userId << "\n";
    std::cout << "  " << std::setw(12) << "Email"    << ": " << email  << "\n";
    std::cout << "  " << std::setw(12) << "Region"   << ": " << region << "\n";
    std::cout << "  " << std::setw(12) << "Role"     << ": " << role   << "\n";
    printUserDivider();
}

std::string User::getUserId() const { return userId; }
std::string User::getName()   const { return name;   }
std::string User::getRegion() const { return region; }
std::string User::getRole()   const { return role;   }
