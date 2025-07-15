#include "Gradebook.h"
#include "Classroom.h"
#include "utilities.h"
#include <iostream>
#include <iomanip>


namespace gradebook {
    void Gradebook::createSchool(){
        Administrator myAdmin;
        std::string password;

        std::cout << "Let's get started by entering some basic infomation." << std::endl;

        do{
            myAdmin.setAdminTitle(stringValidator("Which honorific are you addressed by? "));
            myAdmin.setAdminLastName(stringValidator("What is your last name?"));
            myAdmin.setSchoolName(stringValidator("What is the name of your school?"));
        }while (!userCheck("Does this look right to you? [Y/N] ",
			"Great! Let's continue.",
			"That's okay. Let's try again."));
        
        do{
            password = stringValidator("Please select a strong password that is at least 8 characters long, and that contains a mix of upper and lower case characters, numbers, and special symbols.");            
        }while(!isStrongPassword(password));

        myAdmin.setAdminPassword(password);

        school.push_back(myAdmin);
    }

    
    void Gradebook::createClassroom() {
		Teacher myClassroom;

		std::cout << "Let's get started by entering some basic information." << std::endl;

		do {
			myClassroom.setTitle(stringValidator("Which honorific do your students address you by? "));
			myClassroom.setTeacherFirstName(stringValidator("What is your first name? "));
			myClassroom.setTeacherLastName(stringValidator("What is your last name? "));
			myClassroom.setGradeLevel(numericValidator<unsigned>("Which grade do you teach? [1�12]: ", 1, 12));
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
			"Okay, let's add another.",
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
			Assignment& selectedAssignment = assignments[assignmentChoice - 1];

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

	void Gradebook::scoreAllStudents() {
		for (auto& student : students) {
			student.calculateGrade(assignments);
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

		if (userCheck("Would you like to add another assignment? ",
			"Okay, let's add another.",
			"Returning to main menu.")) {
			addAssignment();
		}
		else {
			mainMenu();
		}

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
}