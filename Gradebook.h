#pragma once
#include "Classroom.h"
#include "Student.h"
#include "Administrator.h"
#include <iostream>
#include <vector>

namespace gradebook {
    	class Gradebook {
	private:
		Teacher currentClass;
		std::vector<Student> students;
		std::vector<Assignment> assignments;
        Administrator currentSchool;
		std::vector<Administrator> school;

	public:
		//Admin Function
        void createSchool();

        //Classroom Function
		void createClassroom();

		//Student Functions
		void addStudent();
		void enterGrades();
		void scoreAllStudents();
		
		//Assignemnt Function
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