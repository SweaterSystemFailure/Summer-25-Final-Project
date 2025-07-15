#pragma once
#include <iostream>


namespace gradebook {
	class Classroom {
	private:
		//Biographical Variables
		std::string title;
		std::string teacherFirstName;
		std::string teacherLastName;
		unsigned gradeLevel;
	public:
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

}