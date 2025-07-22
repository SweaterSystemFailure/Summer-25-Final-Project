#include "Gradebook.h"
#include "utilities.h"
#include <iostream>

using namespace gradebook;

/**
 * @brief Entry point of the Gradebook application.
 *
 * Creates a Gradebook instance and launches the welcome menu
 * which handles loading, login, and the main program flow.
 *
 * @return int Returns 0 upon successful program termination.
 */
int main()
{
    Gradebook gradebook;
    welcomeMenu(gradebook);

    return 0;
}