// entities/Admin.h
// Purpose: Header file defining the Admin class, inheriting from User, representing platform administrators.

#pragma once

#include "User.h"
#include <string>

/**
 * @class Admin
 * @brief Represents an administrator user.
 *
 * Inherits from User and overrides display functions. Administrators have special
 * system permissions to oversee the catalog, licenses, network analysis, and route simulations.
 */
class Admin : public User {
public:
    /**
     * @brief Constructs a default Admin object.
     */
    Admin();

    /**
     * @brief Constructs an Admin object initialized with profile and credentials attributes.
     */
    Admin(std::string userId,
          std::string name,
          std::string email,
          std::string region);

    /**
     * @brief Displays administrator user stats in list views.
     */
    void displaySummary() const;

    /**
     * @brief Overrides the base displayFull method to show detailed system status information.
     */
    void displayFull() const override;
};
