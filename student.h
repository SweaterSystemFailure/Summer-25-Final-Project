#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <map>

class student
{
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
	std::map<std::string, float> assignmentScores;	//keyed to assignment name. value is the score


};

