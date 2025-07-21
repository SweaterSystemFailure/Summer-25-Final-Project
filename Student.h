#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <map>
#include "Assignment.h"
#include "User.h"

namespace gradebook {
	class Gradebook;

	class Student : public User {
	private:
		//Biographical Variables
		std::string pronouns;
		unsigned age;
		unsigned gradelevel;
		unsigned id;
		std::string seat;
		std::string notes;

		//Grade Variables
		char overallGrade;
		float gradePercent;

		//Individual Student Assignment Container
		std::map<std::string, float> assignmentScores;

	public:
		//Mutators
		void setPronouns(const std::string& entry);
		void setAge(const unsigned& entry);
		void setGradeLevel(const unsigned& entry);
		void setID(const unsigned& entry);
		void setSeat(const std::string& entry);
		void setNotes(const std::string& entry);
		void setOverallGrade(const char& entry);
		void setGradePercent(const float& entry);
		void setAssignmentScore(const std::string& assignmentName, float score);

		//Accessors
		std::string getFirstName() const override;
		std::string getLastName() const override;
		std::string getPronouns() const;
		unsigned getAge() const;
		unsigned getGradeLevel() const;
		unsigned getID() const;
		std::string getSeat() const;
		std::string getNotes() const;
		std::string getPassword() const override;
		char getOverallGrade() const;
		float getGradePercent() const;
		const std::map<std::string, float>& getAssignmentScores() const;

		//Grade Calculation
		void calculateGrade(const std::vector<Assignment>& assignments);

		//Print Function
		void printStudent() const;
		void printStudentReport() const;

		//Export
		void exportStudentReportToCSV() const;

		//Menu
		void menu(Gradebook& book) override;
		std::string getRole() const override { return "Student"; };
	};
}