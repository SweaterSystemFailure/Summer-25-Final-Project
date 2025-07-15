#include "Teacher.h"
#include "utilities.h"
#include <iostream>

namespace gradebook {

// Mutators
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
void Teacher::setAutoSave(const bool& entry){
	autosave = entry;
}

// Accessors
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
bool Teacher::getAutoSave() const{
	return autosave;
}

// Assignment Construction and Grading
void Teacher::addStudentToClassroom(const Student& student) {
    students.push_back(student);
}

void Teacher::enterGrades(const std::vector<Assignment>& assignments) {
	if (students.empty()) {
		std::cout << "No students in your class. Please add students first." << std::endl;
		return;
	}
	if (assignments.empty()) {
		std::cout << "No assignments available to grade. Please add assignments first." << std::endl;
		return;
	}

	// Select student
	std::cout << "Select a student to enter grades for:" << std::endl;
	for (size_t i = 0; i < students.size(); ++i) {
		std::cout << i + 1 << ". " << students[i].getFirstName() << " " << students[i].getLastName() << std::endl;
	}
	unsigned studentChoice = numericValidator<unsigned>("Enter the number of the student: ", 1, students.size());
	Student& selectedStudent = students[studentChoice - 1];

	// Grading choice
	std::cout << "Would you like to:" << std::endl;
	std::cout << "1. Enter grades for all assignments" << std::endl;
	std::cout << "2. Enter grade for one specific assignment" << std::endl;
	unsigned choice = numericValidator<unsigned>("Choose an option [1-2]: ", 1, 2);

	switch (choice) {
		case 1: {
			for (const auto& assign : assignments) {
				std::string prompt = "Enter score for \"" + assign.getAssignmentName() + "\" (" + std::to_string(assign.getPointsPossible()) + " pts): ";
				float score = numericValidator<float>(prompt, 0.0f, assign.getPointsPossible());
				selectedStudent.setAssignmentScore(assign.getAssignmentName(), score);
			}
			std::cout << "All grades for " << selectedStudent.getFirstName() << " " << selectedStudent.getLastName() << " have been recorded." << std::endl;
			break;
		}
		case 2: {
			std::cout << "Select an assignment:" << std::endl;
			for (size_t i = 0; i < assignments.size(); ++i) {
				std::cout << i + 1 << ". " << assignments[i].getAssignmentName() << " (" << assignments[i].getPointsPossible() << " pts)" << std::endl;
			}
			unsigned assignmentChoice = numericValidator<unsigned>("Enter the number of the assignment: ", 1, assignments.size());
			const Assignment& selectedAssignment = assignments[assignmentChoice - 1];

			std::string prompt = "Enter score for \"" + selectedAssignment.getAssignmentName() + "\" (" + std::to_string(selectedAssignment.getPointsPossible()) + " pts): ";
			float score = numericValidator<float>(prompt, 0.0f, selectedAssignment.getPointsPossible());
			selectedStudent.setAssignmentScore(selectedAssignment.getAssignmentName(), score);

			std::cout << "Grade recorded for " << selectedAssignment.getAssignmentName() << "." << std::endl;
			break;
		}
		default:
			std::cout << "Invalid choice, please try again." << std::endl;
			return;
	}

	selectedStudent.calculateGrade(assignments);
}

void Teacher::scoreAllStudents(const std::vector<Assignment>& assignments) {
	for (auto& student : students) {
		student.calculateGrade(assignments);
	}
}

void Teacher::addAssignment(std::vector<Assignment>& assignments) {
	Assignment assignment;

	do {
		assignment.setAssignmentName(stringValidator("Please enter the name of this assignment: "));
		assignment.setAssignmentDescription(stringValidator("Please enter a description of this assignment: "));
		assignment.setPointsPossible(numericValidator("Please enter the number of points it is possible to receive on this assignment: ", 1, 500));
		assignment.printAssignments();
	} while (!userCheck("Does this look right to you? [Y / N] ", "Great! Let's continue", "That's okay. Let's try again."));

	assignments.push_back(assignment);

	if (userCheck("Would you like to add another assignment? ",
		"Okay, let's add another.",
		"Returning to main menu.")) {
		addAssignment(assignments);
	}
	else {
		// Call menu with proper Gradebook reference - must be passed or stored.
	}
}

// Print
void Teacher::printClassroomReport() const {
	std::cout << "This is " << getTitle() << " " << getTeacherFirstName() << " "
		<< getTeacherLastName() << "'s grade " << getGradeLevel() << " book." << std::endl;
}

// Menu
void Teacher::menu(Gradebook& book) {
	while (true) {
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
				book.printStudentReport();
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
				book.welcomeMenu();
				return;
			default:
				std::cout << "Invalid option. Please try again." << std::endl;
		}
	}
}

}
