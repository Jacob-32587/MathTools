#include "../include/MTString.hpp"
#include <ctype.h>
#include <math.h>

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

// ARG0: Checks in the null-terminated character string is of zero value
// ie: "0000000" "0"
bool onlyZero(const char *input)
{
	while (*input != '\0')
	{
		// Only a valid character if 1 decimal appears and any number of zero's
		if (*input == '0' || isspace(*input))
			;
		else
			return false;

		input++; // Next character
	}
	return true; // Past all tests
}

// ARG0: Null-terminated string to be converted
// Convert a string to all upper case chars
void strToUpper(char *str)
{
	while (*str != '\0')
	{
		*str = toupper(*str);
		str++;
	}
}

// ARG0: Integer value to be converted
// ARG1: Radix of output string
// ARG2: Output buffer
void LLtoStr(long long input, short radix, char *output)
{
	char buff[128]; // Hold aux buffer

	long long size = 0; // Track num of digits in aux buffer
	long long mod = 0;  // Temporary value for mod calcs

	// Preform conversion and store result in aux buff
	do
	{
		mod = (input % radix);

		if (mod <= 9)
			buff[size] = mod + 48; // 0-9
		else
			buff[size] = mod + 55; // A-Z

		size++;
	} while (input /= radix);

	size--; // Get actual size

	// Write in reverse order
	for (; size >= 0; size--)
	{
		*output = buff[size];
		output++;
	}
	*output = '\0'; // Null-terminate
}

// ARG0: Interpreted as a floating point value from 0.9999999-0.000000
// ARG2: Radix of input string
// ARG1: Radix of output string
// ARG2: Output buffer
// Provides up to 7 decimal places of precision
// Do NOT include the decimal place of the floating point number in ARG0
// Undefined behavior will be invoked for any radix < 2 or radix > 36
// Zero may be returned for an invalid ARG0
void convertBaseFL(const char *inputStr, short inRadix, short outRadix, char *output)
{
	char buff[128]; // aux buff

	long long size = 0; // Tracks the size of the output
	long long mod = 0;  // Stores temporary mod values
	short digitCnt = 0; // Track the number of digits in the input

	unsigned long long floatNum = strtoll(inputStr, nullptr, inRadix); // Convert input num

	// Calculate digit expansion in new base
	floatNum = floatNum * outRadix * outRadix * outRadix * outRadix * outRadix * outRadix * outRadix;

	// Get the number of digits in the input float value
	while (*(inputStr + digitCnt) != '\0')
		digitCnt++;

	// Divide out the number of decimal places from the input
	for (short i = 0; i < digitCnt; i++)
	{
		floatNum /= inRadix;
	}

	// Preform conversion and store result in aux buff
	do
	{
		mod = (floatNum % outRadix);

		if (mod <= 9)
			buff[size] = mod + 48;
		else
			buff[size] = mod + 55;

		size++;
	} while (floatNum /= outRadix);

	// If all seven digits of division were not preformed then place zeros
	for (int i = size; i < 7; i++)
	{
		*output = '0';
		output++;
	}

	size--; // Get actual size

	// Reverse calculated answer
	for (; size >= 0; size--)
	{
		*output = buff[size];
		output++;
	}
	*output = '\0'; // Null-terminate
}