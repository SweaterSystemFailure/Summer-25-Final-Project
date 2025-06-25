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

	void addStudent(globalStorage& storage) {
		Student newStudent;

		do {
			newStudent.firstName = stringValidator("Please enter the student's first name: ");
			newStudent.lastName = stringValidator("Please enter the student's last name: ");
			newStudent.pronouns = stringValidator("Please enter the student's pronouns: ");
			newStudent.age = numericValidator("Please enter the student's age: ", 4, 19);
			newStudent.id =numericValidator("Please enter the student's ID number: ", 1, 9999);
			newStudent.seat = stringValidator("Please enter the student's seat location: ");
			newStudent.notes = stringValidator("Enter any additional notes for this student: ");
			printStudent(newStudent);
		} while (!userCheck("Does this look right to you ? [Y / N] ", "Great! Let's continue", "That's okay. Let's try again."));

		storage.students.push_back(newStudent);
		std::cout << "Student " << newStudent.firstName << " " << newStudent.lastName << " has been added successfully!" << std::endl;
		std::cout << std::endl;
		if (userCheck("Would you like to add another student? ",
			"Okay, let's add another",
			"Returning to main menu.")) {
			addStudent(storage);
		}
		else {
			mainMenu(storage);
		}
	}
}