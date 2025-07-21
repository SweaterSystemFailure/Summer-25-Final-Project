#include "Administrator.h"
#include "Gradebook.h"
#include "utilities.h"
#include "Student.h"
#include "Teacher.h"
#include <fstream>
#include <iomanip> 
#include <iostream>
#include <sstream>
#include <vector> 

namespace gradebook {
    // Mutators
    void Administrator::setAdminTitle(const std::string& entry) {
        adminTitle = entry;
    }

    void Administrator::setSchoolName(const std::string& entry) {
        schoolName = entry;
    }

    // Accessors
    std::string Administrator::getAdminTitle() const {
        return adminTitle;
    }

    std::string Administrator::getFirstName() const {
        return firstName;
    }

    std::string Administrator::getLastName() const {
        return lastName;
    }

    std::string Administrator::getSchoolName() const {
        return schoolName;
    }

    std::string Administrator::getPassword() const {
        return password;
    }

    //Add Students & Teachers
    void Administrator::createClassroom(Gradebook& gradebook) {
        Teacher newTeacher;

        std::cout << "Let's get started by entering some basic information." << std::endl;
        bool addMore = true;
        while (addMore) {

            do {
                newTeacher.setTitle(stringValidator("Which honorific do your students address you by? "));
                newTeacher.setFirstName(stringValidator("What is your first name? "));
                newTeacher.setLastName(stringValidator("What is your last name? "));
                newTeacher.setGradeLevel(numericValidator<unsigned>("Which grade do you teach? [1–12]: ", 1, 12));
                newTeacher.printClassroomReport();

            } while (!userCheck(
                "Does this look right to you? [Y/N] ",
                "Great! Let's continue.",
                "That's okay. Let's try again."));

            gradebook.getTeachers().push_back(newTeacher);

            std::cout << "Teacher " << newTeacher.getFirstName() << " " << newTeacher.getLastName()
                << " has been added successfully!" << std::endl;

            addMore = userCheck("Would you like to add another teacher? ",
                "Okay, let's add another.",
                "Returning to menu.");
        }
        menu(gradebook);
    }

    void Administrator::addStudent(Gradebook& gradebook) {
        auto& teachers = gradebook.getTeachers();

        if (teachers.empty()) {
            std::cout << "No teachers available to assign this student to. You must add teachers first." << std::endl;
            return;
        }

        bool addMore = true;
        while (addMore) {
            Student newStudent;

            do {
                newStudent.setFirstName(stringValidator("Please enter the student's first name: "));
                newStudent.setLastName(stringValidator("Please enter the student's last name: "));
                newStudent.setPronouns(stringValidator("Please enter the student's pronouns: "));
                newStudent.setAge(numericValidator("Please enter the student's age: ", 4, 19));
                newStudent.setGradeLevel(numericValidator("Please enter the student's grade: ", 1, 12));
                newStudent.setID(numericValidator("Please enter the student's ID number: ", 1, 999999));
                newStudent.setSeat(stringValidator("Please enter the student's seat location: "));
                newStudent.setNotes(stringValidator("Enter any additional notes for this student: "));
                newStudent.printStudent();
            } while (!userCheck("Does this look right to you ? [Y / N] ", "Great! Let's continue", "That's okay. Let's try again."));

            std::vector<Teacher*> eligibleTeachers;
            for (auto& teacher : teachers) {
                if (teacher.getGradeLevel() == newStudent.getGradeLevel()) {
                    eligibleTeachers.push_back(&teacher);
                }
            }

            if (eligibleTeachers.empty()) {
                std::cout << "No teachers available for grade " << newStudent.getGradeLevel()
                    << ". Student cannot be assigned." << std::endl;
                std::cout << "Student will NOT be added." << std::endl;
            }
            else {
                std::cout << "Assign student to which teacher?" << std::endl;
                for (size_t i = 0; i < eligibleTeachers.size(); ++i) {
                    std::cout << i + 1 << ". "
                        << eligibleTeachers[i]->getFirstName() << " "
                        << eligibleTeachers[i]->getLastName() << std::endl;
                }

                unsigned teacherChoice = numericValidator<unsigned>("Enter the number of the teacher: ", 1, eligibleTeachers.size());
                Teacher* chosenTeacher = eligibleTeachers[teacherChoice - 1];

                gradebook.getStudents().push_back(newStudent);
                chosenTeacher->addStudentToClassroom(newStudent);

                std::cout << "Student " << newStudent.getFirstName() << " " << newStudent.getLastName()
                    << " has been added and assigned to " << chosenTeacher->getFirstName() << " "
                    << chosenTeacher->getLastName() << " successfully!" << std::endl;
            }
            
            if (gradebook.getAutosaveEnabled()) {
                gradebook.serializeAndSave();
            }

            addMore = userCheck("Would you like to add another student? ",
                "Okay, let's add another.",
                "Returning to menu.");
        }
        menu(gradebook);
    }

