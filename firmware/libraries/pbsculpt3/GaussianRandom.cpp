/*
 * GaussianRandom.cpp
 *
 *  Created on: Jun 8, 2016
 *      Author: dkadish
 */

#include <GaussianRandom.h>
#include <Arduino.h>
#include <math.h>

GaussianRandom::GaussianRandom(long mean, long std):
	_mean(mean), _std(std), _hasNext(false){
	randomSeed(analogRead(A0));
}

GaussianRandom::~GaussianRandom() {
}

long GaussianRandom::randLong(){
	if(_hasNext){
		_hasNext = false;
		return _next;
	} else {
		// Calculate next random numbers
		/* From https://en.wikipedia.org/wiki/Normal_distribution#Generating_values_from_normal_distribution
		*/
		float S;
		float X=0.0, Y=0.0;
		do {
			float U = random(2000000000L)/2000000000.0;
			float V = random(2000000000L)/2000000000.0;

			S = pow(U,2.0) + pow(V, 2.0);
			if( S < 1.0 ){
				float T =  sqrt(-2*log(S)/S);

				X = U * T;
				Y = V * T;
			}
		} while ( S >= 1.0 );

		_next = (long)(Y * _std + _mean);
		return (long)(X * _std + _mean);
	}

	return 0L;
}
