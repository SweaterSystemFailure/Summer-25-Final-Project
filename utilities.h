#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <limits>
#include <functional>
#include "Administrator.h"
#include "Teacher.h"
#include "Student.h"
#include "User.h"
#include "Gradebook.h"

namespace gradebook {
    //user character input validators
    std::string stringValidator(const std::string& prompt);
    char charValidator(const std::string& prompt, const std::vector<char>& validOptions);

    template <typename T>
    T numericValidator(const std::string& prompt, T min, T max) {
        T number;
        while (true) {
            std::cout << prompt;
            std::cin >> number;

            if (std::cin.fail()) {
                std::cout << "Enter a valid number." << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            else if (number < min || number > max) {
                std::cout << "Number must be between " << min << " and " << max << "." << std::endl;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            else {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
            }
        }
        return number;
    }
    //user password validators
    bool isStrongPassword(const std::string& password);

    template<typename T>
    std::unique_ptr<gradebook::User, std::function<void(gradebook::User*)>>
        attemptLogin(std::vector<T>& users, gradebook::Gradebook& gradebook) {
        using namespace gradebook;

        if (users.empty()) {
            std::cout << "No users of this type are registered.\n";
            return nullptr;
        }

        T* userPtr = nullptr;
        if constexpr (std::is_same_v<T, Student>) {
            unsigned id = numericValidator<unsigned>("Enter your Student ID: ", 1, 999999);
            for (auto& student : users) {
                if (student.getID() == id) {
                    userPtr = &student;
                    break;
                }
            }
        }
        else {
            std::string first = stringValidator("Enter your first name: ");
            std::string last = stringValidator("Enter your last name: ");
            for (auto& u : users) {
                if constexpr (std::is_same_v<T, Teacher> ||
                    std::is_same_v<T, Administrator>) {
                    if (u.getFirstName() == first && u.getLastName() == last) {
                        userPtr = &u;
                        break;
                    }
                }
            }
        }

        if (!userPtr) {
            std::cout << "User not found.\n";
            return nullptr;
        }

        // Inline password handler
        auto doPasswordFlow = [&](User& u) {
            std::string entered;
            if (u.getPassword().empty()) {
                std::cout << "No password set for your account. Please create one now.\n";
                do {
                    entered = stringValidator("Enter new password: ");
                } while (!isStrongPassword(entered));
                u.setPassword(entered);
                std::cout << "Password set successfully.\n";
                if (gradebook.isAutosaveEnabled()) {
                    gradebook.serializeAndSave();
                }
            }
            else {
                entered = stringValidator("Enter your password: ");
                if (entered != u.getPassword()) {
                    std::cout << "Incorrect password.\n";
                    return false;
                }
                std::cout << "Password accepted.\n";
            }
            return true;
            };

        if (!doPasswordFlow(*userPtr)) {
            return nullptr;
        }

        return std::unique_ptr<User, std::function<void(User*)>>(
            static_cast<User*>(userPtr),
            [](User*) {}  // no-op deleter, since we don't own the object
        );
    }

    //user verification check
    bool userCheck(const std::string& prompt, const std::string& yesPrompt, const std::string& noPrompt);

    //basic menus
    bool fileExists(const std::string& filename);
    void welcomeMenu(Gradebook& gradebook);
    void closeMenu(Gradebook& gradebook);
}