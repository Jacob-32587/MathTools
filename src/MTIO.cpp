#include "../include/MTIO.hpp"
#include "../include/MTString.hpp"
#include <stdio.h>
#include <math.h>
#include <string.h>

extern bool MT_QUIT_FLAG;

// ARG0: Message that the user will be prompted to enter data with
// ARG1: Buffer to hold data
long double promptAndGetLD(const char *message, char *buff, size_t buffSize)
{
	long double retVal = 0.0L; // Hold value that may be returned

	printf("%s", message); // Prompt user to enter data

	if (fgets(buff, buffSize, stdin) != nullptr)
	{
		retVal = strtold(buff, nullptr); // Convert the input string into a long double

		if (strncmp(buff, MT_ESCAPE, MT_ESCAPE_SIZE) == 0) // Check if the user wants to exit program
			MT_QUIT_FLAG = true;
		else if (retVal == 0.0 && !onlyZeroF(buff)) // Check if strtold() could not convert the input
			return INFINITY;
		else
			return retVal;
	}
	else
		return INFINITY;
}

short LDErrorCheck(long double input, const char *errorMessage)
{
	if (MT_QUIT_FLAG)
		return MT_QUIT;
	else if (isinf(input) || (input == HUGE_VALL))
	{
		printf("%s", errorMessage);
		return MT_LD_ERROR;
	}
	else
		return MT_LD_NO_ERROR;
}