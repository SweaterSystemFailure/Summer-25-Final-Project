#include "Classroom.h"
#include <iostream>

namespace gradebook {
//Classroom
	//Mutators
	void Classroom::setTitle(const std::string& entry) {
		title = entry;
	}
	void Classroom::setTeacherFirstName(const std::string& entry) {
		teacherFirstName = entry;
	}
	void Classroom::setTeacherLastName(const std::string& entry) {
		teacherLastName = entry;
	}
	void Classroom::setGradeLevel(const unsigned& entry) {
		gradeLevel = entry;
	}

	//Accessors
	std::string Classroom::getTitle() const {
		return title;
	}
	std::string Classroom::getTeacherFirstName() const {
		return teacherFirstName;
	}
	std::string Classroom::getTeacherLastName() const {
		return teacherLastName;
	}
	unsigned Classroom::getGradeLevel() const {
		return gradeLevel;
	}

	//Print Function
	void printClassroom(Classroom& myClassroom) {
		std::cout << "This is " << myClassroom.getTitle() << " " << myClassroom.getTeacherFirstName() << " "
			<< myClassroom.getTeacherLastName() << "'s grade " << myClassroom.getGradeLevel() << " book." << std::endl;
	}
}