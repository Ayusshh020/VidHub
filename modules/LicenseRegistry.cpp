// modules/LicenseRegistry.cpp
// Purpose: Implementation of LicenseRegistry and LicenseInfo methods, providing O(1) hash map lookup capabilities.

#include "LicenseRegistry.h"
#include "../utils/Helpers.h"

#include <iostream>
#include <iomanip>
#include <string>

// Helper function to print a uniform divider line for the license registry outputs
static void printLRDivider(char c = '-') {
    std::cout << "  " << std::string(62, c) << "\n";
}

// Default constructor initializing empty/inactive license state
LicenseInfo::LicenseInfo()
    : licenseKey(""),
      ownerName(""),
      ownerId(""),
      type(""),
      expiry(""),
      isActive(false)
{}

// Parameterized constructor initializing a license with explicitly defined fields
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

// Prints individual license details in a structured console layout
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

// Registers a new license in the hash map. Uses lowercase keys for case-insensitive indexing.
bool LicenseRegistry::registerLicense(const LicenseInfo& info) {
    std::string keyLower = toLower(info.licenseKey);
    // Return false if key is already registered to avoid duplicates
    if (registry.count(keyLower) > 0)
        return false;
    registry[keyLower] = info;
    return true;
}

// Revokes a license (marks isActive as false) while keeping it in the registry (audit trail)
bool LicenseRegistry::revokeLicense(const std::string& licenseKey) {
    auto it = registry.find(toLower(licenseKey));
    if (it == registry.end())
        return false;
    it->second.isActive = false;
    return true;
}

// Hard erases a license record from the hash map lookup registry
bool LicenseRegistry::removeLicense(const std::string& licenseKey) {
    return registry.erase(toLower(licenseKey)) > 0;
}

// Verifies a license's validity: must exist in registry map AND be active
bool LicenseRegistry::verify(const std::string& licenseKey) const {
    auto it = registry.find(toLower(licenseKey));
    if (it == registry.end())
        return false;
    return it->second.isActive;
}

// Checks if a license key exists anywhere in the registry (even if revoked)
bool LicenseRegistry::exists(const std::string& licenseKey) const {
    return registry.count(toLower(licenseKey)) > 0;
}

// Returns a reference to the LicenseInfo associated with the key
const LicenseInfo& LicenseRegistry::getInfo(const std::string& licenseKey) const {
    return registry.at(toLower(licenseKey));
}

// Returns count of all license entries in the hash map
int LicenseRegistry::getCount() const {
    return static_cast<int>(registry.size());
}

// Counts and returns the number of active licenses currently in the hash map
int LicenseRegistry::getActiveCount() const {
    int count = 0;
    for (const auto& pair : registry)
        if (pair.second.isActive) ++count;
    return count;
}

// Displays all records in the registry
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

// Displays only active license records
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

// Displays only revoked license records
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
