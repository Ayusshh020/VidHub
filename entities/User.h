#pragma once
// Note: Comments are only for understanding—do not modify the code itself.

#include <string>

class User {
public:
    std::string userId;   // e.g. "USR-001"
    std::string name;     // e.g. "Riya Sharma"
    std::string email;
    std::string region;   // e.g. "Mumbai"
    std::string role;     // viewer, creator, or admin

    User();

    User(std::string userId,
         std::string name,
         std::string email,
         std::string region,
         std::string role = "viewer");

    void displaySummary() const; // One-line summary
    virtual void displayFull() const; // Full profile

    std::string getUserId()  const;
    std::string getName()    const;
    std::string getRegion()  const;
    std::string getRole()    const;

    virtual ~User() = default;
};
