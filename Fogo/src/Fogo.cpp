#include "Fogo.h"

const char * Fogo::fizzbuzz(int n)
{
	if (n % 15 == 0) return "FizzBuzz";
	if (n % 3 == 0) return "Fizz";
	if (n % 5 == 0) return "Buzz";
	return nullptr;
}
