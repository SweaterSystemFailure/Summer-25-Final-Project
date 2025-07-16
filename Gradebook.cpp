#include "Gradebook.h"
#include "Administrator.h"
#include "Teacher.h"
#include "utilities.h"
#include <iostream>
#include <iomanip>


namespace gradebook {
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

    //Admin function
    void Gradebook::createSchool() {
        if (!school.empty()) {
            std::cout << "A school and administrator already exist in the system." << std::endl;
            std::cout << "School: " << school.front().getSchoolName() << std::endl;
            std::cout << "Administrator: " << school.front().getFirstName() << " " << school.front().getLastName() << std::endl;

            if (!userCheck("Would you like to overwrite this school and administrator? [Y/N] ",
                "Overwriting existing school and administrator.",
                "Keeping existing data.")) {
                return; 
            }

            school.clear();
        }

        Administrator myAdmin;
        std::string password;

        std::cout << "Let's get started by entering some basic information." << std::endl;

        do {
            myAdmin.setAdminTitle(stringValidator("Which honorific are you addressed by? "));
            myAdmin.setAdminFirstName(stringValidator("What is your first name? "));
            myAdmin.setAdminLastName(stringValidator("What is your last name? "));
            myAdmin.setSchoolName(stringValidator("What is the name of your school? "));
        } while (!userCheck("Does this look right to you? [Y/N] ",
            "Great! Let's continue.",
            "That's okay. Let's try again."));

        do {
            password = stringValidator("Please select a strong password that is at least 8 characters long, and that contains a mix of upper and lower case characters, numbers, and special symbols.");
        } while (!isStrongPassword(password));

        myAdmin.setPassword(password);

        school.push_back(myAdmin);

        std::cout << "School and administrator successfully created." << std::endl;
    }


    //Login Functions
    bool handlePassword(User& user) {
        std::string enteredPassword;

        if (user.getPassword().empty()) {
            std::cout << "No password set for your account. Please create one now." << std::endl;
            do {
                enteredPassword = stringValidator("Enter new password: ");
            } while (!isStrongPassword(enteredPassword));

            user.setPassword(enteredPassword);
            std::cout << "Password set successfully." << std::endl;
            return true;
        }
        else {
            enteredPassword = stringValidator("Enter your password: ");
            if (enteredPassword == user.getPassword()) {
                std::cout << "Password accepted." << std::endl;
                return true;
            }
            else {
                std::cout << "Incorrect password." << std::endl;
                return false;
            }
        }
    }

    bool Gradebook::login() {
        std::cout << "Select your role to log in:" << std::endl
            << "1. Student" << std::endl
            << "2. Teacher" << std::endl
            << "3. Administrator" << std::endl;

        int roleChoice = numericValidator<int>("Enter the number for your role: ", 1, 3);

        switch (roleChoice) {
        case 1: {
            unsigned studentID = numericValidator<unsigned>("Enter your Student ID: ", 1, 999999);

            Student* studentPtr = nullptr;
            for (auto& student : students) {
                if (student.getID() == studentID) {
                    studentPtr = &student;
                    break;
                }
            }
            if (!studentPtr) {
                std::cout << "Student ID not found." << std::endl;
                return false;
            }
            return handlePassword(*studentPtr);
        }
        case 2: {
            std::string firstName = stringValidator("Enter your first name: ");
            std::string lastName = stringValidator("Enter your last name: ");

            User* userPtr = nullptr;
            for (auto& teacher : teachers) {
                if (teacher.getTeacherFirstName() == firstName &&
                    teacher.getTeacherLastName() == lastName) {
                    userPtr = &teacher;
                    break;
                }
            }
            if (!userPtr) {
                std::cout << "Teacher not found." << std::endl;
                return false;
            }
            return handlePassword(*userPtr);
        }
        case 3: {
            std::string firstName = stringValidator("Enter your first name: ");
            std::string lastName = stringValidator("Enter your last name: ");

            User* userPtr = nullptr;
            for (auto& admin : school) {
                if (admin.getFirstName() == firstName &&
                    admin.getLastName() == lastName) {
                    userPtr = &admin;
                    break;
                }
            }
            if (!userPtr) {
                std::cout << "Administrator not found." << std::endl;
                return false;
            }
            return handlePassword(*userPtr);
        }
        default:
            std::cout << "Invalid selection." << std::endl;
            return false;
        }
    }

    //Save/Load Functions
    void Gradebook::autosaveToggle() {
        autosaveEnabled = !autosaveEnabled;
        std::cout << "Autosave " << (autosaveEnabled ? "enabled." : "disabled.") << std::endl;
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