/*
 * functions.h
 *
 *  Created on: 17/08/2013
 *      Author: J.M. Tomé
 */

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

// #if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
// #else
// #include "WProgram.h"
// #endif

#define DEBUG 0

#include <arduino_utils.h>
#include <math_utils.h>

typedef struct
{
	float center;
	float amp;
	float freq;

} CircleAroundData;

class Functions
{

protected:
	unsigned long startTime;

	// periodic data
	uint8_t periodicDataSize;
	bool isConst;
	float *_amps;
	float *_freqs;
	float *_phases;

public:

	float speed = 1.0;
	uint8_t functionType;

	typedef enum
	{
		NONE,
		COSINES,
		CIRCLES,
		SINE,
		COSINE,
		TRIANGULAR,
		SQUARE,
		PULSE,
		SAWTOOTH,
		INVERSE_SAWTOOTH,
		RHOMBOIDAL,
		SINE_PULSE
	} FunctionTypes;

	Functions();

	virtual ~Functions();

	/** Sets startTime to now mills */
	void resetTimer();
	/* Gets internal function time in milliseconds	(ms)*/
	float mt();
	/* Gets internal function time in seconds (s)	 */
	float t();

	/* Gets angular phase (TWO_PI*t) for internal time (s)
	 */
	float wt();

	/* Gets angular phase (TWO_PI*f*t) for internal time (s)
	 * @param in Hertz (1/s)
	 */
	float wt(float freq);

	/* Gets angular phase (TWO_PI*f*t) for
	 * @param freq Frequency in Hertz (1/s)
	 * @param phase0 Initial phase in radians
	 */
	float wt(float freq, float phase0);

	void setPeriodicData(float amp, float freq, float phase);
	void setPeriodicData(uint8_t dataSize, float *amps, float *freqs, float *phases);

	void setPeriodicData(uint8_t dataSize, const float *amps, const float *freqs, const float *phases);

	void clearMemory();

	void resetPeriodicData(uint8_t newSize = 0);

	// FUNCTIONS

	float circleAround(CircleAroundData data);

	/** Cosines addition
	 * @param size Number of cosines
	 * @param amplitudes Amplitudes of each cosine wave.
	 * @param freqs Frequencies of each cosine wave. A 0 value will cause an offset level.
	 * @param phases Initial phase for each cosine wave. A -HALF_PI value Will convert the cosine in a sine wave.
	 */
	float cosines(uint8_t size, float *amplitudes, float *freqs, float *phases);

	/** Sines addition
	 * @param size Number of cosines
	 * @param amplitudes Amplitudes of each cosine wave.
	 * @param freqs Frequencies of each sine wave. A 0 value will cause an offset level.
	 * @param phases Initial phase for each wave.
	 */
	float sines(uint8_t size, float *amplitudes, float *freqs, float *phases);

	float circle(float f, float ph = 0.0);
	float circles(uint8_t size, float *amplitudes, float *freqs, float *phases);

	/**
	 * Maps a value in the range [0,1)
	 */
	float getInCircle(float f);

	/** Periodic oscillation every @period time, the funtion will go from @start to @end and return from @end to start
	 * @param start Initial value
	 * @param end End value
	 * @param period Time taken to go from @start to @end and return from @end to start. This can be read also as how often a value is repeated.
	 * @param minDistance If true, min distance path between end and start will be calculated.
	 */
	float circleFromTo(float start, float end, float period, bool minDistance = false);

	CircleAroundData getCircleAroundData(float start, float end, float period, bool minDistance = false);

	/** Periodic oscillation every @period time, the funtion will go from @start to @end and return from @end to start
	 * @param start Initial value
	 * @param end End value
	 * @param period Time taken to go from @start to @end and return from @end to start. This can be rad also ashow often a value is repeated.
	 * @param minDistance If true, min distance path between end and start will be calculated.
	 */
	void setCircleFromTo(float start, float end, float period, bool minDistance = false);
	float sawtooth(float frequency, float phase0 = 0);

	float inverseSawtooth(float frequency, float phase0 = 0);

	float triangular(float frequency, float phase0 = 0, float k = 0.5);
	float pulse(float frequency, float phase0 = 0, float k = 0.5);

	float rectangular(float frequency, float phase0 = 0, float k = 0.5);

	float square(float frequency, float phase0 = 0, float k = 0.5);
	float rhomboidWave(float frequency, float phase0 = 0, float k = 0.5);

	float sinePulse(float frequency, float phase0 = 0, float k = 0.5);

	float linearMovement(float x0, float v);

	void setFunctionType(uint8_t type);

	float updateFuntion();
};

#endif /* FUNCTIONS_H_ */
