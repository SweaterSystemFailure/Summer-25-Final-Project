#include "Student.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <map>
#include "Teacher.h"
#include "User.h"
#include "utilities.h"

namespace gradebook {

    // === Mutators ===

    /**
     * @brief Sets the student's pronouns.
     * @param entry Pronouns string.
     */
    void Student::setPronouns(const std::string& entry) {
        pronouns = entry;
    }

    /**
     * @brief Sets the student's age.
     * @param entry Age value.
     */
    void Student::setAge(const unsigned& entry) {
        age = entry;
    }

    /**
     * @brief Sets the student's grade level.
     * @param entry Grade level.
     */
    void Student::setGradeLevel(const unsigned& entry) {
        gradelevel = entry;
    }

    /**
     * @brief Sets the student's unique ID.
     * @param entry ID number.
     */
    void Student::setID(const unsigned& entry) {
        id = entry;
    }

    /**
     * @brief Sets the student's seat location.
     * @param entry Seat description.
     */
    void Student::setSeat(const std::string& entry) {
        seat = entry;
    }

    /**
     * @brief Sets notes related to the student.
     * @param entry Notes string.
     */
    void Student::setNotes(const std::string& entry) {
        notes = entry;
    }

    /**
     * @brief Sets the student's overall letter grade.
     * @param entry Letter grade.
     */
    void Student::setOverallGrade(const char& entry) {
        overallGrade = entry;
    }

    /**
     * @brief Sets the student's grade percentage.
     * @param entry Grade percentage.
     */
    void Student::setGradePercent(const float& entry) {
        gradePercent = entry;
    }

    /**
     * @brief Sets or updates the student's score for a specific assignment.
     * @param assignmentName Name of the assignment.
     * @param score Score achieved.
     */
    void Student::setAssignmentScore(const std::string& assignmentName, float score) {
        assignmentScores[assignmentName] = score;
    }

    // === Accessors ===

    /**
     * @brief Gets the student's first name (overrides User).
     * @return First name string.
     */
    std::string Student::getFirstName() const {
        return firstName;
    }

    /**
     * @brief Gets the student's last name (overrides User).
     * @return Last name string.
     */
    std::string Student::getLastName() const {
        return lastName;
    }

    /**
     * @brief Gets the student's pronouns.
     * @return Pronouns string.
     */
    std::string Student::getPronouns() const {
        return pronouns;
    }

    /**
     * @brief Gets the student's age.
     * @return Age as unsigned.
     */
    unsigned Student::getAge() const {
        return age;
    }

    /**
     * @brief Gets the student's grade level.
     * @return Grade level as unsigned.
     */
    unsigned Student::getGradeLevel() const {
        return gradelevel;
    }

    /**
     * @brief Gets the student's unique ID.
     * @return ID number.
     */
    unsigned Student::getID() const {
        return id;
    }

    /**
     * @brief Gets the student's seat location.
     * @return Seat string.
     */
    std::string Student::getSeat() const {
        return seat;
    }

    /**
     * @brief Gets any notes about the student.
     * @return Notes string.
     */
    std::string Student::getNotes() const {
        return notes;
    }

    /**
     * @brief Gets the student's password (overrides User).
     * @return Password string.
     */
    std::string Student::getPassword() const {
        return password;
    }

    /**
     * @brief Gets the student's overall letter grade.
     * @return Letter grade character.
     */
    char Student::getOverallGrade() const {
        return overallGrade;
    }

    /**
     * @brief Gets the student's grade percentage.
     * @return Grade percentage as float.
     */
    float Student::getGradePercent() const {
        return gradePercent;
    }

    /**
     * @brief Gets the map of assignment scores.
     * @return Const reference to assignment name-score map.
     */
    const std::map<std::string, float>& Student::getAssignmentScores() const {
        return assignmentScores;
    }

    // === Grade Calculation ===

    /**
     * @brief Calculates and updates the student's overall grade and percentage
     * based on the provided vector of assignments and their scores.
     * @param assignments Vector of Assignment objects to consider.
     */
    void Student::calculateGrade(const std::vector<Assignment>& assignments) {
        float totalPointsPossible = 0.0f;
        float totalPointsScored = 0.0f;

        for (const auto& a : assignments) {
            totalPointsPossible += a.getPointsPossible();
            auto it = assignmentScores.find(a.getAssignmentName());
            if (it != assignmentScores.end()) {
                totalPointsScored += it->second;
            }
        }

        if (totalPointsPossible > 0.0f) {
            gradePercent = (totalPointsScored / totalPointsPossible) * 100.0f;
        }
        else {
            gradePercent = 0.0f;
        }

        overallGrade = (gradePercent >= 90.0f) ? 'A'
            : (gradePercent >= 80.0f) ? 'B'
            : (gradePercent >= 70.0f) ? 'C'
            : (gradePercent >= 60.0f) ? 'D'
            : 'F';
    }

