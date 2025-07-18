#include "utilities.h"
#include <vector>
#include <string>
#include <iostream>
#include <limits>
#include <cctype>
#include "Gradebook.h"

namespace gradebook {
	std::string stringValidator(const std::string& prompt) {
		std::string userInput;
		while (true) {
			std::cout << prompt;
			std::getline(std::cin, userInput);

			userInput.erase(0, userInput.find_first_not_of(" \t\n\r\f\v"));
			userInput.erase(userInput.find_last_not_of(" \t\n\r\f\v") + 1);

			if (userInput.empty()) {
				std::cout << "Invalid input. This field can't be empty or just whitespace. Try again:\n";
			}
			else {
				return userInput;
			}
		}
	}

	char charValidator(const std::string& prompt, const std::vector<char>& validOptions) {
		char input;
		while (true) {
			std::cout << prompt << std::endl;
			std::cin >> input;
			input = static_cast<char>(std::tolower(static_cast<unsigned char>(input)));
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

			for (char option : validOptions) {
				if (static_cast<char>(std::tolower(static_cast<unsigned char>(option))) == input) {
					return input;
				}
			}

			std::cout << "Invalid input. Please enter one of the following: ";
			for (char option : validOptions) {
				std::cout << option << " ";
			}
			std::cout << std::endl;
		}
	}

	//user password validators
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
			std::cout << "Invalid password. Passwords must include at least 8 characters, a mix of upper and lower case letters, numbers, and special symbols.\n";
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

	//basic menus
	void welcomeMenu(Gradebook& gradebook) {
		std::cout << "Welcome to Gradebook!" << std::endl;

		// Load binary if available
		if (/* !binaryLoaded() */ false) {  // replace with actual check
			std::cout << "No saved school could be found. Let's set up a new school.\n";
			gradebook.createSchool();
		}

		while (true) {
			std::cout << "\nPlease select a login type:\n";
			std::cout << "1. Administrator." << std::endl;
			std::cout << "2. Teacher." << std::endl;
			std::cout << "3. Student." << std::endl;
			std::cout << "4. Exit." << std::endl;

			int choice = numericValidator("Choose an option [1-4]: ", 1, 4);

			std::unique_ptr<User> user = nullptr;

			std::unique_ptr<User> user = nullptr;
			switch (choice) {
			case 1: user = attemptLogin<Administrator>(gradebook.getAdmins()); break;
			case 2: user = attemptLogin<Teacher>(gradebook.getTeachers()); break;
			case 3: user = attemptLogin<Student>(gradebook.getStudents()); break;
			case 4: closeMenu(); break;
			}

			if (user) {
				std::cout << "Login successful. Welcome, "
					<< user->getRole() << " "
					<< static_cast<std::string>(user->getFirstName()) << " "
					<< user->getLastName()
					<< "Launching menu." << std::endl << std::endl;
			}
			else {
				std::cout << "Login failed. Please try again." << std::endl;
			}

		}
	}
	void closeMenu(Gradebook& gradebook) {
		if (userCheck("Would you like to save before exiting? [Y/N]",
			"School saved! Exiting.",
			"Exiting without saving.")) {
			gradebook.serializeAndSave();
		}
		else {
			gradebook.clearCachedData();
		}
	}
}