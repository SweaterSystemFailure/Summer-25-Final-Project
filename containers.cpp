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
	void Gradebook::addStudent() {
		Student newStudent;

		do {
			newStudent.setFirstName(stringValidator("Please enter the student's first name: "));
			newStudent.setLastName(stringValidator("Please enter the student's last name: "));
			newStudent.setPronouns(stringValidator("Please enter the student's pronouns: "));
			newStudent.setAge(numericValidator("Please enter the student's age: ", 4, 19));
			newStudent.setID(numericValidator("Please enter the student's ID number: ", 1, 9999));
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


	//Classroom
	Classroom Classroom::createClassroom() {
		Classroom myClassroom;

		std::cout << "Let's get started by entering some basic information." << std::endl;

		do {
			myClassroom.title = stringValidator("Which honorific do your students address you by? ...");
			myClassroom.teacherFirstName = stringValidator("What is your first name? ");
			myClassroom.teacherLastName = stringValidator("What is your last name? ");
			myClassroom.gradeLevel = numericValidator<unsigned>("Which grade do you teach? [1–12]: ", 1, 12);

			std::cout << "This is " << myClassroom.title << " " << myClassroom.teacherFirstName << " "
				<< myClassroom.teacherLastName << "'s grade " << myClassroom.gradeLevel << " book." << std::endl;

		} while (!userCheck(
			"Does this look right to you? [Y/N] ",
			"Great! Let's continue.",
			"That's okay. Let's try again."));

		return myClassroom;
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