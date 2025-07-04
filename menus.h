#pragma once
#include "containers.h"
#include "helperFunctions.h"
#include "menus.h"
#include <iostream>

using namespace gradebook {

	void welcome();
	void mainMenu(Gradebook& storage);
	void exitMenu();
}