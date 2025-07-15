#pragma once
#include "Classroom.h"
#include "Student.h"
#include "Administrator.h"
#include <iostream>
#include <vector>

namespace gradebook {
    	class Gradebook {
	private:
		Classroom currentClass;
		std::vector<Student> students;
		std::vector<Assignment> assignments;
		std::vector<Administrator> school;

	public:
		//Classroom Functions
		void createClassroom();

		//Student Functions
		void addStudent();
		void enterGrades();
		void scoreAllStudents();
		
		//Assignemnt Functions
		void addAssignment();
        
        //Binary Save/Load Functions
        void serializeAndSave();
        void deserializeAndLoad();

		//Print Functions
		void printAllStudents() const;
		void printAllAssignments() const;
		void printClassReport() const;
        void exportToCSV();

	};
}