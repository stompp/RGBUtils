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

struct PeriodicData
{
	uint8_t functionType;
	float amp;
	float freq;
	float phase;

	void debug()
	{
		println("PeriodicData");
		debugValue("amp", amp);
		debugValue("freq", freq);
		debugValue("phase", phase);
	}
};

template <int N>
struct NPeriodicData
{
	uint8_t functionType;
	uint8_t size;
	float amps[N];
	float freqs[N];
	float phases[N];

	void debug()
	{
		debugValue("PeriodicDataT", N);
		printArrayln(amps, N);
		printArrayln(freqs, N);
		printArrayln(phases, N);
	}
};

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

	/** Free allocated memory **/
	void clearMemory();	

public:
	float speed = 1.0;
	float k_param = 0.5;
	uint8_t functionType;

	// MAX 127
	typedef enum
	{
		NO_FUNCTION,
		COSINES,
		SINES,
		CIRCLES,
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

	/** Reset internal timer to current time */
	void resetTimer();

	/* Get internal function time in milliseconds	(ms)*/
	float mt();

	/* Get internal function time in seconds (s)	 */
	float t();

	/* Get angular phase (TWO_PI*t) for internal time (s)
	 */
	float wt();

	/* Get angular phase (TWO_PI*f*t) for internal time (s)
	 * @param in Hertz (1/s)
	 */
	float wt(float freq);

	/* Get angular phase (TWO_PI*f*t) for
	 * @param freq Frequency in Hertzs (1/s)
	 * @param phase0 Initial phase in radians
	 */
	float wt(float freq, float phase0);

	/** Set base amplitude if not using const periodic data
	 * 	@param amp Amplitude float value, best 1.0 to use as a base function
	 */
	void setBaseAmp(float amp);

	/** Set base frequency in Hzs if not using const periodic data
	 * 	@param freq Frequency in Hz (1/s) float value.
	 */
	void setBaseFrequency(float freq);

	/** Set base period in seconds if not using const periodic data
	 * 	@param period Period in s (1/Hz) float value.
	 */
	void setBasePeriod(float period);
	
	/** Set base initial phase in radians  if not using const periodic data
	 * 	@param phase Phase in radians.
	 */
	void setBasePhase(float phase);

	/** Set periodic data size to 1 and base amplitude, frequency and initial phase **/
	void setPeriodicData(float amp, float freq, float phase);

	/** Set periodic data doing a copy from input data **/
	void setPeriodicData(uint8_t dataSize, float *amps, float *freqs, float *phases);

	/** Set periodic data pointing to constant input data **/
	void setPeriodicData(uint8_t dataSize, const float *amps, const float *freqs, const float *phases);

	/** Set periodic data size allocating memory **/		
	void resetPeriodicData(uint8_t newSize = 0);

	// FUNCTIONS

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

	// CIRCLES TODO TEST
	float circleAround(CircleAroundData data);

	float circle(float f, float ph = 0.0);

	float circles(uint8_t size, float *amplitudes, float *freqs, float *phases);

	/**
	 * Maps a value in the range [0,1) ????
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

	// WAVE FORMS
	float sawtooth(float frequency, float phase0 = 0);
	float inverseSawtooth(float frequency, float phase0 = 0);
	float triangular(float frequency, float phase0 = 0, float k = 0.5);
	float pulse(float frequency, float phase0 = 0, float k = 0.5);
	float rectangular(float frequency, float phase0 = 0, float k = 0.5);
	float square(float frequency, float phase0 = 0, float k = 0.5);
	float rhomboid(float frequency, float phase0 = 0, float k = 0.5);
	float sinePulse(float frequency, float phase0 = 0, float k = 0.5);

	// LINEAR MOVEMENT
	float linearMovement(float x0, float v);

	void setFunctionType(uint8_t type);

	float value();

	void debugData();
};

#endif /* FUNCTIONS_H_ */
