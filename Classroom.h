#pragma once
#include <iostream>


namespace gradebook {
	class Classroom : public Student {
	private:
		//Biographical Variables
		std::string title;
		std::string teacherFirstName;
		std::string teacherLastName;
		unsigned gradeLevel;
        std::string teacherPassword
	public:
		//Mutators
		void setTitle(const std::string& entry);
		void setTeacherFirstName(const std::string& entry);
		void setTeacherLastName(const std::string& entry);
		void setGradeLevel(const unsigned& entry);
        void setTeacherPassword(const std::string entry);

		//Accessors
		std::string getTitle() const;
		std::string getTeacherFirstName() const;
		std::string getTeacherLastName() const;
		unsigned getGradeLevel() const;
        std::string getTeacherPassword() const;

		//Print Function
		void printClassroomReport();

        //Menu
        void teacherMainMenu() override;
	};
}