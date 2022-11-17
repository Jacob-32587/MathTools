#include "../include/MTQuadraticFormula.hpp"
#include "../include/MTIO.hpp"
#include <stdio.h>

bool MT_QUIT_FLAG = false;

int main()
{
	while (!MT_QUIT_FLAG)
	{
		quadraticFormula();
	}
}