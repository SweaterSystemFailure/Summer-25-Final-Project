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
    // === Mutators ===

    /**
     * @brief Sets the administrator's title.
     * @param entry The new title to assign.
     */
    void Administrator::setAdminTitle(const std::string& entry) {
        adminTitle = entry;
    }

    /**
     * @brief Sets the name of the school.
     * @param entry The school name to assign.
     */
    void Administrator::setSchoolName(const std::string& entry) {
        schoolName = entry;
    }

    // === Accessors ===

    /**
     * @brief Retrieves the administrator's title.
     * @return Current administrator title.
     */
    std::string Administrator::getAdminTitle() const {
        return adminTitle;
    }

    /**
     * @brief Retrieves the administrator's first name.
     * @return First name string.
     */
    std::string Administrator::getFirstName() const {
        return firstName;
    }

    /**
     * @brief Retrieves the administrator's last name.
     * @return Last name string.
     */
    std::string Administrator::getLastName() const {
        return lastName;
    }

    /**
     * @brief Retrieves the name of the school.
     * @return School name string.
     */
    std::string Administrator::getSchoolName() const {
        return schoolName;
    }

    /**
     * @brief Retrieves the administrator's password.
     * @return Password string.
     */
    std::string Administrator::getPassword() const {
        return password;
    }

    // === Other Member Functions ===

    /**
     * @brief Creates a new teacher classroom by gathering user input.
     * @param gradebook Reference to the Gradebook instance.
     *
     * Repeatedly prompts for teacher details until confirmed,
     * then adds the teacher to the gradebook.
     */
    void Administrator::createClassroom(Gradebook& gradebook) {
        Teacher newTeacher;

        std::cout << "Let's get started by entering some basic information about your teacher." << std::endl;
        bool addMore = true;
        while (addMore) {

            do {
                newTeacher.setTitle(stringValidator("Which honorific do their students address them by? "));
                newTeacher.setFirstName(stringValidator("What is their first name? "));
                newTeacher.setLastName(stringValidator("What is their last name? "));
                newTeacher.setGradeLevel(numericValidator<unsigned>("Which grade do they teach? [1-12]: ", 1, 12));
                newTeacher.printClassroomReport();

            } while (!userCheck(
                "Does this look right to you? [Y/N] ",
                "Great! Let's continue.",
                "That's okay. Let's try again."));

            gradebook.getTeachers().push_back(newTeacher);

            std::cout << "Teacher " << newTeacher.getFirstName() << " " << newTeacher.getLastName()
                << " has been added successfully!" << std::endl;

            addMore = userCheck("Would you like to add another teacher? [Y / N]",
                "Okay, let's add another.",
                "Returning to menu.");
        }
        menu(gradebook);
    }

    /**
     * @brief Adds a new student to a teacher's classroom after collecting details.
     * @param gradebook Reference to the Gradebook instance.
     *
     * Prompts for student info, validates it, assigns student to teacher based on grade level,
     * and saves to the gradebook.
     */
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
                newStudent.setAge(numericValidator<unsigned>("Please enter the student's age: ", 4, 19));
                newStudent.setGradeLevel(numericValidator<unsigned>("Please enter the student's grade: ", 1, 12));
                newStudent.setID(numericValidator<unsigned>("Please enter the student's ID number: ", 1, 999999));
                newStudent.setSeat(stringValidator("Please enter the student's seat location: "));
                newStudent.setNotes(stringValidator("Enter any additional notes for this student: "));
                newStudent.printStudent();
            } while (!userCheck(
                "Does this look right to you? [Y/N] ",
                "Great! Let's continue.",
                "That's okay. Let's try again."
            ));

            std::vector<Teacher*> eligible;
            for (auto& t : teachers)
                if (t.getGradeLevel() == newStudent.getGradeLevel())
                    eligible.push_back(&t);

            if (eligible.empty()) {
                std::cout << "No teacher for grade " << newStudent.getGradeLevel()
                    << ". Student will NOT be added." << std::endl;
            }
            else {
                std::cout << "Assign student to which teacher?\n";
                for (size_t i = 0; i < eligible.size(); ++i) {
                    std::cout << "  " << (i + 1) << ". "
                        << eligible[i]->getFirstName() << " "
                        << eligible[i]->getLastName() << std::endl;
                }
                unsigned choice = numericValidator<unsigned>(
                    "Enter the number of the teacher: ", 1, eligible.size()
                );
                Teacher* chosen = eligible[choice - 1];

                auto studentPtr = std::make_unique<Student>(std::move(newStudent));
                Student* stored = studentPtr.get();
                gradebook.getStudents().push_back(std::move(studentPtr));

                chosen->addStudentToClassroom(stored);

                std::cout << "Student "
                    << stored->getFirstName() << " "
                    << stored->getLastName()
                    << " assigned to "
                    << chosen->getFirstName() << " "
                    << chosen->getLastName() << "!" << std::endl;
            }

            if (gradebook.isAutosaveEnabled()) {
                gradebook.serializeAndSave();
            }

            addMore = userCheck(
                "Would you like to add another student? [Y/N] ",
                "Okay, let's add another.",
                "Returning to menu."
            );
        }

        menu(gradebook);
    }

    /**
     * @brief Prints a detailed school-wide report of all teachers and their students.
     * @param gradebook Reference to the Gradebook instance.
     */
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

            std::cout << std::string(81, '-') << std::endl;

            for (const Student* student : students) {
                if (!student) continue;

                std::cout << std::left
                    << std::setw(16) << student->getFirstName()
                    << std::setw(16) << student->getLastName()
                    << std::setw(9) << student->getAge()
                    << std::setw(11) << student->getID()
                    << std::setw(16) << std::fixed << std::setprecision(2) << student->getGradePercent()
                    << std::setw(13) << student->getOverallGrade()
                    << std::endl;
            }
            std::cout << std::endl;
        }
    }

    /**
     * @brief Exports the school report to a CSV file named "SchoolReport.csv".
     * @param gradebook Reference to the Gradebook instance.
     *
     * Writes all teachers and their students' information in CSV format.
     */
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

            for (const auto* studentPtr : students) {  // students are now pointers
                if (!studentPtr) continue;  // just in case

                const Student& student = *studentPtr;  // dereference pointer

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

    /**
     * @brief Main menu loop for administrator actions.
     * @param gradebook Reference to the Gradebook instance.
     *
     * Presents options and calls relevant Administrator functions based on user input.
     */
    void Administrator::menu(Gradebook& gradebook) {
        while (true) {
            std::cout << "=== Administrator Menu ===" << std::endl;
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
                welcomeMenu(gradebook);
                break;
            default:
                std::cout << "Invalid selection. Please try again." << std::endl;
                return;
            }
        }
    }
}