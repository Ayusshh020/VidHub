#include "User.h"

#include <iostream>
#include <iomanip>   // std::setw, std::left
#include <string>

// ─────────────────────────────────────────────────────────────
//  File-local helpers (not exposed in the header)
// ─────────────────────────────────────────────────────────────

static void printUserDivider() {
    std::cout << "  " << std::string(50, '-') << "\n";
}

// =============================================================
//  CONSTRUCTORS
// =============================================================

// Default constructor — empty user. Used by STL containers
// and when building User arrays in DummyData before filling.
User::User()
    : userId(""),
      name(""),
      email(""),
      region(""),
      role("viewer")
{}

// Parameterized constructor — primary constructor used by
// DummyData to build pre-populated test users.
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

// =============================================================
//  DISPLAY
// =============================================================

// One-line summary — used in "Select a user" menus and
// session picker screens.
//
// Output:
//   [USR-001] Riya Sharma           | Mumbai    | viewer
void User::displaySummary() const {
    std::cout << "  ["
              << std::left << std::setw(7)  << userId << "] "
              << std::left << std::setw(22) << name
              << " | " << std::left << std::setw(12) << region
              << " | " << role
              << "\n";
}

// Full profile block — displayed on "My Profile" screen.
//
// Output:
//   --------------------------------------------------
//   👤  Riya Sharma
//   --------------------------------------------------
//     User ID  : USR-001
//     Email    : riya@vidhub.com
//     Region   : Mumbai
//     Role     : viewer
//   --------------------------------------------------
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

// =============================================================
//  GETTERS
// =============================================================

std::string User::getUserId() const { return userId; }
std::string User::getName()   const { return name;   }
std::string User::getRegion() const { return region; }
std::string User::getRole()   const { return role;   }
