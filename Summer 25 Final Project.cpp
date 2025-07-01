#include "containers.h"
#include "helperFunctions.h"
#include "menus.h"
#include <iostream>

using namespace Helpers;
using namespace gradebookContainers;

int main()
{
    Classroom::createClassroom();
    mainMenu();

    return 0;
}