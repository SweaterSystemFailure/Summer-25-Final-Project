#include "utilities.h"
#include <vector>
#include <functional>
#include <fstream>
#include <string>
#include <iostream>
#include <limits>
#include <cctype>
#include "Gradebook.h"

namespace gradebook {

    /**
     * @brief Prompts the user for a non-empty string input with trimming.
     *
     * @param prompt The prompt message displayed to the user.
     * @return A validated, non-empty trimmed string input from the user.
     */
    std::string stringValidator(const std::string& prompt) {
        std::string userInput;
        while (true) {
            std::cout << prompt;
            std::getline(std::cin, userInput);

            userInput.erase(0, userInput.find_first_not_of(" \t\n\r\f\v"));
            userInput.erase(userInput.find_last_not_of(" \t\n\r\f\v") + 1);

            if (userInput.empty()) {
                std::cout << "Invalid input. This field can't be empty. Try again:\n";
            }
            else {
                return userInput;
            }
        }
    }

    /**
     * @brief Checks if a password is strong based on specific security criteria.
     *
     * A strong password must:
     * - Be at least 8 characters long
     * - Include uppercase and lowercase letters
     * - Include at least one digit
     * - Include at least one special character
     *
     * Prompts the user to re-enter the password to confirm.
     *
     * @param password The password to validate.
     * @return true if the password is strong and confirmed, false otherwise.
     */
    bool isStrongPassword(const std::string& password) {
        bool hasLength = password.length() >= 8;
        bool hasUpper = false, hasLower = false, hasDigit = false, hasSymbol = false;

        for (char ch : password) {
            if (std::isupper(ch)) hasUpper = true;
            else if (std::islower(ch)) hasLower = true;
            else if (std::isdigit(ch)) hasDigit = true;
            else hasSymbol = true;
        }

        if (!hasLength || !hasUpper || !hasLower || !hasDigit || !hasSymbol) {
            std::cout << "Invalid password. Passwords must include at least 8 characters, "
                "a mix of upper and lower case letters, numbers, and special symbols.\n";
            return false;
        }

        std::string rematchChallenge;
        do {
            rematchChallenge = stringValidator("Please reenter your password: ");
            if (rematchChallenge != password) {
                std::cout << "Passwords do not match. Please try again.\n";
            }
        } while (rematchChallenge != password);

        return true;
    }

    /**
     * @brief Prompts the user with a yes/no question and handles input validation.
     *
     * @param prompt The question to ask the user.
     * @param yesPrompt Message to display if the user selects 'y'.
     * @param noPrompt Message to display if the user selects 'n'.
     * @return true if user selects 'y', false if 'n'.
     */
    bool userCheck(const std::string& prompt, const std::string& yesPrompt, const std::string& noPrompt) {
        char choice;
        while (true) {
            std::cout << prompt << std::endl;
            std::cin >> choice;
            choice = static_cast<char>(std::tolower(static_cast<unsigned char>(choice)));
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (choice == 'y') {
                std::cout << yesPrompt << std::endl << std::endl;
                return true;
            }
            else if (choice == 'n') {
                std::cout << noPrompt << std::endl << std::endl;
                return false;
            }
            else {
                std::cout << "Invalid input. Please enter y or n." << std::endl << std::endl;
            }
        }
    }

    /**
     * @brief Checks if a file exists on disk.
     *
     * @param filename The name of the file to check.
     * @return true if the file exists and can be opened, false otherwise.
     */
    bool fileExists(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);
        return file.good();
    }

    /**
     * @brief Displays the initial welcome menu and handles login/setup flow.
     *
     * If a saved `gradebook.dat` file is found, it loads school data.
     * If no file is found, it initiates school setup.
     * Then repeatedly prompts user to log in as an Administrator, Teacher, or Student.
     *
     * @param gradebook Reference to the Gradebook instance.
     */
    void welcomeMenu(Gradebook& gradebook) {
        std::cout << "Welcome to Gradebook!" << std::endl << std::endl;

        const std::string filename = "gradebook.dat";

        if (fileExists(filename)) {
            std::cout << "Loading saved data..." << std::endl;
            gradebook.deserializeAndLoad();
        }
        else {
            std::cout << "No saved school found. Let's set up a new school." << std::endl;
            gradebook.createSchool();
            gradebook.serializeAndSave();
        }

        std::unique_ptr<User, std::function<void(User*)>> user = nullptr;

        while (true) {
            std::cout << std::endl << "Please select a login type:" << std::endl;
            std::cout << "  1. Administrator" << std::endl;
            std::cout << "  2. Teacher" << std::endl;
            std::cout << "  3. Student" << std::endl;
            std::cout << "  4. Exit" << std::endl;

            int choice = numericValidator("Choose an option [1-4]: ", 1, 4);

            switch (choice) {
            case 1:
                user = attemptLogin<Administrator>(gradebook.getSchool(), gradebook);
                break;
            case 2:
                user = attemptLogin<Teacher>(gradebook.getTeachers(), gradebook);
                break;
            case 3:
                user = attemptLogin<Student>(gradebook.getStudents(), gradebook);
                break;
            case 4:
                closeMenu(gradebook);
                return;
            default:
                user = nullptr;
                break;
            }

            if (user) {
                std::cout << std::endl << "Login successful. Welcome, "
                    << user->getRole() << " "
                    << user->getFirstName() << " "
                    << user->getLastName() << "." << std::endl << std::endl;

                user->menu(gradebook);
            }
            else {
                std::cout << "Login failed. Please try again." << std::endl;
            }
        }
    }

    /**
     * @brief Displays the close menu and prompts user to save before exiting.
     *
     * If the user confirms, saves all data to disk before exiting.
     * Otherwise, clears cached data and exits without saving.
     *
     * @param gradebook Reference to the Gradebook instance.
     */
    void closeMenu(Gradebook& gradebook) {
        if (userCheck("Would you like to save before exiting? [Y/N]",
            "School saved! Exiting.",
            "Exiting without saving.")) {
            gradebook.serializeAndSave();
            std::exit(0);
        }
        else {
            gradebook.clearCachedData();
            std::exit(0);
        }
    }
}