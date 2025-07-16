#pragma once
#include "Student.h"
#include <string>
#include <vector>
#include "Assignment.h"
#include "Gradebook.h"

namespace gradebook {
	class Teacher : public User {
	private:
		std::string title;
		std::string teacherFirstName;
		std::string teacherLastName;
		unsigned gradeLevel;
		std::string password;

		std::vector<Student> students;
		std::vector<Assignment> assignments;
	public:
		// Mutators
		void setTitle(const std::string& entry);
		void setTeacherFirstName(const std::string& entry);
		void setTeacherLastName(const std::string& entry);
		void setGradeLevel(const unsigned& entry);
		void setPassword(const std::string entry);

		// Accessors
		std::string getTitle() const;
		std::string getFirstName() const;
		std::string getLastName() const override;
		unsigned getGradeLevel() const;
		std::string getPassword() const override;
		std::vector<Student>& getClassroomStudents();
		const std::vector<Student>& getClassroomStudents() const;

		// Assignment Construction and Grading
		void addStudentToClassroom(const Student& student);
		void enterGrades(const std::vector<Assignment>& assignments, Gradebook& gradebook);
		void scoreAllStudents(const std::vector<Assignment>& assignments);
		void addAssignment(std::vector<Assignment>& assignments, Gradebook& gradebook);

		// Print
		void printClassroomReport() const;
		void printAllStudents() const;
		void printAllAssignments() const;

		//Export
		void exportClassroomReportToCSV() const;
		void exportAssignmentScoresToCSV() const;

		// Menu override
		void menu(Gradebook& gradebook) override;
		std::string getRole() const override { return "Teacher"; }
	};
}
