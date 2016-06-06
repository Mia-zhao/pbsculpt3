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

#define DEBUG_BEHAVIOUR 0

#include <LinkedList.h>
#include <elapsedMillis.h>

/** States for the behaviour
 *
 * 	@brief Defines the behaviour state as stopped, playing, or paused.
 *
 * 	@details Uses a media player analogy and allows a behaviour to be in one of three states.
 */
enum BehaviourState {
	STOPPED,//!< Not playing, reset to the beginning
	PLAYING,//!< Playing
	PAUSED  //!< Stopped, but position is kept
};

/** Point on the behaviour plot
 *
 * 	@brief Charts a point on the time-value (x,y) plot for a behaviour.
 *
 * 	@details Both are stored as unsigned long's to simplify creation. The points
 * 			 must be added in chronological order. Any points out of order will
 * 			 be ignored.
 *
 */
struct Point {
	unsigned long time; //!< Time (in microseconds) at the behaviour point
	unsigned long value; //!< Value (usually between 0-255) at behaviour point
};

/**	Stores a specific behaviour for a specific actuator in the pbsculpt system
 *
 *	@brief A behaviour is defined by a set of control points that are passed
 *	       over a span of time.
 *
 *	@details Currently, the module performs a simple linear interpolation
 *			 between points, but in the future, this may shift to a more
 *			 complex (Hermite, Catmull-Rom) spline-based interpolation.
 *			 NOTE: Behaviours MUST have a start point and an end point
 *			 and will fail with LESS than two points.
 *
 */
class Behaviour {
public:

	Behaviour();
	virtual ~Behaviour();

	// Integration functions
	/** Call this in the loop() function of the .ino script to keep the
	 * 	behaviour up to date.
	 *
	 */
	void loop();

	/** Get the current value of the behaviour
	 *
	 * @return The Integer value of the current behaviour (default: [0-255])
	 */
	int value();

	// Control functions
	/** Start (or continue) the behaviour
	 *
	 */
	void play();

	/** Stop and reset the behaviour
	 *
	 */
	void stop();

	/** Pause the behaviour
	 *
	 */
	void pause();

	/** Reset the behaviour to the beginning
	 *
	 * If the behaviour is playing it will continue to play, just from the start.
	 * If it is stopped or paused, it will revert to the beginning, but will not
	 * play until play() is called.
	 */
	void reset();

	/**	Set the maximum value for behaviour
	 *
	 * The ceiling will be enforced so that if the value rises above the max,
	 * it will be capped at this number.
	 *
	 * @param value The maximum value that the behaviour should output.
	 */
	void setMaxValue( unsigned long value );

	/** @name Scaling Functions
	 *
	 * Because the behaviour system uses integer math, the scaling factors have
	 * to be set as separate multipiers and divisors. This goes for both the
	 * time and values. So, for example, to increase the length of time that
	 * the behaviour takes, you should run
	 * @code
	 * behaviour.setTimeMultiplier(3);
	 * behaviour.setTimeDivisor(2);
	 * @endcode
	 * which will lead to a multiplication of the behaviour time by 1.5.
	 */
	//@{
	/** Set the multiplier for the time in order to stretch the behaviour
	 *
	 * @param mux Multiplication factor
	 */
	void setTimeMultiplier( int mux );

	/** Set the divisor for the time in order to compress the behaviour
	 *
	 * @param div Division factor
	 */
	void setTimeDivisor( int div );

	/** Set the multiplier for the value in order to compress the behaviour
	 *
	 * @param mux Multiplication factor
	 */
	void setValueMultiplier( int mux);

	/**Set the divisor for the time in order to compress the behaviour
	 *
	 * @param div Division factor
	 */
	void setValueDivisor(int div);
	//@}

	/** LinkedList of Points (time, value) that the behaviour will pass through
	 *
	 * 	This LinkedList must be populated (in the order of time) with
	 * 	Points that will be passed through for the behaviour. At least
	 * 	two points must be specified.
	 *
	 *	For example:
	 * 	@code
	 * 	Behaviour behaviour = Behaviour();
	 *	unsigned long timeValue[n_timeValue][2] = {
	 *		{0L, 0L},
	 *		{100000L, 30L},
	 *	};
	 *
	 *	for( int i = 0; i < n_timeValue; i++ ){
	 *		Point *point = new Point();
	 *		point->time = timeValue[i][0];
	 *		point->value = timeValue[i][1];
	 *
	 *		behaviour.points.add(point);
	 *	}
	 * 	@endcode
	 *
	 */
    LinkedList<Point*> points;

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

	unsigned long _maxValue;

	int _timeMux, _timeDiv;
	int _valueMux, _valueDiv;

	// Playing variables
	BehaviourState _state;
	int _i_lastPoint;

	Point *_lastPoint, *_nextPoint;
};

#endif /* BEHAVIOUR_H_ */
