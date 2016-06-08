/*
 * GaussianRandom.h
 *
 *  Created on: Jun 8, 2016
 *      Author: dkadish
 */

#ifndef GAUSSIANRANDOM_H_
#define GAUSSIANRANDOM_H_

class GaussianRandom {
public:
	GaussianRandom(long mean, long std);
	virtual ~GaussianRandom();

	long randLong();

	static constexpr float e = 2.718281828459045235360287471352;

protected:
	long _mean;
	long _std;

	long _next;

	bool _hasNext;

};

#endif /* GAUSSIANRANDOM_H_ */
