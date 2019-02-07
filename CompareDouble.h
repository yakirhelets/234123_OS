/*
 * CompareDouble.h
 */

#ifndef COMPAREDOUBLE_H_
#define COMPAREDOUBLE_H_
#include <math.h>


namespace mtm{

	static bool compareIsBigger(double a, double b)
	{
		const double EPSILON = 1e-10;
		return fabs(a - b) < EPSILON;
	}

}


#endif /* COMPAREDOUBLE_H_ */
