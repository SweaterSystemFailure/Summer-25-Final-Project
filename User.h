#pragma once
#include <string>
#include <iostream>

namespace gradebook {

    class Gradebook;

    class User {
    protected:
        std::string firstName;
        std::string lastName;
        std::string password;

    public:
        virtual ~User() = default;

        virtual void setPassword(const std::string& entry) {
            password = entry;
        }

        void setFirstName(const std::string& fn) { firstName = fn; }
        void setLastName(const std::string& ln) { lastName = ln; }

        virtual void menu(Gradebook& gradebook) = 0;
        virtual std::string getRole() const = 0;
        virtual std::string getFirstName() const = 0;
        virtual std::string getLastName() const = 0;
        virtual std::string getPassword() const = 0;
      
    };
}