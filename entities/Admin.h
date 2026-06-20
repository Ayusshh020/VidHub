#pragma once

#include "User.h"
#include <string>

class Admin : public User {
public:
    Admin();

    Admin(std::string userId,
          std::string name,
          std::string email,
          std::string region);

    void displaySummary() const;
    void displayFull() const override;
};
