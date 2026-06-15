#pragma once

#include <string>

// =============================================================
//  User — Represents a viewer on the VidHub platform.
//
//  This is a pure data class — it holds profile information.
//  WatchHistory and RecommendationQueue are NOT members here
//  to preserve the entities → modules dependency rule.
//  They are managed externally at the session level (ConsoleUI).
//
//  Creator and Admin inherit from User.
// =============================================================

class User {
public:
    // ── Identity ─────────────────────────────────────────────
    std::string userId;        // Unique identifier  e.g. "USR-001"
    std::string name;          // Display name       e.g. "Riya Sharma"
    std::string email;         // e.g. "riya@vidhub.com"
    std::string region;        // Geographic region  e.g. "Mumbai"

    // ── Role Tag ─────────────────────────────────────────────
    // Used only for menu routing — NOT for security/auth.
    // Values: "viewer", "creator", "admin"
    std::string role;

    // ── Constructors ─────────────────────────────────────────
    User();

    User(std::string userId,
         std::string name,
         std::string email,
         std::string region,
         std::string role = "viewer");

    // ── Display ──────────────────────────────────────────────
    void displaySummary() const;   // One-line: [ID] Name | Region | Role
    virtual void displayFull() const;   // All fields formatted

    // ── Getters ──────────────────────────────────────────────
    std::string getUserId()  const;
    std::string getName()    const;
    std::string getRegion()  const;
    std::string getRole()    const;

    // ── Virtual destructor (required for inheritance) ─────────
    virtual ~User() = default;
};
