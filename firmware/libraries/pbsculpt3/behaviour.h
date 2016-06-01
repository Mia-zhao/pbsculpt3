/*  Holds a behaviour for a Series 3 node
 *
 *  Created on: May 27, 2016
 *      Author: dkadish
 *
 *
 *  @todo: What if this was reformulated with time being the delay in microseconds
 *  	   from the previous action in the default setting. Which could then get
 *  	   stretched and shrunk. MUCH EASIER MATH!
 */

#ifndef BEHAVIOUR_H_
#define BEHAVIOUR_H_

#define DEBUG_BEHAVIOUR 1

#include <LinkedList.h>
#include <elapsedMillis.h>

enum BehaviourState {
	STOPPED,
	PLAYING,
	PAUSED
};

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
	 *				 NOTE: Behaviours MUST have a start point and an end point
	 *				 and will fail with LESS than two points.
	 *
	 */
	Behaviour();
	virtual ~Behaviour();

	// Integration functions
	void loop();
	int value();

	// Control functions
	void play();
	void stop();
	void pause();
	void reset();

	// Set limitations for behaviour
	void setMaxValue( unsigned long value );

	// Set multipliers and dividers
	void setTimeMultiplier( int mux );
	void setTimeDivisor( int div );
	void setValueMultiplier( int mux);
	void setValueDivisor(int div);

    LinkedList<unsigned long*> points;

private:
    unsigned long currentPointStartTime();
    unsigned long nextPointStartTime();

    /** Advance to the next set of control points.
     *
     *	@return Boolean indicating whether there are more points in the behaviour definition.
     */
    bool advancePoint();
    bool loadPoints();

	elapsedMicros _runTime;
	unsigned long _pauseTime;

	/** Pointer towards a 2D array of points in the format [time, value]
	 *
	 *	@description
	 */
	unsigned long _maxValue;

	int _timeMux, _timeDiv;
	int _valueMux, _valueDiv;

	// Playing variables
	BehaviourState _state;
	int _i_lastPoint;

	unsigned long *_lastPoint, *_nextPoint;
};

#endif /* BEHAVIOUR_H_ */
