#include "Admin.h"

#include <iostream>
#include <iomanip>
#include <string>

// ─────────────────────────────────────────────────────────────
//  File-local helpers
// ─────────────────────────────────────────────────────────────

static void printAdminDivider() {
    std::cout << "  " << std::string(50, '=') << "\n";
}

// =============================================================
//  CONSTRUCTORS
// =============================================================

// Default constructor.
// Role is hardcoded to "admin" so menus route correctly.
Admin::Admin()
    : User()
{
    role = "admin";
}

// Parameterized constructor — used by DummyData.
// Region is accepted (useful for audit-trail display)
// but role is always locked to "admin".
Admin::Admin(std::string userId,
             std::string name,
             std::string email,
             std::string region)
    : User(userId, name, email, region, "admin")
{}

// =============================================================
//  DISPLAY
// =============================================================

// One-line summary — used in the session picker to identify
// the admin account clearly.
//
// Output:
//   [USR-005] Admin Kumar            | System    | *** ADMIN ***
void Admin::displaySummary() const {
    std::cout << "  ["
              << std::left << std::setw(7)  << userId << "] "
              << std::left << std::setw(22) << name
              << " | " << std::left << std::setw(12) << region
              << " | *** ADMIN ***"
              << "\n";
}

// Full profile block — admin badge is prominent so it's clear
// in any testing scenario which session has elevated access.
//
// Output:
//   ==================================================
//   ⚙   Admin Kumar  [PLATFORM ADMINISTRATOR]
//   ==================================================
//     User ID  : USR-005
//     Email    : admin@vidhub.com
//     Region   : System
//     Role     : admin
//   ==================================================
//   ACCESS: Rankings | Registry | Sharing Network | Delivery Routes
//   ==================================================
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
