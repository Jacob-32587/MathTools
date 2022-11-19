#include "../include/MTBaseConvertor.hpp"
#include "../include/MTString.hpp"
#include "../include/MTIO.hpp"
#include <stdio.h>
#include <string.h>
#include <math.h>

#define MT_BASE_BUFF_SIZE 64

void baseConvertor()
{
	char buff[MT_BASE_BUFF_SIZE];		// temporary buff
	char numBuff[MT_BASE_BUFF_SIZE]; // buff to hold input number

	long long temp;					 // Hold temporary data
	unsigned short inputBase;		 // Hold entered input radix
	unsigned long long outputBase; // Hold entered output radix

	char *floatStr; // Hold decimal portion of input base

	bool QUIT_FLAG = false; // Track if the user wants to quit
	char negChar;				// Track weather the input is negative

	while (!QUIT_FLAG) // Run while the user has not quit
	{
		floatStr = nullptr;
		negChar = '\0';

		temp = 0;
		inputBase = 0;
		outputBase = 0;

		////////////////////////////
		// Get input base Number //
		//////////////////////////

		// Prompt user to get the base of the input number
		temp = promptAndGetLL("\nInput base: ", buff, MT_BASE_BUFF_SIZE, &QUIT_FLAG);

		if (!((temp > 1) && (temp <= 36)) && !QUIT_FLAG) // Not in valid base range
		{
			printf("Invalid base range, only valid for 2-36\n");
			continue;
		}
		else if (errorCheckLL(temp) && !QUIT_FLAG) // Error has occurred
		{
			printf("Input was out of range/invalid\n");
			continue;
		}
		else if (QUIT_FLAG) // User wants to quit
			break;
		else
			inputBase = temp;

		///////////////////////
		// Get input number //
		/////////////////////

		// Prompt user to enter the input number (processing of this number is handled later)
		printf("Input number: ");
		getInput(numBuff, MT_BASE_BUFF_SIZE);

		if (quitCheck(numBuff)) // Check if the user wants to quit
			break;

		floatStr = strchr(numBuff, '.'); // Check if the input number is a float or int

		// Remove negative sign from the buff
		if (numBuff[0] == '-')
		{
			negChar = '-';
			numBuff[0] = '0';
		}

		// Split string into integer and decimal portion
		if (floatStr != nullptr)
		{
			*floatStr = '\0';
			floatStr++;
		}

		/////////////////////////////
		// Get output base number //
		///////////////////////////

		// Prompt user to enter the output base

		temp = promptAndGetLL("Output base: ", buff, MT_BASE_BUFF_SIZE, &QUIT_FLAG);

		if (!((temp > 1) && (temp <= 36)) && !QUIT_FLAG) // Not in valid base range
		{
			printf("Invalid base range, only valid for 2-36\n");
			continue;
		}
		else if (errorCheckLL(temp) && !QUIT_FLAG) // Error has occurred
		{
			printf("Input was out of range/invalid\n");
			continue;
		}
		else if (QUIT_FLAG) // User wants to quit
			break;
		else
			outputBase = temp;

		///////////////////////
		// Calculate Output //
		/////////////////////

		if (inputBase == outputBase)
		{
			printf("\nInput and output bases are the same\n");
			continue;
		}

		if (onlyZero(numBuff)) // Avoid conversion of zero
			numBuff[0] = '0';
		else if (numBuff[0] != '\0') // Only convert if integer portion exists
		{
			temp = strtoll(numBuff, nullptr, inputBase);

			if ((errorCheckLL(temp) != MT_NO_ERROR) || (temp == 0 && !onlyZero(numBuff)))
			{
				printf("Input number was out of range/invalid\n");
				continue;
			}

			LLtoStr(temp, outputBase, buff);
		}
		if (floatStr != nullptr) // Convert decimal portion
		{
			convertBaseFL(floatStr, inputBase, outputBase, numBuff);
		}

		// Print the final result
		printf("Number in base %d is: %c%s", outputBase, negChar, buff);
		if (floatStr != nullptr)
			printf(".%s", numBuff);
		putchar('\n');
	}
}