// modules/LicenseRegistry.h
// Purpose: Header file defining LicenseInfo and LicenseRegistry classes, representing copyright license mappings via a Hash Map.

#pragma once

#include <string>
#include <unordered_map>
#include <vector>

/**
 * @struct LicenseInfo
 * @brief Stores ownership details, key status, and type of a digital media license.
 */
struct LicenseInfo {
    std::string licenseKey;  // Unique license key string (e.g., MIT-AJ-002)
    std::string ownerName;   // Name of the license holder/owner
    std::string ownerId;     // Unique user ID of the owner
    std::string type;        // License type classification (e.g., MIT, CC, Proprietary)
    std::string expiry;      // Expiration date string or "NEVER"
    bool        isActive;    // Status flag: true if valid, false if revoked

    /**
     * @brief Constructs a default empty LicenseInfo struct.
     */
    LicenseInfo();

    /**
     * @brief Constructs a LicenseInfo struct initialized with parameters.
     */
    LicenseInfo(std::string key,
                std::string ownerName,
                std::string ownerId,
                std::string type,
                std::string expiry,
                bool        isActive = true);

    /**
     * @brief Displays the formatted license details on the console.
     */
    void display() const;
};

/**
 * @class LicenseRegistry
 * @brief Manages platform license keys using a Hash Map look-up table.
 *
 * Implements register, verify, revoke, and remove capabilities for licenses,
 * enabling O(1) verify check operations.
 */
class LicenseRegistry {
private:
    std::unordered_map<std::string, LicenseInfo> registry; // Hash map storing lowercase key to LicenseInfo mapping

public:
    LicenseRegistry() = default;

    /**
     * @brief Registers a license in the mapping. Fails if the key already exists.
     * @param info The LicenseInfo object.
     * @return True if registration was successful.
     */
    bool registerLicense(const LicenseInfo& info);

    /**
     * @brief Revokes a license, turning isActive to false.
     * @param licenseKey Key of the license to revoke.
     * @return True if license was found and successfully revoked.
     */
    bool revokeLicense(const std::string& licenseKey);

    /**
     * @brief Deletes a license completely from the map.
     * @param licenseKey Key of the license to erase.
     * @return True if license was found and deleted.
     */
    bool removeLicense(const std::string& licenseKey);

    /**
     * @brief Instantly verifies whether a license exists and is active.
     * @return True if active.
     */
    bool verify(const std::string& licenseKey) const;

    /**
     * @brief Returns true if a license key exists in the registry (even if revoked).
     */
    bool exists(const std::string& licenseKey) const;

    /**
     * @brief Fetches the full LicenseInfo structure associated with a key.
     */
    const LicenseInfo& getInfo(const std::string& licenseKey) const;

    /**
     * @brief Returns total count of licenses stored in the registry.
     */
    int  getCount()  const;

    /**
     * @brief Returns count of active (valid) licenses in the registry.
     */
    int  getActiveCount() const;

    /**
     * @brief Displays all license records in the registry.
     */
    void displayAll()     const;

    /**
     * @brief Displays only active license records.
     */
    void displayActive()  const;

    /**
     * @brief Displays only revoked/inactive license records.
     */
    void displayRevoked() const;
};
