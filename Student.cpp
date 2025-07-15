#include "containers.h"
#include "utilities.h"
#include "menus.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <map>

namespace gradebook {
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
	unsigned Student::getID() const {
		return id;
	}
	std::string Student::getSeat() const {
		return seat;
	}
	std::string Student::getNotes() const {
		return notes;
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
}