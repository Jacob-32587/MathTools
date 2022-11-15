#include <stdio.h>
#include <math.h>
#include "../include/MTIO.hpp"

#define MT_QFC_ERROR_MSG "Formatting error on last input, try again\n"
#define MT_QFC_BUFF_SIZE 32

extern bool MT_QUIT_FLAG;

void quadraticFormula()
{
	char buff[MT_QFC_BUFF_SIZE];

	long double a = 0.0L;
	long double b = 0.0L;
	long double c = 0.0L;

	long double discriminant = 0.0L;

	short temp = 0;

	while (!MT_QUIT_FLAG)
	{
		a = promptAndGetLD("a = ", buff, MT_QFC_BUFF_SIZE);
		if (temp = LDErrorCheck(a, MT_QFC_ERROR_MSG))
		{
			if (temp == MT_QUIT)
				break;
			else
				continue;
		}

		b = promptAndGetLD("b = ", buff, MT_QFC_BUFF_SIZE);
		if (temp = LDErrorCheck(b, MT_QFC_ERROR_MSG))
		{
			if (temp == MT_QUIT)
				break;
			else
				continue;
		}

		c = promptAndGetLD("c = ", buff, MT_QFC_BUFF_SIZE);
		if (temp = LDErrorCheck(c, MT_QFC_ERROR_MSG))
		{
			if (temp == MT_QUIT)
				break;
			else
				continue;
		}

		break;
	} // User input collection loop

	if (!MT_QUIT_FLAG)
	{
		discriminant = (b * b) - (4 * a * c);

		if (a != 0) // Equation is quadratic
		{
			if (discriminant < 0) // Imaginary roots
			{
				printf("First root: %lfi\n", (double)((-b + sqrt(discriminant * -1.0L)) / (2.0L * a)));
				printf("Second root: %lfi\n", (double)((-b - sqrt(discriminant * -1.0L)) / (2.0L * a)));
			}
			else if (discriminant != 0) // Quadratic has two roots
			{
				printf("First root: %lf\n", (double)((-b + sqrt(discriminant)) / (2 * a)));
				printf("Second root: %lf\n", (double)((-b - sqrt(discriminant)) / (2 * a)));
			}
			else // Quadratic has multiplicity of 2
				printf("Multiplicity of 2, singular root is: %lf\n", (double)(-b / (2 * a)));
		}
		else if (b != 0) // Equation is linear
		{
			printf("Singular root is: %lf", (double)(-c / b));
		}
		else // Not an equation
		{
			printf("You've only entered a constant value...");
		}
	} // Check if the user has quit the program
}
