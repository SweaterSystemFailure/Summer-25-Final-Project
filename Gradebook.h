#pragma once
#include "Classroom.h"
#include "Student.h"
#include "Administrator.h"
#include <iostream>
#include <vector>

namespace gradebook {
    	class Gradebook {
	private:
        std::vector<Teacher> teacher;
		std::vector<Student> students;
		std::vector<Administrator> school;

	public:
		//Admin Function
        void createSchool();

        //Classroom Function
		void createClassroom();

		//Student Functions
		void addStudent();
		void scoreAllStudents();
		
        
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