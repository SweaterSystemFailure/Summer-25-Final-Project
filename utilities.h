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

    /**
     * @brief Validates string input from the user.
     *
     * Re-prompts the user until valid, non-empty input is entered.
     *
     * @param prompt The message to display before input.
     * @return A validated, trimmed string.
     */
    std::string stringValidator(const std::string& prompt);

    /**
     * @brief Validates numeric input from the user within a specified range.
     *
     * Re-prompts until a number in the range [min, max] is entered.
     *
     * @tparam T The numeric type (int, float, etc.)
     * @param prompt The message to display before input.
     * @param min The minimum acceptable value.
     * @param max The maximum acceptable value.
     * @return A validated number of type T within the range.
     */
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

    /**
     * @brief Checks if a password meets strength requirements.
     *
     * Password strength rules (typically):
     * - Minimum length
     * - Contains uppercase and lowercase letters
     * - Contains digits and/or special characters
     *
     * @param password The password to evaluate.
     * @return true if strong, false otherwise.
     */
    bool isStrongPassword(const std::string& password);

    /**
     * @brief Attempts to log in a user (unique_ptr overload).
     *
     * Prompts the user for identifying information (ID or name),
     * validates the password, and returns a smart pointer to the User.
     *
     * @tparam T The user type (Student, Teacher, Administrator).
     * @param users A vector of unique_ptrs to the user objects.
     * @param gradebook Reference to the Gradebook object (used for autosave).
     * @return A unique_ptr to the matched User, or nullptr on failure.
     */
    template<typename T>
    std::unique_ptr<gradebook::User, std::function<void(gradebook::User*)>>
        attemptLogin(std::vector<std::unique_ptr<T>>& users, gradebook::Gradebook& gradebook) {
        using namespace gradebook;

        if (users.empty()) {
            std::cout << "No users of this type are registered.\n";
            return nullptr;
        }

        T* userPtr = nullptr;
        if constexpr (std::is_same_v<T, Student>) {
            unsigned id = numericValidator<unsigned>("Enter your Student ID: ", 1, 999999);
            for (auto& studentPtr : users) {
                if (studentPtr->getID() == id) {
                    userPtr = studentPtr.get();
                    break;
                }
            }
        }
        else {
            std::string first = stringValidator("Enter your first name: ");
            std::string last = stringValidator("Enter your last name: ");
            for (auto& uPtr : users) {
                if constexpr (std::is_same_v<T, Teacher> || std::is_same_v<T, Administrator>) {
                    if (uPtr->getFirstName() == first && uPtr->getLastName() == last) {
                        userPtr = uPtr.get();
                        break;
                    }
                }
            }
        }

        if (!userPtr) {
            std::cout << "User not found.\n";
            return nullptr;
        }

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
            [](User*) {}
        );
    }

    /**
     * @brief Attempts to log in a user (raw vector overload).
     *
     * Matches a user by ID or name, performs password authentication,
     * and returns a smart pointer to a User.
     *
     * @tparam T The user type (Student, Teacher, Administrator).
     * @param users A vector of raw user objects.
     * @param gradebook Reference to the Gradebook object (used for autosave).
     * @return A unique_ptr to the matched User, or nullptr on failure.
     */
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
                if constexpr (std::is_same_v<T, Teacher> || std::is_same_v<T, Administrator>) {
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
            [](User*) {}
        );
    }

    /**
     * @brief Asks the user to confirm an action.
     *
     * Prompts with a yes/no question and custom messages for each case.
     *
     * @param prompt The yes/no question to ask.
     * @param yesPrompt Message shown if user answers "yes".
     * @param noPrompt Message shown if user answers "no".
     * @return true if the user confirms (yes), false otherwise.
     */
    bool userCheck(const std::string& prompt, const std::string& yesPrompt, const std::string& noPrompt);

    /**
     * @brief Checks whether a file exists on disk.
     *
     * @param filename Name or path of the file to check.
     * @return true if file exists, false otherwise.
     */
    bool fileExists(const std::string& filename);

    /**
     * @brief Displays the initial menu and handles login/registration logic.
     *
     * @param gradebook Reference to the Gradebook object for access to all data.
     */
    void welcomeMenu(Gradebook& gradebook);

    /**
     * @brief Displays the closing message and handles autosave or shutdown tasks.
     *
     * @param gradebook Reference to the Gradebook object for final processing.
     */
    void closeMenu(Gradebook& gradebook);
}