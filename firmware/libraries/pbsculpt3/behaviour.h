/*  Holds a behaviour for a Series 3 node
 *
 *  Created on: May 27, 2016
 *      Author: dkadish
 */

#ifndef BEHAVIOUR_H_
#define BEHAVIOUR_H_

class Behaviour {
public:

	/** Stores a specific behaviour for a specific actuator in the pbsculpt system
	 *
	 *	@brief A behaviour is defined by a set of control points that are hit
	 *	       over a span of time.
	 *
	 *	@description Currently, the module performs a simple linear interpolation
	 *				 between points, but in the future, this may shift to a more
	 *				 complex (Hermite, Catmull-Rom) spline-based interpolation.
	 *
	 */
	Behaviour();
	virtual ~Behaviour();

	void start();
	void stop();
	void pause();
	void reset();

private:
	elapsedMillis _runTime;
	long _pauseTime;
};

#endif /* BEHAVIOUR_H_ */
