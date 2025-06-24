#include "containers.h"
#include "helperFunctions.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <map>

namespace gradebookContainers {
    using namespace Helpers; 

    Classroom Classroom::createClassroom() {
        Classroom myClassroom;

        std::cout << "Let's get started by entering some basic information." << std::endl;

        do {
            myClassroom.title = stringValidator("Which honorific do your students address you by? ...");
            myClassroom.teacherFirstName = stringValidator("What is your first name? ");
            myClassroom.teacherLastName = stringValidator("What is your last name? ");
            myClassroom.gradeLevel = numericValidator<unsigned>("Which grade do you teach? [1–12]: ", 1, 12);

            std::cout << "This is " << myClassroom.title << " " << myClassroom.teacherFirstName << " "
                << myClassroom.teacherLastName << "'s grade " << myClassroom.gradeLevel << " book." << std::endl;

        } while (!userCheck(
            "Does this look right to you? [Y/N] ",
            "Great! Let's continue.",
            "That's okay. Let's try again."));

        return myClassroom;
    }
}

