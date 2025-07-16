#include "Student.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <map>
#include "User.h"
#include "utilities.h"

namespace gradebook {
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
	void Student::setGradeLevel(const unsigned& entry) {
		gradelevel = entry;
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
	void Student::setStudentPassword(const std::string entry) {
		studentPassword = entry;
	}
	void Student::setOverallGrade(const char& entry) {
		overallGrade = entry;
	}
	void Student::setGradePercent(const float& entry) {
		gradePercent = entry;
	}
	void Student::setAssignmentScore(const std::string& assignmentName, float score) {
		assignmentScores[assignmentName] = score;
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
	unsigned Student::getGradeLevel() const {
		return gradelevel;
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
	std::string Student::getStudentPassword() const {
		return studentPassword;
	}
	char Student::getOverallGrade() const {
		return overallGrade;
	}
	float Student::getGradePercent() const {
		return gradePercent;
	}
	float Student::getAssignmentScore(const std::string& assignmentName) const {
		auto it = assignmentScores.find(assignmentName);
		return it != assignmentScores.end() ? it->second : 0.0f;
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

		overallGrade = (gradePercent >= 90.0f) ? 'A' :
			(gradePercent >= 80.0f) ? 'B' :
			(gradePercent >= 70.0f) ? 'C' :
			(gradePercent >= 60.0f) ? 'D' : 'F';
	}

	//Print Functions
	void Student::printStudent() {
		std::cout << "Student Profile\n";
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
		printStudent();

		std::cout << std::endl << "=== Assignment Scores ===" << std::endl;
		if (assignmentScores.empty()) {
			std::cout << "No assignments have been graded yet.\n";
		}
		else {
			for (const auto& pair : assignmentScores) {
				std::cout << std::left << std::setw(20) << pair.first
					<< "Score: " << std::fixed << std::setprecision(2)
					<< pair.second << " pts\n";
			}
		}

		std::cout << std::endl;
		std::cout << "Overall Grade: " << overallGrade
			<< " (" << std::fixed << std::setprecision(2) << gradePercent << "%)\n";
	}

	void Student::menu() {
		while (true) {
			std::cout << "\n=== Student Menu ===\n";
			std::cout << "1. View My Profile." << std::cout;
			std::cout << "2. View My Grades." << std::cout;
			std::cout << "3. Log Out." << std::cout;

			unsigned choice = numericValidator<unsigned>("Choose an option [1-3]: ", 1, 3);

			switch (choice) {
			case 1:
				printStudent();
				break;
			case 2:
				printStudentReport();
				break;
			case 3:
				welcomeMenu();
				return;
			default:
				std::cout << "Invalid selection. Please try again.\n";
				break;
			}
		}
	}
}