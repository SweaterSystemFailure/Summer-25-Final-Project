#include "containers.h"
#include "helperFunctions.h"
#include "menus.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <map>

namespace gradebookContainers {
	using namespace Helpers;

	//Gradebook
	//Student Functions
	void Gradebook::addStudent() {
		Student newStudent;

		do {
			newStudent.setFirstName(stringValidator("Please enter the student's first name: "));
			newStudent.setLastName(stringValidator("Please enter the student's last name: "));
			newStudent.setPronouns(stringValidator("Please enter the student's pronouns: "));
			newStudent.setAge(numericValidator("Please enter the student's age: ", 4, 19));
			newStudent.setID(numericValidator("Please enter the student's ID number: ", 1, 999999));
			newStudent.setSeat(stringValidator("Please enter the student's seat location: "));
			newStudent.setNotes(stringValidator("Enter any additional notes for this student: "));
			newStudent.printStudent();
		} while (!userCheck("Does this look right to you ? [Y / N] ", "Great! Let's continue", "That's okay. Let's try again."));

		students.push_back(newStudent);

		std::cout << "Student " << newStudent.getFirstName() << " " << newStudent.getLastName()
			<< " has been added successfully!" << std::endl;

		if (userCheck("Would you like to add another student? ",
			"Okay, let's add another",
			"Returning to main menu.")) {
			addStudent();
		}
		else {
			mainMenu();
		}
	}

	void Gradebook::enterGrades();

	//Assignemnt Functions
	void Gradebook::addAssignment();

	//Print Functions
	void Gradebook::printAllStudents() const;
	void Gradebook::printAllAssignments() const;
	void Gradebook::printClassReport() const;



	//Classroom
	//Initializer -- this needs to go to Gradebook!
	void Classroom::createClassroom() {
		Classroom myClassroom;

		std::cout << "Let's get started by entering some basic information." << std::endl;

		do {
			myClassroom.setTitle(stringValidator("Which honorific do your students address you by? ..."));
			myClassroom.setTeacherFirstName(stringValidator("What is your first name? "));
			myClassroom.setTeacherLastName(stringValidator("What is your last name? "));
			myClassroom.setGradeLevel(numericValidator<unsigned>("Which grade do you teach? [1–12]: ", 1, 12));
			myClassroom.printClassroom();

		} while (!userCheck(
			"Does this look right to you? [Y/N] ",
			"Great! Let's continue.",
			"That's okay. Let's try again."));
		currentClass.push_back(myClassroom);
		mainMenu();
		
	}
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
	

	//Student
	//Mutators
	void Student::setFirstName(const std::string& entry) {
		firstName = entry;
	}
	void Student::setLastName(const std::string& entry) {
		lastName = entry;
	}
	void Student::setPronouns(const std::string& entry) {
		pronouns = entry;
	}
	void Student::setAge(const unsigned& entry) {
		age = entry;
	}
	void Student::setID(const unsigned& entry) {
		id = entry;
	}
	void Student::setSeat(const std::string& entry) {
		seat = entry;
	}
	void Student::setNotes(const std::string& entry) {
		notes = entry;
	}

	//Accessors
	std::string Student::getFirstName() const {
		return firstName;
	}
	std::string Student::getLastName() const {
		return lastName;
	}
	std::string Student::getPronouns() const {
		return pronouns;
	}
	unsigned Student::getAge() const {
		return age;
	}
	unsigned Student::getID() const {
		return id;
	}
	std::string Student::getSeat() const {
		return seat;
	}
	std::string Student::getNotes() const {
		return notes;
	}
}