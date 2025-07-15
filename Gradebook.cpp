#include "Gradebook.h"
#include "Classroom.h"
#include "utilities.h"
#include <iostream>
#include <iomanip>


namespace gradebook {
    void Gradebook::createSchool(){
        Administrator myAdmin;
        std::string password;

        std::cout << "Let's get started by entering some basic infomation." << std::endl;

        do{
            myAdmin.setAdminTitle(stringValidator("Which honorific are you addressed by? "));
            myAdmin.setAdminLastName(stringValidator("What is your last name?"));
            myAdmin.setSchoolName(stringValidator("What is the name of your school?"));
        }while (!userCheck("Does this look right to you? [Y/N] ",
			"Great! Let's continue.",
			"That's okay. Let's try again."));
        
        do{
            password = stringValidator("Please select a strong password that is at least 8 characters long, and that contains a mix of upper and lower case characters, numbers, and special symbols.");            
        }while(!isStrongPassword(password));

        myAdmin.setAdminPassword(password);

        school.push_back(myAdmin);
    }

    
    void Gradebook::createClassroom() {
		Teacher myClassroom;

		std::cout << "Let's get started by entering some basic information." << std::endl;

		do {
			myClassroom.setTitle(stringValidator("Which honorific do your students address you by? "));
			myClassroom.setTeacherFirstName(stringValidator("What is your first name? "));
			myClassroom.setTeacherLastName(stringValidator("What is your last name? "));
			myClassroom.setGradeLevel(numericValidator<unsigned>("Which grade do you teach? [1�12]: ", 1, 12));
			myClassroom.printClassroom();

		} while (!userCheck(
			"Does this look right to you? [Y/N] ",
			"Great! Let's continue.",
			"That's okay. Let's try again."));

		currentClass = myClassroom;

		mainMenu();
	}

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

    if (teachers.empty()) {
        std::cout << "No teachers available to assign this student to.\n";
    } else {
        std::cout << "Assign student to which teacher?\n";
        for (size_t i = 0; i < teachers.size(); ++i) {
            std::cout << i + 1 << ". " << teachers[i].getTeacherFirstName() << " " << teachers[i].getTeacherLastName() << std::endl;
        }
        unsigned teacherChoice = numericValidator<unsigned>("Enter the number of the teacher: ", 1, teachers.size());
        teachers[teacherChoice - 1].addStudentToClassroom(newStudent);
    }

    std::cout << "Student " << newStudent.getFirstName() << " " << newStudent.getLastName()
        << " has been added successfully!" << std::endl;

    if (userCheck("Would you like to add another student? ",
        "Okay, let's add another.",
        "Returning to main menu.")) {
        addStudent();
    }
    else {
        mainMenu();
    }
}


	//Print Functions
	void Gradebook::printAllStudents() const {
		if (students.empty()) {
			std::cout << "No students in the system." << std::endl;
			return;
		}

		//Sortable vector
		std::vector<Student> sortedStudents = students;

		// Bubble sort for alpha by last name
		for (size_t i = 0; i < sortedStudents.size(); ++i) {
			for (size_t j = 0; j < sortedStudents.size() - 1 - i; ++j) {
				if (sortedStudents[j].getLastName() > sortedStudents[j + 1].getLastName() ||
					(sortedStudents[j].getLastName() == sortedStudents[j + 1].getLastName() && sortedStudents[j].getFirstName() > sortedStudents[j + 1].getFirstName())) {
					Student temp = sortedStudents[j];
					sortedStudents[j] = sortedStudents[j + 1];
					sortedStudents[j + 1] = temp;
				}
			}
		}

		//Prints list
		int count = 1;
		std::cout << "List of students:" << std::endl;
		for (const auto& s : sortedStudents) {
			std::cout << std::setw(2) << count++ << ". " << s.getFirstName() << " " << s.getLastName() << std::endl;
		}
	}
	void Gradebook::printAllAssignments() const {

	}
	void Gradebook::printClassReport() const {}
}