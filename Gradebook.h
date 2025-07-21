#pragma once
#include "Teacher.h"
#include "Student.h"
#include "Administrator.h"
#include "User.h"
#include <iostream>
#include <vector>
#include <map>

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
		const std::vector<Student>& getStudents() const;
		std::vector<Administrator>& getSchool();
		bool isAutosaveEnabled() const;
		
		//Admin Function
        void createSchool();
		
		//Login Function
		bool handlePassword(User& user);

        //Binary Save/Load Functions
        void serializeAndSave();
        void deserializeAndLoad();
		void autosaveToggle();
		void clearCachedData();
	};
}