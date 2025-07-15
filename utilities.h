#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <limits>

namespace gradebook {
	//user character input validators
	std::string stringValidator(const std::string& prompt);
	char charValidator(const std::string& prompt, const std::vector<char>& validOptions);
	
	template <typename T>
	T numericValidator(const std::string& prompt, T min, T max) {
		T number;
		while (true) {
			std::cout << prompt << std::endl;
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
	
template <typename T>
std::unique_ptr<gradebook::User> attemptLogin(const std::vector<T>& users) {
	using namespace gradebook;

	std::string lastName = stringValidator("Enter your last name: ");
	std::string password = stringValidator("Enter your password: ");

	for (const T& user : users) {
		if (user.getLastName() == lastName && user.getPassword() == password) {
			return std::make_unique<T>(user);  // Copy constructor
		}
	}

	std::cout << "No matching user found.\n";
	return nullptr;
}

	//user verification check
	bool userCheck(const std::string& prompt, const std::string& yesPrompt, const std::string& noPrompt);

	//basic menus
	void welcomeMenu(Gradebook& gradebook);
	void closeMenu();
}