    // === Print Functions ===

    /**
     * @brief Prints the student's basic profile information.
     */
    void Student::printStudent() const {
        std::cout << "Student Profile\n";
        std::cout << "------------------------\n";
        std::cout << "First Name      : " << firstName << std::endl;
        std::cout << "Last Name       : " << lastName << std::endl;
        std::cout << "Pronouns        : " << pronouns << std::endl;
        std::cout << "Age             : " << age << std::endl;
        std::cout << "Student ID      : " << id << std::endl;
        std::cout << "Seat Location   : " << seat << std::endl;
        std::cout << "Notes           : " << notes << std::endl;
        std::cout << "------------------------\n" << std::endl;
    }

    /**
     * @brief Prints a detailed report of the student including assignment scores and overall grade.
     */
    void Student::printStudentReport() const {
        // printStudent() shows name, ID, seat, notes, etc.
        printStudent();

        std::cout << std::endl
            << "=== Assignment Scores ===" << std::endl;

        if (assignmentScores.empty()) {
            std::cout << "No assignments have been graded yet.\n";
        }
        else {
            for (const auto& [name, score] : assignmentScores) {
                std::cout << std::left << std::setw(20) << name
                    << "Score: " << std::fixed << std::setprecision(2)
                    << score << " pts\n";
            }
        }

        std::cout << std::endl
            << "Overall Grade: " << overallGrade
            << " (" << std::fixed << std::setprecision(2)
            << gradePercent << "%)\n";

        if (userCheck(
            "Would you like to export this student report to CSV? [Y/N] ",
            "Exporting student report to CSV...",
            "Skipping export.")) {
            exportStudentReportToCSV();
        }
    }

    // === Export ===

    /**
     * @brief Exports the student's detailed report to a CSV file named with their last and first name.
     */
    void Student::exportStudentReportToCSV() const {
        // Use a file name that includes the student's full name or ID to keep it unique
        std::string filename = lastName + "_" + firstName + "_Report.csv";

        std::ofstream file(filename);
        if (!file) {
            std::cerr << "Failed to open file for writing: " << filename << std::endl;
            return;
        }

        // Write biographical info
        file << "First Name," << firstName << "\n";
        file << "Last Name," << lastName << "\n";
        file << "Pronouns," << pronouns << "\n";
        file << "Age," << age << "\n";
        file << "Student ID," << id << "\n";
        file << "Seat Location," << seat << "\n";
        file << "Notes," << notes << "\n\n";

        // Write assignment scores header
        file << "Assignment,Score\n";

        if (assignmentScores.empty()) {
            file << "No assignments graded yet,\n";
        }
        else {
            for (const auto& pair : assignmentScores) {
                file << pair.first << "," << std::fixed << std::setprecision(2) << pair.second << "\n";
            }
        }

        file << "\nOverall Grade," << overallGrade << "\n";
        file << "Grade Percent," << std::fixed << std::setprecision(2) << gradePercent << "\n";

        file.close();

        std::cout << "Student report exported successfully to " << filename << std::endl;
    }

    // === Menu ===

    /**
     * @brief Displays the student menu and handles user input for profile viewing, grade viewing, or logout.
     * @param gradebook Reference to the Gradebook instance.
     */
    void Student::menu(Gradebook& gradebook) {
        while (true) {
            std::cout << "\n=== Student Menu ===\n";
            std::cout << "1. View My Profile." << std::endl;
            std::cout << "2. View My Grades." << std::endl;
            std::cout << "3. Log Out." << std::endl;

            unsigned choice = numericValidator<unsigned>("Choose an option [1-3]: ", 1, 3);

            switch (choice) {
            case 1:
                printStudent();
                break;
            case 2:
                printStudentReport();
                break;
            case 3:
                welcomeMenu(gradebook);
                return;
            default:
                std::cout << "Invalid selection. Please try again.\n";
                break;
            }
        }
    }
}