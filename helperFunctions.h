#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

#include <string>
#include <vector>
#include <iostream>
#include <limits>

namespace gradebook {

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

	std::string stringValidator(const std::string& prompt);
	bool userCheck(const std::string& prompt, const std::string& yesPrompt, const std::string& noPrompt);
	char charValidator(const std::string& prompt, const std::vector<char>& validOptions);
}

#endif