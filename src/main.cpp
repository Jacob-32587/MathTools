#include "../include/MTQuadraticFormula.hpp"
#include "../include/MTIO.hpp"
#include "../include/MTBaseConvertor.hpp"
#include "../include/MTString.hpp"
#include "../include/MTLinkedList.hpp"
#include "../include/MTMath.hpp"
#include <stdio.h>
#include <string.h>
#include <chrono>
#include <iostream>
#include <math.h>

#define MT_MAIN_BUFFER_SIZE 32

#define MT_QUADRATIC_STR "QUAD"
#define MT_QUADRATIC_STR_SIZE 4

#define MT_CONVERT_BASE_STR "BASE CONVERT"
#define MT_CONVERT_BASE_STR_SIZE 12

#define MT_HELP_STR "HELP"
#define MT_HELP_STR_SIZE 4

#define MT_HELP_TXT "\
\n----------------------------------------------------------------------------------------\n\
Type \"quad\" to use the quadratic formula calculator\n\
Type \"base convert\" to convert from one base to another\n\
Type \"quit\" to exit the program (this can be typed at anytime to exit a program)\n\
----------------------------------------------------------------------------------------\n"

static bool MT_QUIT_FLAG = false;

int main()
{

	char buff[MT_MAIN_BUFFER_SIZE]; // Hold user input
	while (!MT_QUIT_FLAG)
	{
		printf("\nProgram/command to enter: "); // Prompt user to enter data

		getInput(buff, MT_MAIN_BUFFER_SIZE); // Get user input
		strToUpper(buff);							 // Not case sensetive

		// Execute command/program user has specified in the input
		if (strncmp(buff, MT_ESCAPE, MT_ESCAPE_SIZE) == 0)
			MT_QUIT_FLAG = true;
		else if (strncmp(buff, MT_QUADRATIC_STR, MT_QUADRATIC_STR_SIZE) == 0)
			quadraticFormula();
		else if (strncmp(buff, MT_CONVERT_BASE_STR, MT_CONVERT_BASE_STR_SIZE) == 0)
			baseConvertor();
		else if (strncmp(buff, MT_HELP_STR, MT_HELP_STR_SIZE) == 0)
			printf(MT_HELP_TXT);
		else
			printf("\nInvalid program, type \"help\" for options\n");
	}
}