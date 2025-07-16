#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <limits>
#include "User.h"

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
	
	template<typename T>
	std::unique_ptr<User> attemptLogin(std::vector<T>& users) {
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
			for (auto& user : users) {
				if constexpr (std::is_same_v<T, Teacher>) {
					if (user.getTeacherFirstName() == first && user.getTeacherLastName() == last)
						userPtr = &user;
				}
				else if constexpr (std::is_same_v<T, Administrator>) {
					if (user.getFirstName() == first && user.getLastName() == last)
						userPtr = &user;
				}
				if (userPtr) break;
			}
		}

		if (!userPtr) {
			std::cout << "User not found.\n";
			return nullptr;
		}

		if (!handlePassword(*userPtr)) {
			return nullptr;
		}
		return std::unique_ptr<User>(userPtr, [](User*) {});
	}


	//user verification check
	bool userCheck(const std::string& prompt, const std::string& yesPrompt, const std::string& noPrompt);

	//basic menus
	void welcomeMenu(Gradebook& gradebook);
	void closeMenu(Gradebook& gradebook);
}