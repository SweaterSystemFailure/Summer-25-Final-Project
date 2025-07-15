#pragma once
#include <iostream>

namespace gradebook{
    class Assignment {
	private:
		std::string assignmentName;
		std::string assignmentDescription;
		float pointsPossible;
	public:
		//Mutators
		void setAssignmentName(const std::string& entry);
		void setAssignmentDescription(const std::string& entry);
		void setPointsPossible(float entry);
		
		//Accessors
		std::string getAssignmentName() const;
		std::string getAssignmentDescription() const;
		float getPointsPossible() const;

		//Print Function
		void printAssignments() const;
	};
}