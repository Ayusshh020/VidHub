#pragma once
// Note: Comments are only for understanding—do not modify the code itself.

#include <string>
#include <unordered_map>
#include <vector>

struct LicenseInfo {
    std::string licenseKey;
    std::string ownerName;
    std::string ownerId;
    std::string type;
    std::string expiry;
    bool        isActive;

    LicenseInfo();

    LicenseInfo(std::string key,
                std::string ownerName,
                std::string ownerId,
                std::string type,
                std::string expiry,
                bool        isActive = true);

    void display() const;
};

class LicenseRegistry {
private:
    std::unordered_map<std::string, LicenseInfo> registry;

public:
    LicenseRegistry() = default;

    bool registerLicense(const LicenseInfo& info);
    bool revokeLicense(const std::string& licenseKey);
    bool removeLicense(const std::string& licenseKey);

    bool verify(const std::string& licenseKey) const;
    bool exists(const std::string& licenseKey) const;
    const LicenseInfo& getInfo(const std::string& licenseKey) const;

    int  getCount()  const;
    int  getActiveCount() const;

    void displayAll()     const;
    void displayActive()  const;
    void displayRevoked() const;
};
