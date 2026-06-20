// entities/User.cpp
// Purpose: Implementation of the User class methods, defining user profile printing and basic getters.

#include "User.h"

#include <iostream>
#include <iomanip>
#include <string>

// Prints a standard visual divider line in the console for user profile displays
static void printUserDivider() {
    std::cout << "  " << std::string(50, '-') << "\n";
}

// Default constructor initializing user attributes to blank values, defaulting role to viewer
User::User()
    : userId(""),
      name(""),
      email(""),
      region(""),
      role("viewer")
{}

// Parameterized constructor initializing a user object with explicit parameters
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

// Outputs a single-line user entry suitable for list formatting
void User::displaySummary() const {
    std::cout << "  ["
              << std::left << std::setw(7)  << userId << "] "
              << std::left << std::setw(22) << name
              << " | " << std::left << std::setw(12) << region
              << " | " << role
              << "\n";
}

// Displays detailed user profile information in a clear, formatted block
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

// Inline accessor definitions for encapsulation
std::string User::getUserId() const { return userId; }
std::string User::getName()   const { return name;   }
std::string User::getRegion() const { return region; }
std::string User::getRole()   const { return role;   }
