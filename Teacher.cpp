#include "Teacher.h"
#include "Student.h"
#include "User.h"
#include "Gradebook.h"
#include "utilities.h"
#include <fstream>
#include <iostream>
#include <iomanip>

namespace gradebook {

	// Mutators
	void Teacher::setTitle(const std::string& entry) {
		title = entry;
	}
	void Teacher::setGradeLevel(const unsigned& entry) {
		gradeLevel = entry;
	}

	// Accessors
	std::string Teacher::getTitle() const {
		return title;
	}
	std::string Teacher::getFirstName() const {
		return firstName;
	}
	std::string Teacher::getLastName() const {
		return lastName;
	}
	unsigned Teacher::getGradeLevel() const {
		return gradeLevel;
	}
	std::string Teacher::getPassword() const {
		return password;
	}
	std::vector<Assignment>& Teacher::getAssignments() {
		return assignments;
	}
	const std::vector<Assignment>& Teacher::getAssignments() const {
		return assignments;
	}
	std::vector<Student*>& Teacher::getClassroomStudents() {
		return students;
	}
	const std::vector<Student*>& Teacher::getClassroomStudents() const {
		return students;
	}

	// Assignment Construction and Grading
	void Teacher::addStudentToClassroom(Student* student) {
		students.push_back(student);
	}

