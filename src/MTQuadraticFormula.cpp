#include <stdio.h>
#include <math.h>
#include <wchar.h>
#include "../include/MTIO.hpp"
#include <float.h>

#define MT_QFC_ERROR_MSG "Formatting error on last input, try again\n"
#define MT_QFC_BUFF_SIZE 32

void quadraticFormula()
{
	char buff[MT_QFC_BUFF_SIZE];

	long double a = 0.0L;
	long double b = 0.0L;
	long double c = 0.0L;
	long double discriminant = 0.0L;

	short temp = 0;

	bool MT_QUIT_FLAG = false;

	while (!MT_QUIT_FLAG)
	{
		a = promptAndGetLD("a = ", buff, MT_QFC_BUFF_SIZE, &MT_QUIT_FLAG);
		if ((temp = errorCheckLD(a)) && !MT_QUIT_FLAG)
		{
			printf("%s", MT_QFC_ERROR_MSG);
			continue;
		}
		else if (MT_QUIT_FLAG)
			break;

		b = promptAndGetLD("b = ", buff, MT_QFC_BUFF_SIZE, &MT_QUIT_FLAG);
		if ((temp = errorCheckLD(b)) && !MT_QUIT_FLAG)
		{
			printf("%s", MT_QFC_ERROR_MSG);
			continue;
		}
		else if (MT_QUIT_FLAG)
			break;

		c = promptAndGetLD("c = ", buff, MT_QFC_BUFF_SIZE, &MT_QUIT_FLAG);
		if ((temp = errorCheckLD(c)) && !MT_QUIT_FLAG)
		{
			printf("%s", MT_QFC_ERROR_MSG);
			continue;
		}
		else if (MT_QUIT_FLAG)
			break;

		putchar('\n');

		discriminant = (b * b) - (4 * a * c);

		// Chain of if statements to create an equation in the console
		if (a != 0)
		{
			printf("%lfx^2", (double)a);

			if ((b != 0) || (c != 0))
				printf(" + ");
		}
		if (b != 0)
		{
			printf("%lfx", (double)b);

			if (c != 0)
				printf(" + ");
		}
		if (c != 0)
			printf("%lf", (double)c);

		if ((a == 0) && (b == 0) && (c == 0))
			putchar('0');

		printf(" = 0 \n");

		// Output result of input coefficients

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
			else if (b != 0) // Quadratic has multiplicity of 2
				printf("Multiplicity of 2, singular root is: %lf\n", (double)((b * -1.0L) / (2 * a)));
			else
				printf("Multiplicity of 2, singular root is: 0\n");
		}
		else if (b != 0) // Equation is linear
		{
			printf("Singular root is: %lf\n", (double)(-c / b));
		}
		else if (c != 0) // Constant may or may not equal zero
		{
			printf("You've only entered a constant value...\n");
		}
		else // Stated zero is equal to zero
		{
			printf("0 does, in fact, equal 0\n");
		}

		putchar('\n');

	} // User input & output loop
}
