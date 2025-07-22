#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <map>
#include "Assignment.h"
#include "User.h"

namespace gradebook {

    /**
     * @class Student
     * @brief Represents a student in the gradebook system, inheriting from User.
     * Contains biographical data, grade info, assignment scores, and related functionality.
     */
    class Student : public User {
    private:
        // Biographical Variables
        std::string pronouns;               /**< Student's pronouns */
        unsigned age;                      /**< Student's age */
        unsigned gradelevel;               /**< Student's grade level */
        unsigned id;                      /**< Unique student ID */
        std::string seat;                 /**< Seating location */
        std::string notes;                /**< Additional notes */

        // Grade Variables
        char overallGrade;                /**< Letter grade */
        float gradePercent;               /**< Grade percentage */

        // Individual Student Assignment Container
        std::map<std::string, float> assignmentScores;  /**< Map of assignment names to scores */

    public:
        // === Mutators ===

        /**
         * @brief Sets the student's pronouns.
         * @param entry Pronouns string.
         */
        void setPronouns(const std::string& entry);

        /**
         * @brief Sets the student's age.
         * @param entry Age value.
         */
        void setAge(const unsigned& entry);

        /**
         * @brief Sets the student's grade level.
         * @param entry Grade level.
         */
        void setGradeLevel(const unsigned& entry);

        /**
         * @brief Sets the student's unique ID.
         * @param entry ID number.
         */
        void setID(const unsigned& entry);

        /**
         * @brief Sets the student's seat location.
         * @param entry Seat description.
         */
        void setSeat(const std::string& entry);

        /**
         * @brief Sets notes related to the student.
         * @param entry Notes string.
         */
        void setNotes(const std::string& entry);

        /**
         * @brief Sets the student's overall letter grade.
         * @param entry Letter grade.
         */
        void setOverallGrade(const char& entry);

        /**
         * @brief Sets the student's grade percentage.
         * @param entry Grade percentage.
         */
        void setGradePercent(const float& entry);

        /**
         * @brief Sets or updates the student's score for a specific assignment.
         * @param assignmentName Name of the assignment.
         * @param score Score achieved.
         */
        void setAssignmentScore(const std::string& assignmentName, float score);

        // === Accessors ===

        /**
         * @brief Gets the student's first name (overrides User).
         * @return First name string.
         */
        std::string getFirstName() const override;

        /**
         * @brief Gets the student's last name (overrides User).
         * @return Last name string.
         */
        std::string getLastName() const override;

        /**
         * @brief Gets the student's pronouns.
         * @return Pronouns string.
         */
        std::string getPronouns() const;

        /**
         * @brief Gets the student's age.
         * @return Age as unsigned.
         */
        unsigned getAge() const;

        /**
         * @brief Gets the student's grade level.
         * @return Grade level as unsigned.
         */
        unsigned getGradeLevel() const;

        /**
         * @brief Gets the student's unique ID.
         * @return ID number.
         */
        unsigned getID() const;

        /**
         * @brief Gets the student's seat location.
         * @return Seat string.
         */
        std::string getSeat() const;

        /**
         * @brief Gets any notes about the student.
         * @return Notes string.
         */
        std::string getNotes() const;

        /**
         * @brief Gets the student's password (overrides User).
         * @return Password string.
         */
        std::string getPassword() const override;

        /**
         * @brief Gets the student's overall letter grade.
         * @return Letter grade character.
         */
        char getOverallGrade() const;

        /**
         * @brief Gets the student's grade percentage.
         * @return Grade percentage as float.
         */
        float getGradePercent() const;

        /**
         * @brief Gets the map of assignment scores.
         * @return Const reference to assignment name-score map.
         */
        const std::map<std::string, float>& getAssignmentScores() const;

        // === Grade Calculation ===

        /**
         * @brief Calculates and updates the student's overall grade and percentage
         * based on the provided vector of assignments and their respective scores.
         * @param assignments Vector of Assignment objects to consider.
         */
        void calculateGrade(const std::vector<Assignment>& assignments);

        // === Print Functions ===

        /**
         * @brief Prints a summary of the student's information and grades to console.
         */
        void printStudent() const;

        /**
         * @brief Prints a detailed student report including assignment scores.
         */
        void printStudentReport() const;

        // === Export Functions ===

        /**
         * @brief Exports the student's report to a CSV file.
         */
        void exportStudentReportToCSV() const;

        // === Menu ===

        /**
         * @brief Provides the student's menu interface for interacting with the gradebook.
         * @param book Reference to the Gradebook instance.
         */
        void menu(Gradebook& book) override;

        /**
         * @brief Gets the role of the user (overrides User).
         * @return "Student" string literal.
         */
        std::string getRole() const override { return "Student"; };
    };
}