	void Teacher::enterGrades(const std::vector<Assignment>& assignments, Gradebook& gradebook) {
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
			std::cout << i + 1 << ". " << students[i]->getFirstName() << " " << students[i]->getLastName() << std::endl;
		}
		unsigned studentChoice = numericValidator<unsigned>("Enter the number of the student: ", 1, students.size());
		Student* selectedStudent = students[studentChoice - 1];

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
				selectedStudent->setAssignmentScore(assign.getAssignmentName(), score);
			}
			std::cout << "All grades for " << selectedStudent->getFirstName() << " " << selectedStudent->getLastName() << " have been recorded." << std::endl;

			if (gradebook.isAutosaveEnabled()) {
				gradebook.serializeAndSave();
			}
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
			selectedStudent->setAssignmentScore(selectedAssignment.getAssignmentName(), score);

			std::cout << "Grade recorded for " << selectedAssignment.getAssignmentName() << "." << std::endl;

			if (gradebook.isAutosaveEnabled()) {
				gradebook.serializeAndSave();
			}
			break;
		}
		default:
			std::cout << "Invalid choice, please try again." << std::endl;
			return;
		}

		// Recalculate grade using full assignment list
		selectedStudent->calculateGrade(assignments);
	}

	void Teacher::scoreAllStudents(const std::vector<Assignment>& assignments) {
		for (auto* student : students) {
			if (student) {
				student->calculateGrade(assignments);
			}
		}
	}

	void Teacher::addAssignment(std::vector<Assignment>& assignments, Gradebook& gradebook) {
		Assignment assignment;

		do {
			assignment.setAssignmentName(stringValidator("Please enter the name of this assignment: "));
			assignment.setAssignmentDescription(stringValidator("Please enter a description of this assignment: "));
			assignment.setPointsPossible(numericValidator("Please enter the number of points it is possible to receive on this assignment: ", 1, 500));
			assignment.printAssignments();
		} while (!userCheck("Does this look right to you? [Y / N] ", "Great! Let's continue", "That's okay. Let's try again."));

		assignments.push_back(assignment);
		
		if (gradebook.isAutosaveEnabled()) {
			gradebook.serializeAndSave();
		}


		if (userCheck("Would you like to add another assignment? ",
			"Okay, let's add another.",
			"Returning to main menu.")) {
			addAssignment(assignments, gradebook);
		}
		else {
			if (gradebook.isAutosaveEnabled()) {
				gradebook.serializeAndSave();
			}
			menu(gradebook);
		}
	}


	// Print
	void Teacher::printClassroomReport() const {
		if (students.empty()) {
			std::cout << "No students in your classroom." << std::endl;
			return;
		}

		std::cout << "Classroom Report for " << getTitle() << " " << getLastName()
			<< ", Grade " << getGradeLevel() << std::endl;

		std::cout << std::left
			<< std::setw(15) << "First Name"
			<< std::setw(15) << "Last Name"
			<< std::setw(10) << "Pronouns"
			<< std::setw(5) << "Age"
			<< std::setw(10) << "ID"
			<< std::setw(10) << "Seat"
			<< std::setw(20) << "Notes"
			<< std::setw(10) << "Grade %"
			<< std::setw(10) << "Letter"
			<< std::endl;

		std::cout << std::string(105, '-') << std::endl;

		for (const auto* s : students) {
			if (!s) continue;

			std::cout << std::left
				<< std::setw(15) << s->getFirstName()
				<< std::setw(15) << s->getLastName()
				<< std::setw(10) << s->getPronouns()
				<< std::setw(5) << s->getAge()
				<< std::setw(10) << s->getID()
				<< std::setw(10) << s->getSeat()
				<< std::setw(20) << s->getNotes()
				<< std::setw(10) << std::fixed << std::setprecision(2) << s->getGradePercent()
				<< std::setw(10) << s->getOverallGrade()
				<< std::endl;
		}

		if (userCheck("Would you like to export this classroom report to CSV? [Y/N] ",
			"Exporting report to CSV...",
			"Skipping export.")) {
			exportClassroomReportToCSV();
		}
	}

	void Teacher::printAllStudents() const {
		std::cout << "This is " << getTitle() << " " << getFirstName() << " "
			<< getLastName() << "'s grade " << getGradeLevel() << " class." << std::endl;

		if (students.empty()) {
			std::cout << "No students in the system." << std::endl;
			return;
		}

		std::vector<Student*> sortedStudents = students;

		for (size_t i = 0; i < sortedStudents.size(); ++i) {
			for (size_t j = 0; j < sortedStudents.size() - 1 - i; ++j) {
				Student* studentA = sortedStudents[j];
				Student* studentB = sortedStudents[j + 1];
				if (!studentA || !studentB) continue;

				if (studentA->getLastName() > studentB->getLastName() ||
					(studentA->getLastName() == studentB->getLastName() &&
						studentA->getFirstName() > studentB->getFirstName())) {
					std::swap(sortedStudents[j], sortedStudents[j + 1]);
				}
			}
		}

		std::cout << "List of students:" << std::endl;
		int count = 1;
		for (const Student* s : sortedStudents) {
			if (!s) continue; 
			std::cout << std::setw(2) << count++ << ". " << s->getFirstName() << " " << s->getLastName() << std::endl;
		}
	}

	void Teacher::printAllAssignments() const {
		if (assignments.empty()) {
			std::cout << "No assignments to display.\n";
			return;
		}

		if (students.empty()) {
			std::cout << "No students in your class.\n";
			return;
		}

		std::cout << "=== Assignment Scores Report ===\n";

		for (const auto& assignment : assignments) {
			std::cout << "\nAssignment: " << assignment.getAssignmentName() << " ("
				<< assignment.getPointsPossible() << " pts)\n";

			std::cout << std::left << std::setw(25) << "Student Name"
				<< std::right << std::setw(10) << "Score\n";
			std::cout << "----------------------------------------\n";

			for (const auto* student : students) {
				if (!student) continue;  // Safety check

				auto scores = student->getAssignmentScores();
				auto it = scores.find(assignment.getAssignmentName());

				std::string studentName = student->getFirstName() + " " + student->getLastName();

				if (it != scores.end()) {
					std::cout << std::left << std::setw(25) << studentName
						<< std::right << std::setw(10) << std::fixed << std::setprecision(2)
						<< it->second << '\n';
				}
				else {
					std::cout << std::left << std::setw(25) << studentName
						<< std::right << std::setw(10) << "N/A" << '\n';
				}
			}
		}

		if (userCheck("Would you like to export this classroom report to CSV? [Y/N] ",
			"Exporting report to CSV...",
			"Skipping export.")) {
			exportAssignmentScoresToCSV();
		}
	}


	//Export
	void Teacher::exportClassroomReportToCSV() const {
		if (students.empty()) {
			std::cout << "No students in your classroom to export." << std::endl;
			return;
		}

		std::string filename = getTitle() + "_" + getLastName() + "_Grade" + std::to_string(getGradeLevel()) + ".csv";

		std::ofstream file(filename);
		if (!file.is_open()) {
			std::cerr << "Failed to open file " << filename << " for writing." << std::endl;
			return;
		}

		file << "First Name,Last Name,Pronouns,Age,ID,Seat,Notes,Grade %,Letter Grade\n";

		for (const auto* s : students) {
			if (!s) continue;

			file
				<< std::quoted(s->getFirstName()) << ','
				<< std::quoted(s->getLastName()) << ','
				<< std::quoted(s->getPronouns()) << ','
				<< s->getAge() << ','
				<< s->getID() << ','
				<< std::quoted(s->getSeat()) << ','
				<< std::quoted(s->getNotes()) << ','
				<< std::fixed << std::setprecision(2) << s->getGradePercent() << ','
				<< s->getOverallGrade()
				<< '\n';
		}

		file.close();
		std::cout << "Classroom report exported successfully to " << filename << std::endl;
	}

	void Teacher::exportAssignmentScoresToCSV() const {
		if (assignments.empty() || students.empty()) {
			std::cout << "Cannot export. Either no assignments or no students exist.\n";
			return;
		}

		std::string filename = getTitle() + "_" + getLastName() + "_Grade" + std::to_string(getGradeLevel()) + "_AssignmentScores.csv";
		for (char& ch : filename) {
			if (ch == ' ') {
				ch = '_';
			}
		}

		std::ofstream outFile(filename);
		if (!outFile.is_open()) {
			std::cerr << "Error opening file for writing.\n";
			return;
		}

		for (const auto& assignment : assignments) {
			outFile << "Assignment:," << assignment.getAssignmentName() << ","
				<< assignment.getPointsPossible() << " pts\n";
			outFile << "Student Name,Score\n";

			for (const auto* student : students) {
				if (!student) continue;

				std::string studentName = student->getFirstName() + " " + student->getLastName();
				auto scores = student->getAssignmentScores();
				auto it = scores.find(assignment.getAssignmentName());

				if (it != scores.end()) {
					outFile << "\"" << studentName << "\"," << std::fixed << std::setprecision(2) << it->second << "\n";
				}
				else {
					outFile << "\"" << studentName << "\",N/A\n";
				}
			}

			outFile << "\n";
		}

		outFile.close();
		std::cout << "Assignment scores exported to \"" << filename << "\" successfully.\n";
	}


	// Menu
	void Teacher::menu(Gradebook& gradebook) {
		while (true) {
			std::cout << "Welcome to the main menu.\n";
			std::cout << "From here you can enter student information and grades.\n\n";
			std::cout << "1. View all students.\n";
			std::cout << "2. Generate a report for a single student.\n";
			std::cout << "3. Generate a report for your entire class.\n";
			std::cout << "4. View all assignments.\n";
			std::cout << "5. Create new assignments.\n";
			std::cout << "6. Enter grades for an existing assignment.\n";
			std::cout << "7. Save your work.\n";
			std::cout << "8. Toggle autosave.\n";
			std::cout << "9. Log out.\n\n";

			unsigned choice = numericValidator<unsigned>(
				"Please enter the number of your selection [1-9]: ", 1, 9);

			switch (choice) {
			case 1:
				if (students.empty()) {
					std::cout << "There are no students in your class.\n";
				}
				else {
					printAllStudents();  // Make sure printAllStudents() is updated too
				}
				break;

			case 2: {
				if (students.empty()) {
					std::cout << "There are no students in your class.\n";
					break;
				}
				std::cout << "Select a student to view their report:\n";
				printAllStudents();
				unsigned index = numericValidator<unsigned>("Enter the number of the student: ", 1, students.size());

				Student* selected = students[index - 1];
				if (selected) {
					selected->printStudentReport();
					if (userCheck("Would you like to export this report to CSV? [Y/N] ",
						"Saving report...", "Skipping export.")) {
						selected->exportStudentReportToCSV();
					}
				}
				break;
			}

			case 3:
				if (students.empty()) {
					std::cout << "There are no students in your class.\n";
				}
				else {
					printClassroomReport();
				}
				break;

			case 4:
				if (getAssignments().empty()) {
					std::cout << "There are no assignments in the system.\n";
				}
				else {
					printAllAssignments();
				}
				break;

			case 5:
				addAssignment(getAssignments(), gradebook);
				break;

			case 6:
				if (students.empty()) {
					std::cout << "There are no students in your class.\n";
				}
				else if (getAssignments().empty()) {
					std::cout << "There are no assignments available to grade.\n";
				}
				else {
					enterGrades(getAssignments(), gradebook);
				}
				break;

			case 7:
				gradebook.serializeAndSave();
				break;

			case 8:
				gradebook.autosaveToggle();
				break;

			case 9:
				welcomeMenu(gradebook);
				return;

			default:
				std::cout << "Invalid option. Please try again.\n";
			}
		}
	}

}