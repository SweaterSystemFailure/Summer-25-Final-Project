#include "Gradebook.h"
#include "utilities.h"
#include <iostream>

using namespace gradebook;

int main()
{
    Gradebook gradebook;
    gradebook.deserializeAndLoad();
    welcomeMenu(gradebook);

    return 0;
}