    void Administrator::printSchoolReport(Gradebook& gradebook) const {
        const auto& teachers = gradebook.getTeachers();

        if (teachers.empty()) {
            std::cout << "There are no teachers currently in the system." << std::endl;
            return;
        }

        std::cout << "===== School-Wide Student Report =====" << std::endl << std::endl;

        for (const auto& teacher : teachers) {
            std::cout << teacher.getTitle() << " " << teacher.getFirstName()
                << " " << teacher.getLastName()
                << " (Grade " << teacher.getGradeLevel() << ")" << std::endl;

            const auto& students = teacher.getClassroomStudents();

            if (students.empty()) {
                std::cout << "  No students assigned to this teacher." << std::endl << std::endl;
                continue;
            }

            std::cout << std::left
                << std::setw(16) << "First Name:"
                << std::setw(16) << "Last Name:"
                << std::setw(9) << "Age:"
                << std::setw(11) << "ID:"
                << std::setw(16) << "Percent Grade:"
                << std::setw(13) << "Letter Grade:"
                << std::endl;

            std::cout << std::string(81, '-') << "\n";

            for (const Student& student : students) {
                std::cout << std::left
                    << std::setw(16) << student.getFirstName()
                    << std::setw(16) << student.getLastName()
                    << std::setw(9) << student.getAge()
                    << std::setw(11) << student.getID()
                    << std::setw(16) << std::fixed << std::setprecision(2) << student.getGradePercent()
                    << std::setw(13) << student.getOverallGrade()
                    << "\n";
            }
            std::cout << std::endl;
        }
    }

    void Administrator::saveSchoolReportToCSV(Gradebook& gradebook) {
        std::ofstream file("SchoolReport.csv");

        if (!file.is_open()) {
            std::cerr << "Failed to create SchoolReport.csv" << std::endl;
            return;
        }

        file << "Teacher,Student First Name,Student Last Name,Pronouns,Age,Grade Level,ID,Seat,Notes,Grade %,Letter Grade" << std::endl;

        const auto& teachers = gradebook.getTeachers();

        for (const auto& teacher : teachers) {
            std::string teacherName = teacher.getFirstName() + " " + teacher.getLastName();
            const auto& students = teacher.getClassroomStudents();

            for (const auto& student : students) {
                file << std::quoted(teacherName) << ","
                    << std::quoted(student.getFirstName()) << ","
                    << std::quoted(student.getLastName()) << ","
                    << std::quoted(student.getPronouns()) << ","
                    << student.getAge() << ","
                    << student.getGradeLevel() << ","
                    << student.getID() << ","
                    << std::quoted(student.getSeat()) << ","
                    << std::quoted(student.getNotes()) << ","
                    << std::fixed << std::setprecision(2) << student.getGradePercent() << ","
                    << student.getOverallGrade()
                    << std::endl;
            }
        }

        file.close();
        std::cout << "School data exported successfully to SchoolReport.csv." << std::endl;
    }
    
    void Administrator::menu(Gradebook& gradebook) {
            while (true) {
                std::cout << "\n=== Administrator Menu ===\n";
                std::cout << "1. Add Teacher." << std::endl;
                std::cout << "2. Add Student." << std::endl;
                std::cout << "3. Overwrite Administrator Profile." << std::endl;
                std::cout << "4. Print School Report" << std::endl;
                std::cout << "5. Save School Report to CSV" << std::endl;
                std::cout << "6. Save All Work." << std::endl;
                std::cout << "7. Toggle Autosave." << std::endl;
                std::cout << "8. Log Out." << std::endl;

                unsigned choice = numericValidator<unsigned>("Choose an option [1-8]: ", 1, 8);

                switch (choice) {
                case 1:
                   createClassroom(gradebook);
                    break;
                case 2:
                    addStudent(gradebook);
                    break;
                case 3:
                    gradebook.createSchool();
                    break;
                case 4:
                    printSchoolReport(gradebook);
                    break;
                case 5:
                    saveSchoolReportToCSV(gradebook);
                    break;
                case 6:
                    gradebook.serializeAndSave();
                    break;
                case 7:
                    gradebook.autosaveToggle();
                    break;
                case 8:
                    closeMenu(gradebook);
                    break;
                default:
                    std::cout << "Invalid selection. Please try again." << std::endl;
                   return;
                }
            }
    }
}