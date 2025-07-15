#include "Assignment.h"
#include <iostream>

namespace gradebook {
    //Assignments
	//Mutators
	void Assignment::setAssignmentName(const std::string& entry) {
		assignmentName = entry;
	}
	void Assignment::setAssignmentDescription(const std::string& entry) {
		assignmentDescription = entry;
	}
	void Assignment::setPointsPossible(float entry) {
		pointsPossible = entry;
	}

	//Accessors
	std::string Assignment::getAssignmentName() const {
		return assignmentName;
	}
	std::string Assignment::getAssignmentDescription() const {
		return assignmentDescription;
	}
	float Assignment::getPointsPossible() const {
		return pointsPossible;
	}

	//Print Function
	void Assignment::printAssignments() const {}
}