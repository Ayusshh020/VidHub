#pragma once

#include "User.h"
#include <string>

// =============================================================
//  Admin — A User with platform-wide oversight capabilities.
//
//  Inherits all User fields. Does NOT add data fields — Admin
//  is a role marker, not a data store. The admin's extra
//  capabilities (view rankings, inspect registry, run Dijkstra,
//  analyze sharing network) are unlocked by the ConsoleUI
//  based on the "admin" role tag.
//
//  Keeping Admin lean preserves single-responsibility:
//  the DSA modules own their own data; Admin just queries them.
// =============================================================

class Admin : public User {
public:
    // ── Constructors ─────────────────────────────────────────
    Admin();

    Admin(std::string userId,
          std::string name,
          std::string email,
          std::string region);

    // ── Display (overrides User) ──────────────────────────────
    void displaySummary() const;          // [ID] Name | Admin | Region
    void displayFull()    const override; // All user fields + "ADMIN ACCOUNT" label
};
