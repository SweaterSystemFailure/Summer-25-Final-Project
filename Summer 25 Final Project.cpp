#include "containers.h"
#include "helperFunctions.h"
#include "menus.h"
#include <iostream>

using namespace gradebook;

int main()
{
    Gradebook::createClassroom();
    mainMenu();

    return 0;
}