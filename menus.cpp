#include "containers.h"
#include "helperFunctions.h"
#include "menus.h"
#include <iostream>

namespace gradebook {
	void welcome(storage) {
		std::cout << "Welcome to your grade book!" << std::endl;
		std::cout << "Would you like to: " << std::endl;
		std::cout << "1. Create a new classroom." << std::endl;
		std::cout << "2. Load an existing classroom?" << std::endl;

		switch (numericValidator("Please enter the number of the option that you would like to select: [1-2]", 1, 2)) {
		case 1:
			//create classroom function
			break;
		case 2:
			// import file function
			break;
		default:
			std::cout << "It looks like something went wrong. Let's try again." << std::endl;
			welcome(storage);
		}
	}


	void exitMenu() {
		//dump globalStorage containers
		welcome(storage);

		//exit to desktop w/o saving
	}

	void mainMenu(storage) {
		//menu options
		std::cout << "Welcome to the main menu." << std::endl;
		std::cout << "From here you can enter student information and grades." << std::endl;
		std::cout << "Would you like to: " << std::endl;
		std::cout << std::endl;
		std::cout << "1. Enter new student information." << std::endl;
		std::cout << "2. View all students." << std::endl;
		std::cout << "3. Generate an individual student report." << std::endl;
		std::cout << "4. Create new assignments." << std::endl;
		std::cout << "5. View all assignments." << std::endl;
		std::cout << "6. Enter grades for an existing assignments." << std::endl;
		std::cout << "7. Export a report for your whole class to a .csv file" << std::endl;
		std::cout << "8. Save your work." << std::endl;
		std::cout << "9. Exit program." << std::endl;
		std::cout << std::endl;

		//selector for menu options
		switch (numericValidator("Please enter the number of the option that you would like to select: [1-9] ", 1, 9)) {
		case 1:
			addStudent(storage);
			break;
		case 2:
			printAllStudents(storage);
			break;
		case 3:
			printStudent(storage);
			break;
		case 4:
			addAssignment(storage);
			break;
		case 5:
			printAssignments(storage);
			break;
		case 6:
			enterGrades(storage);
			break;
		case 7:
			printCSV(storage);
			break;
		case 8:
			save(storage);
			break;
		case 9:
			exitMenu();
			break;
		default:
			std::cout << "Please select a valid option and try again." << std::endl;
			return mainMenu(storage);
			break;
		}
	}
}
