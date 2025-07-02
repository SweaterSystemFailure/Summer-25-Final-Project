#include "containers.h"
#include "helperFunctions.h"
#include "menus.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <map>

namespace gradebookContainers {
	using namespace Helpers;

	//Gradebook
	//Classroom Functions
	void Gradebook::createClassroom() {
		Classroom myClassroom;

		std::cout << "Let's get started by entering some basic information." << std::endl;

		do {
			myClassroom.setTitle(stringValidator("Which honorific do your students address you by? ..."));
			myClassroom.setTeacherFirstName(stringValidator("What is your first name? "));
			myClassroom.setTeacherLastName(stringValidator("What is your last name? "));
			myClassroom.setGradeLevel(numericValidator<unsigned>("Which grade do you teach? [1–12]: ", 1, 12));
			myClassroom.printClassroom();

		} while (!userCheck(
			"Does this look right to you? [Y/N] ",
			"Great! Let's continue.",
			"That's okay. Let's try again."));

		currentClass = myClassroom;

		mainMenu();
	}

	//Student Functions
	void Gradebook::addStudent() {
		Student newStudent;

		do {
			newStudent.setFirstName(stringValidator("Please enter the student's first name: "));
			newStudent.setLastName(stringValidator("Please enter the student's last name: "));
			newStudent.setPronouns(stringValidator("Please enter the student's pronouns: "));
			newStudent.setAge(numericValidator("Please enter the student's age: ", 4, 19));
			newStudent.setID(numericValidator("Please enter the student's ID number: ", 1, 999999));
			newStudent.setSeat(stringValidator("Please enter the student's seat location: "));
			newStudent.setNotes(stringValidator("Enter any additional notes for this student: "));
			newStudent.printStudent();
		} while (!userCheck("Does this look right to you ? [Y / N] ", "Great! Let's continue", "That's okay. Let's try again."));

		students.push_back(newStudent);

		std::cout << "Student " << newStudent.getFirstName() << " " << newStudent.getLastName()
			<< " has been added successfully!" << std::endl;

		if (userCheck("Would you like to add another student? ",
			"Okay, let's add another",
			"Returning to main menu.")) {
			addStudent();
		}
		else {
			mainMenu();
		}
	}

	void Gradebook::enterGrades() {
		if (students.empty()) {
			std::cout << "No students in the system. Please add students first." << std::endl;
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
				selectedStudent.assignmentScores[assign.getAssignmentName()] = score;
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
			Assignment& selectedAssignment = assignments[assignmentChoice - 1];

			std::string prompt = "Enter score for \"" + selectedAssignment.getAssignmentName() + "\" (" + std::to_string(selectedAssignment.getPointsPossible()) + " pts): ";
			float score = numericValidator<float>(prompt, 0.0f, selectedAssignment.getPointsPossible());
			selectedStudent.assignmentScores[selectedAssignment.getAssignmentName()] = score;

			std::cout << "Grade recorded for " << selectedAssignment.getAssignmentName() << "." << std::endl;
			break;
		}
		default:
			std::cout << "Invalid choice, please try again." << std::endl;
			return;
		}

