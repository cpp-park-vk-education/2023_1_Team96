#pragma once

#include <string>

class Player {
   private:
    std::string name_;

   public:
    Player(std::string name) : name_(name) {}

    inline std::string WhoAmI() const { return name_; }
};
