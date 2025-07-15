#include "Teacher.h"
#include "utilities.h"
#include <iostream>

namespace gradebook {
//Classroom
	//Mutators
	void Teacher::setTitle(const std::string& entry) {
		title = entry;
	}
	void Teacher::setTeacherFirstName(const std::string& entry) {
		teacherFirstName = entry;
	}
	void Teacher::setTeacherLastName(const std::string& entry) {
		teacherLastName = entry;
	}
	void Teacher::setGradeLevel(const unsigned& entry) {
		gradeLevel = entry;
	}
	void Teacher::setTeacherPassword(const std::string& entry){
		teacherPassword = entry;
	}

	//Accessors
	std::string Teacher::getTitle() const {
		return title;
	}
	std::string Teacher::getTeacherFirstName() const {
		return teacherFirstName;
	}
	std::string Teacher::getTeacherLastName() const {
		return teacherLastName;
	}
	unsigned Teacher::getGradeLevel() const {
		return gradeLevel;
	}
	std::string Teacher::getTeacherPassword() const{
		return teacherPassword;
	}

	//Print Function
	void Teacher::printClassroom(Teacher& myClassroom) {
		std::cout << "This is " << myClassroom.getTitle() << " " << myClassroom.getTeacherFirstName() << " "
			<< myClassroom.getTeacherLastName() << "'s grade " << myClassroom.getGradeLevel() << " book." << std::endl;
	}

	//Menu
	void Teacher::mainMenu(GradeBook& book){while (true) {
			std::cout << "\nWelcome to the Teacher Menu.\n"
					  << "From here you can enter student information and grades.\n"
					  << "\nWhat would you like to do?\n\n"
					  << "1. Enter new student information.\n"
					  << "2. View all students.\n"
					  << "3. Generate an individual student report.\n"
					  << "4. Create new assignments.\n"
					  << "5. View all assignments.\n"
					  << "6. Enter grades for an assignment.\n"
					  << "7. Export full class report to CSV.\n"
					  << "8. Save your work.\n"
					  << "9. Exit program.\n";

			unsigned choice = numericValidator<unsigned>(
				"Please enter the number of your selection [1-9]: ", 1, 9);

			switch (choice) {
				case 1:
					book.addStudent();
					break;
				case 2:
					book.printAllStudents();
					break;
				case 3:
					// TODO: Implement individual student report
					std::cout << "Feature coming soon: Individual student report.\n";
					break;
				case 4:
					book.addAssignment();
					break;
				case 5:
					book.printAllAssignments();
					break;
				case 6:
					book.enterGrades();
					break;
				case 7:
					book.exportToCSV();
					break;
				case 8:
					book.serializeAndSave();
					break;
				case 9:
					book.closeMenu();
					return;  // Exit the teacher menu
				default:
					std::cout << "Invalid option. Please try again.\n";
			}
		}
	}
}