#include "../include/MTString.hpp"
#include <ctype.h>

// ARG0: Checks in the null-terminated character string is of zero value
// ie: "0.0000000" "0.0" "0"
bool onlyZeroF(const char *input)
{
	short decimalCnt = 0; // Track number of decimal places

	while (*input != '\0')
	{
		// Only a valid character if 1 decimal appears and any number of zero's
		if (*input == '0' || isspace(*input))
			;
		else if (*input == '.')
			decimalCnt++;
		else
			return false;

		if (decimalCnt == 2) // Cannot have more than 1 decimal marker
			return false;

		input++; // Next character
	}

	return true; // Past all tests
}