#include "utilities.h"
#include <vector>
#include <string>
#include <iostream>
#include <limits>
#include <cctype>

namespace gradebook {
	//user character input validators
std::string stringValidator(const std::string& prompt) {
    std::string userInput;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, userInput);

        // Trim leading/trailing whitespace
        userInput.erase(0, userInput.find_first_not_of(" \t\n\r\f\v"));
        userInput.erase(userInput.find_last_not_of(" \t\n\r\f\v") + 1);

        if (userInput.empty()) {
            std::cout << "Invalid input. This field can't be empty or just whitespace. Try again:\n";
        } else {
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

	bool isCorrectPassword(const std::string& entry){

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
	void welcomeMenu(){
		std::cout << "Weclome to Gradebook!" <<std::endl;

		//load binary
		if(//!binary loaded){
			std::cout << "No saved school could be found. Let's set up a new school."
		//create new binary
			createAdmin();	
		}
		else{
			std::cout <<"Would you like to: " << std::endl;
			std::cout << "1. Log in as an administrator." << std:: endl;
			std::cout << "2. Log in as a teacher." << std::endl;
			std::cout << "3. Log in as a student." << std::endl;
			std::cout << "4. Exit." << std::endl;
			switch (numericValidator("Please enter the number of the option that you would like to select: [1-9] ", 1, 9)) {
		case 1:
			adminMenu();
			break;
		case 2:
			teacherMenu();
			break;
		case 3:
			studentMenu();
			break;
		case 4:
			closeMenu();
			break;	
		}
		default:
			cout << std::cout << "Please select a valid option and try again." << std::endl;
			welcomeMenu();
	}
	
	void closeMenu(){
		if(userCheck("Would you like to save before exiting?", "School saved! Exiting.", "Exiting without saving.")){
			//savefunction
		}
		else{
			//dump cached containers
		}
	}
}