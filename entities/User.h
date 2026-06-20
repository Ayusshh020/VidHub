// entities/User.h
// Purpose: Header file defining the User base class, representing platform participants (viewers/creators/admins).

#pragma once

#include <string>

/**
 * @class User
 * @brief Base class representing a registered user on the VidHub platform.
 *
 * Tracks basic user details such as ID, name, email, geographic region, and access role.
 * Serves as the base class for specialized roles (Creator, Admin).
 */
class User {
public:
    std::string userId;   // Unique identifier (e.g., "USR-001")
    std::string name;     // Full name of the user
    std::string email;    // Email address for account identification
    std::string region;   // Location region (e.g., "Mumbai", used for CDN routing calculations)
    std::string role;     // Platform role classification: "viewer", "creator", or "admin"

    /**
     * @brief Constructs a default User object.
     */
    User();

    /**
     * @brief Constructs a User object initialized with core identity details.
     */
    User(std::string userId,
         std::string name,
         std::string email,
         std::string region,
         std::string role = "viewer");

    /**
     * @brief Outputs a single-line summary of user credentials for lists.
     */
    void displaySummary() const;

    /**
     * @brief Displays the full user profile details. Virtual to allow role-specific overrides.
     */
    virtual void displayFull() const;

    // Getters for secure attribute retrieval
    std::string getUserId()  const;
    std::string getName()    const;
    std::string getRegion()  const;
    std::string getRole()    const;

    /**
     * @brief Virtual destructor ensuring proper cleanup in derived classes.
     */
    virtual ~User() = default;
};
