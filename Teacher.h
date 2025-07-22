#pragma once
#include "User.h"
#include "Assignment.h"
#include <string>
#include <vector>

namespace gradebook {

	class Student;
	class Gradebook;

	/**
	 * @class Teacher
	 * @brief Represents a teacher in the gradebook system. Inherits from User.
	 *
	 * A Teacher can manage assignments, grade students, and access classroom reports.
	 */
	class Teacher : public User {
	private:
		std::string title;            ///< Professional title (e.g., Mr., Ms., Dr.)
		unsigned gradeLevel;         ///< Grade level taught by the teacher
		std::vector<Student*> students;      ///< Pointers to students in the teacher's class
		std::vector<Assignment> assignments; ///< Assignments created by the teacher

	public:
		/** @brief Sets the teacher's title. */
		void setTitle(const std::string& entry);

		/** @brief Sets the grade level taught by the teacher. */
		void setGradeLevel(const unsigned& entry);

		/** @brief Gets the teacher's title. */
		std::string getTitle() const;

		/** @brief Gets the teacher's first name (inherited override). */
		std::string getFirstName() const override;

		/** @brief Gets the teacher's last name (inherited override). */
		std::string getLastName() const override;

		/** @brief Gets the teacher's grade level. */
		unsigned getGradeLevel() const;

		/** @brief Gets the teacher's password (inherited override). */
		std::string getPassword() const override;

		/** @brief Gets a reference to the assignments vector. */
		std::vector<Assignment>& getAssignments();

		/** @brief Gets a const reference to the assignments vector. */
		const std::vector<Assignment>& getAssignments() const;

		/** @brief Gets a reference to the teacher’s classroom students. */
		std::vector<Student*>& getClassroomStudents();

		/** @brief Gets a const reference to the classroom students. */
		const std::vector<Student*>& getClassroomStudents() const;

		/**
		 * @brief Adds a student to the teacher’s classroom.
		 * @param student Pointer to the student to add.
		 */
		void addStudentToClassroom(Student* student);

		/**
		 * @brief Enters grades for students in the classroom.
		 * @param assignments Vector of assignments to grade.
		 * @param gradebook Reference to the central Gradebook object.
		 */
		void enterGrades(const std::vector<Assignment>& assignments, Gradebook& gradebook);

		/**
		 * @brief Recalculates and updates all student scores in the classroom.
		 * @param assignments Vector of assignments to consider when scoring.
		 */
		void scoreAllStudents(const std::vector<Assignment>& assignments);

		/**
		 * @brief Adds a new assignment to the classroom.
		 * @param assignments Reference to a shared vector of assignments.
		 * @param gradebook Reference to the central Gradebook object.
		 */
		void addAssignment(std::vector<Assignment>& assignments, Gradebook& gradebook);

		/** @brief Prints a summary report of the entire classroom. */
		void printClassroomReport() const;

		/** @brief Prints details for all students in the classroom. */
		void printAllStudents() const;

		/** @brief Prints details of all assignments created by the teacher. */
		void printAllAssignments() const;

		/** @brief Exports a full classroom report to a CSV file. */
		void exportClassroomReportToCSV() const;

		/** @brief Exports assignment scores for all students to a CSV file. */
		void exportAssignmentScoresToCSV() const;

		/**
		 * @brief Displays the teacher's interactive menu.
		 * @param gradebook Reference to the central Gradebook object.
		 */
		void menu(Gradebook& gradebook) override;

		/** @brief Returns the user role as a string ("Teacher"). */
		std::string getRole() const override { return "Teacher"; }
	};

}