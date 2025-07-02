#pragma once
#include "helperFunctions.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <map>

namespace gradebookContainers {
	using namespace Helpers;
	
	class Gradebook {
	private:
		std::vector<Student> students;
		std::vector<Assignment> assignments;
		std::vector<Classroom> currentClass;
	public:
		//Student Functions
		void addStudent();
		void enterGrades();
		
		//Assignemnt Functions
		void addAssignment();

		//Print Functions
		void printAllStudents() const;
		void printAllAssignments() const;
		void printClassReport() const;

	};

	class Classroom {
	private:
		//Biographical Variables
		std::string title;
		std::string teacherFirstName;
		std::string teacherLastName;
		unsigned gradeLevel;
	public:
		//Initializer
		static Classroom createClassroom();

		//Mutators
		void setTitle(const std::string& entry);
		void setTeacherFirstName(const std::string& entry);
		void setTeacherLastName(const std::string& entry);
		void setGradeLevel(const unsigned& entry);

		//Accessors
		std::string getTitle() const;
		std::string getTeacherFirstName() const;
		std::string getTeacherLastName() const;
		unsigned getGradeLevel() const;

		//Print Function
		void printClassroom();
	};

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
	public:
		//Grade Variables
		char overallGrade;
		float gradePercent;
		
		//Individual Student Assignment Container
		std::map<std::string, float> assignmentScores;

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

		//Accessors
		std::string getFirstName() const;
		std::string getLastName() const;
		std::string getPronouns() const;
		unsigned getAge() const;
		unsigned getID() const;
		std::string getSeat() const;
		std::string getNotes() const;

		//Print Function
		void printStudent() const;
		void printStudentReport() const;
	};

	class Assignment {
	private:
		std::string assignmentName;
		float pointsPossible;
	public:
		//Initializer
		static Assignment createAssignment();

		//Mutators
		void setAssignmentName(const std::string& entry);
		void setPointsPossible(float entry);
		
		//Accessors
		std::string getAssignmentName() const;
		float getPointsPossible() const;

		//Print Function
		void printAssignments() const;
	};
}