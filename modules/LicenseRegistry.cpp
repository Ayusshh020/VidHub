#include "LicenseRegistry.h"
#include "../utils/Helpers.h"

#include <iostream>
#include <iomanip>
#include <string>

// ─────────────────────────────────────────────────────────────
//  File-local helpers
// ─────────────────────────────────────────────────────────────

static void printLRDivider(char c = '-') {
    std::cout << "  " << std::string(62, c) << "\n";
}

// =============================================================
//  LicenseInfo — STRUCT IMPLEMENTATION
// =============================================================

LicenseInfo::LicenseInfo()
    : licenseKey(""),
      ownerName(""),
      ownerId(""),
      type(""),
      expiry(""),
      isActive(false)
{}

LicenseInfo::LicenseInfo(std::string key,
                         std::string ownerName,
                         std::string ownerId,
                         std::string type,
                         std::string expiry,
                         bool        isActive)
    : licenseKey(key),
      ownerName(ownerName),
      ownerId(ownerId),
      type(type),
      expiry(expiry),
      isActive(isActive)
{}

// Formatted single-entry display used in displayAll() and getInfo()
//
// Output:
//   ──────────────────────────────────────────────────────────────
//   Key    : MIT-AJ-002
//   Owner  : Arjun Dev  (USR-003)
//   Type   : MIT
//   Expiry : 2026-06-01
//   Status : ✔ ACTIVE
//   ──────────────────────────────────────────────────────────────
void LicenseInfo::display() const {
    printLRDivider();
    std::cout << std::left;
    std::cout << "  " << std::setw(10) << "Key"    << ": " << licenseKey  << "\n";
    std::cout << "  " << std::setw(10) << "Owner"  << ": " << ownerName
                                                   << "  (" << ownerId << ")\n";
    std::cout << "  " << std::setw(10) << "Type"   << ": " << type        << "\n";
    std::cout << "  " << std::setw(10) << "Expiry" << ": " << expiry      << "\n";
    std::cout << "  " << std::setw(10) << "Status" << ": "
              << (isActive ? "\xE2\x9C\x94 ACTIVE" : "\xE2\x9C\x98 REVOKED") << "\n";
}

// =============================================================
//  LicenseRegistry — WRITE OPERATIONS
// =============================================================

// Register a brand-new license.
// Duplicate check: unordered_map::count() is O(1).
// Returns false immediately if the key already exists.
bool LicenseRegistry::registerLicense(const LicenseInfo& info) {
    std::string keyLower = toLower(info.licenseKey);
    if (registry.count(keyLower) > 0)
        return false;
    registry[keyLower] = info;
    return true;
}

// Revoke a license — marks isActive = false but keeps the record.
// Preserves audit trail. Does NOT erase from the map.
bool LicenseRegistry::revokeLicense(const std::string& licenseKey) {
    auto it = registry.find(toLower(licenseKey));
    if (it == registry.end())
        return false;
    it->second.isActive = false;
    return true;
}

// Hard delete — permanently removes the entry.
// Use sparingly; revoke() is preferred for accountability.
bool LicenseRegistry::removeLicense(const std::string& licenseKey) {
    return registry.erase(toLower(licenseKey)) > 0;
}

// =============================================================
//  LicenseRegistry — READ / VERIFICATION OPERATIONS
// =============================================================

// The primary verification gate for creator uploads.
//
// A license is VALID only if:
//   (a) it exists in the registry, AND
//   (b) isActive == true (not revoked)
//
// unordered_map::find() is O(1) average — no scan needed.
bool LicenseRegistry::verify(const std::string& licenseKey) const {
    auto it = registry.find(toLower(licenseKey));
    if (it == registry.end())
        return false;
    return it->second.isActive;
}

// Existence check regardless of active status.
// Used by: admin inspection, duplicate prevention.
bool LicenseRegistry::exists(const std::string& licenseKey) const {
    return registry.count(toLower(licenseKey)) > 0;
}

// Full info retrieval — caller MUST check exists() first.
const LicenseInfo& LicenseRegistry::getInfo(const std::string& licenseKey) const {
    return registry.at(toLower(licenseKey));
}

// =============================================================
//  LicenseRegistry — REGISTRY-WIDE OPERATIONS
// =============================================================

int LicenseRegistry::getCount() const {
    return static_cast<int>(registry.size());
}

int LicenseRegistry::getActiveCount() const {
    int count = 0;
    for (const auto& pair : registry)
        if (pair.second.isActive) ++count;
    return count;
}

// =============================================================
//  LicenseRegistry — DISPLAY
// =============================================================

// Print every entry — used by Admin → "Inspect License Registry"
void LicenseRegistry::displayAll() const {
    std::cout << "\n";
    printLRDivider('=');
    std::cout << "  \xF0\x9F\x94\x92  License Registry  ("
              << getCount()       << " total, "
              << getActiveCount() << " active)\n";
    printLRDivider('=');

    if (registry.empty()) {
        std::cout << "  Registry is empty.\n";
        printLRDivider('=');
        std::cout << "\n";
        return;
    }

    for (const auto& pair : registry)
        pair.second.display();

    printLRDivider('=');
    std::cout << "\n";
}

// Print only active licenses
void LicenseRegistry::displayActive() const {
    std::cout << "\n";
    printLRDivider('=');
    std::cout << "  \xE2\x9C\x94  Active Licenses  (" << getActiveCount() << ")\n";
    printLRDivider('=');

    bool any = false;
    for (const auto& pair : registry) {
        if (pair.second.isActive) {
            pair.second.display();
            any = true;
        }
    }
    if (!any)
        std::cout << "  No active licenses.\n";

    printLRDivider('=');
    std::cout << "\n";
}

// Print only revoked licenses — useful for admin audit
void LicenseRegistry::displayRevoked() const {
    std::cout << "\n";
    printLRDivider('=');
    std::cout << "  \xE2\x9C\x98  Revoked Licenses\n";
    printLRDivider('=');

    bool any = false;
    for (const auto& pair : registry) {
        if (!pair.second.isActive) {
            pair.second.display();
            any = true;
        }
    }
    if (!any)
        std::cout << "  No revoked licenses.\n";

    printLRDivider('=');
    std::cout << "\n";
}
