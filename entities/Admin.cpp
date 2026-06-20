// entities/Admin.cpp
// Purpose: Implementation of Admin class methods, handling admin profile displays.

#include "Admin.h"

#include <iostream>
#include <iomanip>
#include <string>

// Prints a standard admin console divider
static void printAdminDivider() {
    std::cout << "  " << std::string(50, '=') << "\n";
}

// Default constructor initializing base class, setting role specifically to admin
Admin::Admin()
    : User()
{
    role = "admin";
}

// Parameterized constructor mapping identity fields to base class with role forced to admin
Admin::Admin(std::string userId,
             std::string name,
             std::string email,
             std::string region)
    : User(userId, name, email, region, "admin")
{}

// Prints a one-line console record highlighting admin status
void Admin::displaySummary() const {
    std::cout << "  ["
              << std::left << std::setw(7)  << userId << "] "
              << std::left << std::setw(22) << name
              << " | " << std::left << std::setw(12) << region
              << " | *** ADMIN ***"
              << "\n";
}

// Displays full administrator credentials and prints allowed system operations list
void Admin::displayFull() const {
    printAdminDivider();
    std::cout << "  \xE2\x9A\x99\xEF\xB8\x8F   " << name << "  [PLATFORM ADMINISTRATOR]\n";
    printAdminDivider();
    std::cout << std::left;
    std::cout << "  " << std::setw(12) << "User ID"  << ": " << userId << "\n";
    std::cout << "  " << std::setw(12) << "Email"    << ": " << email  << "\n";
    std::cout << "  " << std::setw(12) << "Region"   << ": " << region << "\n";
    std::cout << "  " << std::setw(12) << "Role"     << ": " << role   <<   "\n";
    printAdminDivider();
    std::cout << "  ACCESS: Rankings | Registry | Sharing Network | Delivery Routes\n";
    printAdminDivider();
}
