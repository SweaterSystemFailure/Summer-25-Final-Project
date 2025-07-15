#pragma once
#include <string>
#include <iostream>

namespace gradebook {

class Gradebook;

class User {
protected:
    std::string password;

public:
    virtual ~User() = default;
    virtual void menu(Gradebook& book) = 0;
    void setPassword(const std::string& entry) {
        password = entry;
    }
    std::string getPassword() const {
        return password;
    }
    virtual std::string getRole() const = 0;
    virtual std::string getLastName() const = 0;
    virtual std::string getPassword() const = 0;
};

}
