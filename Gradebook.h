#pragma once
#include "Classroom.h"
#include <iostream>
#include <vector>

namespace gradebook {
    	class Gradebook {
	private:
		Classroom currentClass;
		std::vector<Student> students;
		std::vector<Assignment> assignments;
		
	public:
		//Classroom Functions
		void createClassroom();

		//Student Functions
		void addStudent();
		void enterGrades();
		void scoreAllStudents();
		
		//Assignemnt Functions
		void addAssignment();

		//Print Functions
		void printAllStudents() const;
		void printAllAssignments() const;
		void printClassReport() const;

	};
}