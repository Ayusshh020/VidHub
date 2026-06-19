// Note: Comments are only for understanding—do not modify the code itself.
#include "Admin.h"

#include <iostream>
#include <iomanip>
#include <string>

static void printAdminDivider() {
    std::cout << "  " << std::string(50, '=') << "\n";
}

Admin::Admin()
    : User()
{
    role = "admin";
}

Admin::Admin(std::string userId,
             std::string name,
             std::string email,
             std::string region)
    : User(userId, name, email, region, "admin")
{}

void Admin::displaySummary() const {
    std::cout << "  ["
              << std::left << std::setw(7)  << userId << "] "
              << std::left << std::setw(22) << name
              << " | " << std::left << std::setw(12) << region
              << " | *** ADMIN ***"
              << "\n";
}

void Admin::displayFull() const {
    printAdminDivider();
    std::cout << "  \xE2\x9A\x99\xEF\xB8\x8F   " << name << "  [PLATFORM ADMINISTRATOR]\n";
    printAdminDivider();
    std::cout << std::left;
    std::cout << "  " << std::setw(12) << "User ID"  << ": " << userId << "\n";
    std::cout << "  " << std::setw(12) << "Email"    << ": " << email  << "\n";
    std::cout << "  " << std::setw(12) << "Region"   << ": " << region << "\n";
    std::cout << "  " << std::setw(12) << "Role"     << ": " << role   << "\n";
    printAdminDivider();
    std::cout << "  ACCESS: Rankings | Registry | Sharing Network | Delivery Routes\n";
    printAdminDivider();
}
