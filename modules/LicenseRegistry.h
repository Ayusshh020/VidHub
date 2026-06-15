#pragma once

#include <string>
#include <unordered_map>
#include <vector>

// =============================================================
//  LicenseRegistry — Copyright verification system for VidHub.
//
//  DSA: Hash Map (std::unordered_map)
//  Key  : licenseKey (string) — e.g. "MIT-AJ-002"
//  Value: LicenseInfo (struct) — owner, type, expiry status
//
//  Why Hash Map?
//    License keys are unique strings. We only ever need to:
//      1. Check if a key EXISTS          → O(1) average
//      2. INSERT a new key on approval   → O(1) average
//      3. REVOKE a key on violation      → O(1) average
//    Linear scan (the current pain point) is O(n) per check.
//    Hash map eliminates that entirely.
//
//  Owned by: ConsoleUI (one global instance for the platform)
//  Used by:  Creator upload flow, Admin inspection panel
// =============================================================

// ── License metadata stored per key ──────────────────────────
struct LicenseInfo {
    std::string licenseKey;   // e.g. "MIT-AJ-002"
    std::string ownerName;    // Creator who holds this license
    std::string ownerId;      // Creator's userId
    std::string type;         // "MIT", "CC", "Proprietary", etc.
    std::string expiry;       // "2026-06-01" or "NEVER"
    bool        isActive;     // false = revoked

    // Default constructor — needed by unordered_map operator[]
    LicenseInfo();

    LicenseInfo(std::string key,
                std::string ownerName,
                std::string ownerId,
                std::string type,
                std::string expiry,
                bool        isActive = true);

    void display() const;     // Print all fields in a formatted block
};

// ─────────────────────────────────────────────────────────────

class LicenseRegistry {
private:
    std::unordered_map<std::string, LicenseInfo> registry;
    // Key   = licenseKey string
    // Value = LicenseInfo struct

public:
    // ── Constructor ───────────────────────────────────────────
    LicenseRegistry() = default;

    // ── Write Operations ──────────────────────────────────────

    // Register a new license in the registry.
    // Returns true  → successfully added.
    // Returns false → licenseKey already exists (duplicate).
    bool registerLicense(const LicenseInfo& info);

    // Mark a license as revoked (isActive = false).
    // The entry is kept in the map for audit purposes.
    // Returns true  → found and revoked.
    // Returns false → key not found.
    bool revokeLicense(const std::string& licenseKey);

    // Permanently remove a license entry from the registry.
    // Use only for cleanup — prefer revoke() for audit trail.
    bool removeLicense(const std::string& licenseKey);

    // ── Read / Verification Operations ────────────────────────

    // The core verification call — used during creator upload.
    // Returns true  → key exists AND isActive == true.
    // Returns false → key not found OR has been revoked.
    // Time complexity: O(1) average.
    bool verify(const std::string& licenseKey) const;

    // Check existence only (regardless of active status).
    bool exists(const std::string& licenseKey) const;

    // Retrieve full LicenseInfo for a given key.
    // Caller must check exists() first.
    const LicenseInfo& getInfo(const std::string& licenseKey) const;

    // ── Registry-Wide Operations ──────────────────────────────

    int  getCount()  const;    // Total entries (active + revoked)
    int  getActiveCount() const; // Only active licenses

    // ── Display ───────────────────────────────────────────────
    void displayAll()     const;  // Print every entry in the registry
    void displayActive()  const;  // Print only active licenses
    void displayRevoked() const;  // Print only revoked licenses
};
