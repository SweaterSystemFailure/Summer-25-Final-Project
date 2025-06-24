#pragma once
#include "helperFunctions.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <map>

namespace gradebookContainers {
	using namespace Helpers;
	class Classroom {
	public:
		std::string title;
		std::string teacherFirstName;
		std::string teacherLastName;
		unsigned gradeLevel;

		static Classroom createClassroom();
	};
}

	class Student {
	public:
		//biographical
		std::string firstName;
		std::string lastName;
		std::string pronouns;
		unsigned age;
		unsigned id;
		std::string seat;
		std::string notes;

		//grades
		char overallGrade;
		float gradePercent;
		
		//map keyed to assignment name. value is the score.
		std::map<std::string, float> assignmentScores;	
	};

	class assignment {
		std::string name;
		float pointsPossible;
	};

	struct globalStorage {
		std::vector<gradebookContainers::student> students; //stores students
		std::vector<gradebookContainers::assignment> assignments; //stores assignments
		classroom currentClass; //stores teacher & class info
	};
}