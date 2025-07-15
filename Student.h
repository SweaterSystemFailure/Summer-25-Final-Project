#pragma once
#include "Assignment.h"
#include "Gradebook.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <map>

namespace gradebook {
	class Student {
	private:
		//Biographical Variables
		std::string firstName;
		std::string lastName;
		std::string pronouns;
		unsigned age;
		unsigned id;
		std::string seat;
		std::string notes;
		std::string studentPassword;

		//Individual Student Assignment Container
		std::map<std::string, float> assignmentScores;
	public:
		//Grade Variables
		char overallGrade;
		float gradePercent;
		
		//Initializer
		static Student createStudent();

		//Mutators
		void setFirstName(const std::string& entry);
		void setLastName(const std::string& entry);
		void setPronouns(const std::string& entry);
		void setAge(const unsigned& entry);
		void setID(const unsigned& entry);
		void setSeat(const std::string& entry);
		void setNotes(const std::string& entry);
		void setStudentPassword(const std::string entry);
		void setAssignmentScore(const std::string& assignmentName, float score);

		//Accessors
		std::string getFirstName() const;
		std::string getLastName() const override;
		std::string getPronouns() const;
		unsigned getAge() const;
		unsigned getID() const;
		std::string getSeat() const;
		std::string getNotes() const;
		std::string getStudentPassword() const override;
		float getAssignmentScore(const std::string& assignmentName) const;

		//Grade Calculation
		void calculateGrade(const std::vector<Assignment>& assignments);

		//Print Function
		void printStudent();
		void printStudentReport() const;

		//Menu
		void menu(Gradebook& book) override;
    	std::string getRole() const override { return "Student"; };

	};
}