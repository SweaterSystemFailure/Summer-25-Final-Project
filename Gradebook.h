#pragma once
#include "Teacher.h"
#include "Student.h"
#include "Administrator.h"
#include <iostream>
#include <vector>

namespace gradebook {
    	class Gradebook {
	private:
        std::vector<Teacher> teachers;
		std::vector<Student> students;
		std::vector<Administrator> school;
		bool autosaveEnabled = true;

	public:
		//Accessors
		std::vector<Teacher>& getTeachers();
		const std::vector<Teacher>& getTeachers() const;
		std::vector<Student>& getStudents();
		const std::vector<Student>& getStudents();
		bool getAutosaveEnabled() const;
		
		//Admin Function
        void createSchool();
		
		//Login Function
		bool handlePassword(User& user);
		bool login();

        //Binary Save/Load Functions
        void serializeAndSave();
        void deserializeAndLoad();
		void autosaveToggle();

		//Print Functions
		void printAllStudents() const;
		void printAllAssignments() const;
		void printClassReport() const;
	};
}