		selectedStudent.calculateGrade(assignments);
	}

	void Gradebook::scoreAllStudents() {
		for (auto& s : students) {
			float totalPointsPossible = 0.0f;
			float totalPointsScored = 0.0f;

			for (const auto& assign : assignments) {
				totalPointsPossible += assign.getPointsPossible();
				if (s.assignmentScores.count(assign.getAssignmentName())) {
					totalPointsScored += s.assignmentScores.at(assign.getAssignmentName());
				}
			}

			if (totalPointsPossible > 0) {
				s.gradePercent = (totalPointsScored / totalPointsPossible) * 100.0f;
			}

			if (s.gradePercent >= 90.0f) s.overallGrade = 'A';
			else if (s.gradePercent >= 80.0f) s.overallGrade = 'B';
			else if (s.gradePercent >= 70.0f) s.overallGrade = 'C';
			else if (s.gradePercent >= 60.0f) s.overallGrade = 'D';
			else s.overallGrade = 'F';
		}
	}

	//Assignment Functions
	void Gradebook::addAssignment() {
		Assignment assignment;

		do {
			assignment.setAssignmentName(stringValidator("Please enter the name of this assignment: "));
			assignment.setAssignmentDescription(stringValidator("Please enter a description of this assignment: "));
			assignment.setPointsPossible(numericValidator("Please enter the number of points it is possible to receive on this assignment: ", 1, 500));
			assignment.printAssignments();
		} while (!userCheck("Does this look right to you ? [Y / N] ", "Great! Let's continue", "That's okay. Let's try again."));


	}

	//Print Functions
	void Gradebook::printAllStudents() const {
		if (students.empty()) {
			std::cout << "No students in the system." << std::endl;
			return;
		}

		//Sortable vector
		std::vector<Student> sortedStudents = students;

		// Bubble sort for alpha by last name
		for (size_t i = 0; i < sortedStudents.size(); ++i) {
			for (size_t j = 0; j < sortedStudents.size() - 1 - i; ++j) {
				if (sortedStudents[j].getLastName() > sortedStudents[j + 1].getLastName() ||
					(sortedStudents[j].getLastName() == sortedStudents[j + 1].getLastName() && sortedStudents[j].getFirstName() > sortedStudents[j + 1].getFirstName())) {
					Student temp = sortedStudents[j];
					sortedStudents[j] = sortedStudents[j + 1];
					sortedStudents[j + 1] = temp;
				}
			}
		}

		//Prints list
		int count = 1;
		std::cout << "List of students:" << std::endl;
		for (const auto& s : sortedStudents) {
			std::cout << std::setw(2) << count++ << ". " << s.getFirstName() << " " << s.getLastName() << std::endl;
		}
	}
	void Gradebook::printAllAssignments() const {

	}
	void Gradebook::printClassReport() const {}



	//Classroom
	//Mutators
	void Classroom::setTitle(const std::string& entry) {
		title = entry;
	}
	void Classroom::setTeacherFirstName(const std::string& entry) {
		teacherFirstName = entry;
	}
	void Classroom::setTeacherLastName(const std::string& entry) {
		teacherLastName = entry;
	}
	void Classroom::setGradeLevel(const unsigned& entry) {
		gradeLevel = entry;
	}

	//Accessors
	std::string Classroom::getTitle() const {
		return title;
	}
	std::string Classroom::getTeacherFirstName() const {
		return teacherFirstName;
	}
	std::string Classroom::getTeacherLastName() const {
		return teacherLastName;
	}
	unsigned Classroom::getGradeLevel() const {
		return gradeLevel;
	}

	//Print Function
	void printClassroom(Classroom& myClassroom) {
		std::cout << "This is " << myClassroom.getTitle() << " " << myClassroom.getTeacherFirstName() << " "
			<< myClassroom.getTeacherLastName() << "'s grade " << myClassroom.getGradeLevel() << " book." << std::endl;
	}


	//Student
	//Mutators
	void Student::setFirstName(const std::string& entry) {
		firstName = entry;
	}
	void Student::setLastName(const std::string& entry) {
		lastName = entry;
	}
	void Student::setPronouns(const std::string& entry) {
		pronouns = entry;
	}
	void Student::setAge(const unsigned& entry) {
		age = entry;
	}
	void Student::setID(const unsigned& entry) {
		id = entry;
	}
	void Student::setSeat(const std::string& entry) {
		seat = entry;
	}
	void Student::setNotes(const std::string& entry) {
		notes = entry;
	}

	//Accessors
	std::string Student::getFirstName() const {
		return firstName;
	}
	std::string Student::getLastName() const {
		return lastName;
	}
	std::string Student::getPronouns() const {
		return pronouns;
	}
	unsigned Student::getAge() const {
		return age;
	}
	unsigned Student::getID() const {
		return id;
	}
	std::string Student::getSeat() const {
		return seat;
	}
	std::string Student::getNotes() const {
		return notes;
	}

	//Grade Calculation
	void Student::calculateGrade(const std::vector<Assignment>& assignments) {
		float totalPointsPossible = 0.0f;
		float totalPointsScored = 0.0f;

		for (const auto& assign : assignments) {
			totalPointsPossible += assign.getPointsPossible();
			if (assignmentScores.count(assign.getAssignmentName())) {
				totalPointsScored += assignmentScores.at(assign.getAssignmentName());
			}
		}

		if (totalPointsPossible > 0.0f) {
			gradePercent = (totalPointsScored / totalPointsPossible) * 100.0f;
		}

		if (gradePercent >= 90.0f) overallGrade = 'A';
		else if (gradePercent >= 80.0f) overallGrade = 'B';
		else if (gradePercent >= 70.0f) overallGrade = 'C';
		else if (gradePercent >= 60.0f) overallGrade = 'D';
		else overallGrade = 'F';
	}

	//Print Functions
	void Student::printStudent() {
		std::cout << "New Student Profile\n";
		std::cout << "------------------------\n";
		std::cout << "First Name      : " << firstName << std::endl;
		std::cout << "Last Name       : " << lastName << std::endl;
		std::cout << "Pronouns        : " << pronouns << std::endl;
		std::cout << "Age             : " << age << std::endl;
		std::cout << "Student ID      : " << id << std::endl;
		std::cout << "Seat Location   : " << seat << std::endl;
		std::cout << "Notes           : " << notes << std::endl;
		std::cout << "------------------------\n" << std::endl;
	}

	void Student::printStudentReport() const {

	}

	//Assignments
	//Mutators
	void Assignment::setAssignmentName(const std::string& entry) {
		assignmentName = entry;
	}
	void Assignment::setAssignmentDescription(const std::string& entry) {
		assignmentDescription = entry;
	}
	void Assignment::setPointsPossible(float entry) {
		pointsPossible = entry;
	}

	//Accessors
	std::string Assignment::getAssignmentName() const {
		return assignmentName;
	}
	std::string Assignment::getAssignmentDescription() const {
		return assignmentDescription;
	}
	float Assignment::getPointsPossible() const {
		return pointsPossible;
	}

	//Print Function
	void Assignment::printAssignments() const {}

}