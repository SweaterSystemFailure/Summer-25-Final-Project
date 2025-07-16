#include "Gradebook.h"
#include "Teacher.h"
#include "utilities.h"
#include <iostream>
#include <iomanip>


namespace gradebook {
    //Mutators
    void Gradebook::setAutosaveEnabled(const bool& entry) {
        autosaveEnabled = entry;
    }
    
    //Accessors
    std::vector<Teacher>& Gradebook::getTeachers() {
        return teachers;
    }

    const std::vector<Teacher>& Gradebook::getTeachers() const {
        return teachers;
    }

    std::vector<Student>& Gradebook::getStudents() {
        return students;
    }
    const std::vector<Student>& Gradebook::getStudents() const; {
        return students;
    }

    bool Gradebook::getAutosaveEnabled() const {
        return autosaveEnabled;
    }

    void Gradebook::createSchool() {
        Administrator myAdmin;
        std::string password;

        std::cout << "Let's get started by entering some basic infomation." << std::endl;

        do {
            myAdmin.setAdminTitle(stringValidator("Which honorific are you addressed by? "));
            myAdmin.setAdminLastName(stringValidator("What is your last name?"));
            myAdmin.setSchoolName(stringValidator("What is the name of your school?"));
        } while (!userCheck("Does this look right to you? [Y/N] ",
            "Great! Let's continue.",
            "That's okay. Let's try again."));

        do {
            password = stringValidator("Please select a strong password that is at least 8 characters long, and that contains a mix of upper and lower case characters, numbers, and special symbols.");
        } while (!isStrongPassword(password));

        myAdmin.setAdminPassword(password);

        school.push_back(myAdmin);
    }
    //Save/Load Functions
    void autosaveToggle(Gradebook& gradebook) {
        if (autosaveToggle) {
            gradebook.setAutosaveEnabled(false);
        }
        else if (!autosaveToggle) {
            gradebook.setAutosaveEnabled(true);
        }
    }


    //Print Functions
    void Gradebook::printAllStudents() const {
        if (students.empty()) {
            std::cout << "No students in the system." << std::endl;
            return;
        }

        std::vector<Student> sortedStudents = students;

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