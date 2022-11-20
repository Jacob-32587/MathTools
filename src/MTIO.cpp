#include "../include/MTIO.hpp"
#include "../include/MTString.hpp"
#include <stdio.h>
#include <math.h>
#include <string.h>

// ARG0: Message that the user will be prompted to enter data with
// ARG1: Buffer to hold data
// ARG2: Size of the buffer
// ARG3: Flag to let the programmer quit (compares to MT_ESCAPE)
long double promptAndGetLD(const char *message, char *buff, size_t buffSize, bool *QUIT_FLAG)
{
	long double retVal = 0.0L; // Hold value that may be returned

	printf("%s", message); // Prompt user to enter data

	if (getInput(buff, buffSize) != nullptr)
	{
		retVal = strtold(buff, nullptr); // Convert the input string into a long double

		*QUIT_FLAG = quitCheck(buff);

		if (retVal == 0.0 && !onlyZeroF(buff)) // Check if strtold() could not convert the input
			return INFINITY;
		else
			return retVal;
	}
	else // Bad input
		return INFINITY;
}

// ARG0: Message that the user will be prompted to enter data with
// ARG1: Buffer to hold data
// ARG2: Size of the buffer
// ARG3: Flag to let the programmer quit (compares to MT_ESCAPE)
long long promptAndGetLL(const char *message, char *buff, size_t buffSize, bool *QUIT_FLAG)
{
	long long retVal = 0LL; // Hold value that may be returned

	printf("%s", message); // Prompt user to enter data

	if (getInput(buff, buffSize) != nullptr)
	{
		retVal = strtoll(buff, nullptr, 10); // Convert the input into base 10

		*QUIT_FLAG = quitCheck(buff);

		if (retVal == 0 && !onlyZero(buff)) // Check if strtold() could not convert the input
			return LLONG_MAX;
		else
			return retVal;
	}
	else
		return LLONG_MAX;
}

// ARG0: Buffer that holds data from stdin
// ARG1: Size of the buffer in ARG0
// If the input length exceeds bufferSize - 1 then
// the overflow data will be discarded
char *getInput(char *buffer, size_t bufferSize)
{
	int temp = 0;
	size_t inputSize = 0;

	while (((temp = fgetc(stdin)) != EOF) && (temp != '\n') && (temp != '\r'))
	{
		*(buffer + inputSize) = temp;
		inputSize++;

		if (inputSize == bufferSize)
		{
			// Flush remaining stdin
			while (((temp = getchar()) != '\n') && (temp != EOF))
				;
			break;
		}
	}

	*(buffer + inputSize) = '\0';

	return buffer;
}

// Check if the user entered an escape str
// If the str is not null-terminated then undefined behavior
// WILL modify input str
bool quitCheck(char *str)
{
	strToUpper(str); // Input is not case sensetive

	if (strncmp(str, MT_ESCAPE, MT_ESCAPE_SIZE) == 0)
		return true;
	else
		return false;
}

// Check for errors of a long double parsed by strtold
short errorCheckLD(long double input)
{
	if (isinf(input) || (input == HUGE_VALL))
		return MT_TYPE_ERROR;
	else
		return MT_NO_ERROR;
}

// Check for errors of a long long parsed by strtoll
short errorCheckLL(long long input)
{
	if ((input == LLONG_MAX) || (input == LLONG_MIN))
		return MT_TYPE_ERROR;
	else
		return MT_NO_ERROR;
}