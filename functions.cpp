#include <functions.h>
void Functions::debugData()
{
	int N = periodicDataSize;
	debugValue("PeriodicDataT", N);
	printArrayln(_amps, N);
	printArrayln(_freqs, N);
	printArrayln(_phases, N);
}
Functions::Functions()
{
	resetTimer();
	setPeriodicData(1, 1, 0);
}

Functions::~Functions()
{

	resetPeriodicData();
}

/** Sets startTime to now mills */
void Functions::resetTimer() { startTime = millis(); }
/* Gets internal function time in milliseconds	(ms)*/
float Functions::mt() { return speed * ((float)(millis() - startTime)); }
/* Gets internal function time in seconds (s)	 */
float Functions::t() { return mt() / 1000.0f; }

/* Gets angular phase (TWO_PI*t) for internal time (s)
 */
float Functions::wt() { return TWO_PI * t(); }

/* Gets angular phase (TWO_PI*f*t) for internal time (s)
 * @param in Hertz (1/s)
 */
float Functions::wt(float freq) { return wt() * freq; }

/* Gets angular phase (TWO_PI*f*t) for
 * @param freq Frequency in Hertz (1/s)
 * @param phase0 Initial phase in radians
 */
float Functions::wt(float freq, float phase0) { return phase0 + wt(freq); }

/** Sets base amplitude if not using const periodic data
 * 	@param amp Amplitude float value, best 1.0 to use as a base function
 */
void Functions::setBaseAmp(float amp)
{
	if(!isConst){
		*_amps = amp;
	}
}
/** Sets base frequency in Hzs if not using const periodic data
 * 	@param freq Frequency in Hz (1/s) float value.
 */
void Functions::setBaseFrequency(float freq)
{
		if(!isConst){
		*_freqs = freq;
	}
}
/** Sets base period in seconds if not using const periodic data
 * 	@param period Period in s (1/Hz) float value.
 */
void Functions::setBasePeriod(float period)
{
		if(!isConst){
		*_freqs = 1.0f/period;
	}
}
/** Sets base initial phase in radians  if not using const periodic data
 * 	@param phase Phase in radians.
 */
void Functions::setBasePhase(float phase)
{
		if(!isConst){
		*_phases = phase;
	}
}

void Functions::setPeriodicData(float amp, float freq, float phase)
{
	resetPeriodicData(1);
	*_amps = amp;
	*_freqs = freq;
	*_phases = phase;
	isConst = false;
}

void Functions::setPeriodicData(uint8_t dataSize, float *amps, float *freqs, float *phases)
{

	resetPeriodicData(dataSize);

	// uint8_t n = 0;
	// while (n++ < dataSize)
	// {
	// 	(*_amps++) = (*amps++);
	// 	(*_freqs++) = (*freqs++);
	// 	(*_phases++) = (*phases++);
	// }
	// memccpy(amps,_amps, dataSize, sizeof(float));
	// memccpy(_freqs, _freqs, dataSize, sizeof(float));
	// memccpy(_phases, _phases, dataSize, sizeof(float));

	for (byte n = 0; n < dataSize; n++)
	{
		_amps[n] = amps[n];
		_freqs[n] = freqs[n];
		_phases[n] = phases[n];
	}
	// 	// (*_amps++) = (*amps++);
	// 	// (*_freqs++) = (*freqs++);
	// 	// (*_phases++) = (*phases++);
	// }
}

void Functions::setPeriodicData(uint8_t dataSize, const float *amps, const float *freqs, const float *phases)
{

	clearMemory();
	periodicDataSize = dataSize;
	_amps = (float *)amps;
	_freqs = (float *)freqs;
	_phases = (float *)phases;
	isConst = true;
}

void Functions::clearMemory()
{
	if (!isConst)
	{
		if (_amps != NULL)
		{
			delete[] _amps;
			// free(_amps);
		}
		if (_freqs != NULL)
		{
			delete[] _freqs;
			// free(_freqs);
		}
		if (_phases != NULL)
		{
			delete[] _phases;
			// free(_phases);
		}
	}
}

void Functions::resetPeriodicData(uint8_t newSize)
{

	// if (newSize == periodicDataSize)
	// 	return;

	periodicDataSize = newSize;

	clearMemory();

	if (periodicDataSize != 0)
	{

		_amps = new float[periodicDataSize];
		_freqs = new float[periodicDataSize];
		_phases = new float[periodicDataSize];

		// _amps = (float *)malloc(sizeof(float)*periodicDataSize);
		// _freqs =  (float *)malloc(sizeof(float)*periodicDataSize);
		// _phases =  (float *)malloc(sizeof(float)*periodicDataSize);

		// _amps = (float *)calloc(periodicDataSize, sizeof(float));
		// _freqs = (float *)calloc(periodicDataSize, sizeof(float));
		// _phases = (float *)calloc(periodicDataSize, sizeof(float));
		isConst = false;
	}
}

// FUNCTIONS

float Functions::circleAround(CircleAroundData data)
{
	return getInCircle(data.center - data.amp * cos(wt(data.freq)));
}

/** Cosines addition
 * @param size Number of cosines
 * @param amplitudes Amplitudes of each cosine wave.
 * @param freqs Frequencies of each cosine wave. A 0 value will cause an offset level.
 * @param phases Initial phase for each cosine wave. A -HALF_PI value Will convert the cosine in a sine wave.
 */
float Functions::cosines(uint8_t size, float *amplitudes, float *freqs, float *phases)
{

	if (size == 0)
		return 0.0;

	float out = 0.0;
	float k = wt();
	uint8_t n = 0;

	while (n++ < size)
	{
		out += (*amplitudes++) * cos(k * (*freqs++) + (*phases++));
	}

	// out = getInCircle(out);

	return out;
}

/** Sines addition
 * @param size Number of cosines
 * @param amplitudes Amplitudes of each cosine wave.
 * @param freqs Frequencies of each sine wave. A 0 value will cause an offset level.
 * @param phases Initial phase for each wave.
 */
float Functions::sines(uint8_t size, float *amplitudes, float *freqs, float *phases)
{

	if (size == 0)
		return 0.0;

	float out = 0.0;
	float k = wt();
	uint8_t n = 0;

	while (n++ < size)
	{
		out += (*amplitudes++) * sin(k * (*freqs++) + (*phases++));
	}

	// out = getInCircle(out);

	return out;
}

float Functions::circle(float f, float ph)
{
	return getInCircle(f * t() + ph / TWO_PI);
}

float Functions::circles(uint8_t size, float *amplitudes, float *freqs, float *phases)
{
	float tt = t();
	float x;
	float out;
	uint8_t n = 0;

	while (n++ < size)
	{
		x = ((*freqs++) * tt) + ((*phases++) / TWO_PI);

		out += ((*amplitudes++) * (x - (int)x));
		//  n++;
		//	out += ((*amplitudes++)*cos(x));
	}

	return out;
}

/**
 * Maps a value in the range [0,1)
 */
float Functions::getInCircle(float f)
{

	if (abs(f) > 1.0)
		return f - (int)f;
	else if (f < 0)
		return f - (int)f + 1.0;
	return f;
}

/** Periodic oscillation every @period time, the funtion will go from @start to @end and return from @end to start
 * @param start Initial value
 * @param end End value
 * @param period Time taken to go from @start to @end and return from @end to start. This can be read also as how often a value is repeated.
 * @param minDistance If true, min distance path between end and start will be calculated.
 */
float Functions::circleFromTo(float start, float end, float period, bool minDistance)
{

	// Clockwise
	float distance = getInCircle(start - end);
	// Clockwise then sustract from start.
	float direction = -1.0;
	// Counterclockwise distance
	float a = getInCircle(end - start);
	// Counterclockwise then add from start
	float b = 1.0;

	if ((minDistance && (a < distance)) || ((!minDistance) && (a > distance)))
	{
		swap(distance, a);
		swap(direction, b);
	}

	/* Determinar centro, frecuencia y amplitud
	 * a -> amplitud = direction*distance/.0;
	 * c = from + amplitud
	 * freccuencia = (distancia/time)�/2?
	 */

	// amplitud = direction*distance/.0;

	a = direction * distance / 2.0f;
	// frequency
	b = 2.0f * distance / period;
	//		float c = (start + a);

	return getInCircle((start + a) - a * cos(wt(b)));
}

CircleAroundData Functions::getCircleAroundData(float start, float end, float period, bool minDistance)
{

	// Clockwise

	float distance = getInCircle(start - end);
	// Clockwise then sustract from start.
	float direction = -1.0;
	// Counterclockwise distance
	float a = getInCircle(end - start);
	// Counterclockwise then add from start
	float b = 1.0;

	if ((minDistance && (a < distance)) || ((!minDistance) && (a > distance)))
	{
		swap(distance, a);
		swap(direction, b);
	}

	/* Determinar centro, frecuencia y amplitud
	 * a -> amplitud = direction*distance/.0;
	 * c = from + amplitud
	 * freccuencia = (distancia/time)�/2?
	 */

	// amplitud = direction*distance/.0;

	CircleAroundData data;
	data.amp = distance / 2.0f;
	// frequenzy
	data.freq = TWO_PI * distance / period;
	data.amp *= direction;
	data.center = start + data.amp;

	return data;
}

/** Periodic oscillation every @period time, the funtion will go from @start to @end and return from @end to start
 * @param start Initial value
 * @param end End value
 * @param period Time taken to go from @start to @end and return from @end to start. This can be rad also ashow often a value is repeated.
 * @param minDistance If true, min distance path between end and start will be calculated.
 */
void Functions::setCircleFromTo(float start, float end, float period, bool minDistance)
{

	// Clockwise
	float distance = getInCircle(start - end);
	// Clockwise then sustract from start.
	float direction = -1.0;
	// Counterclockwise distance
	float a = getInCircle(end - start);
	// Counterclockwise then add from start
	float b = 1.0;

	if ((minDistance && (a < distance)) || ((!minDistance) && (a > distance)))
	{
		swap(distance, a);
		swap(direction, b);
	}

	/* Determinar centro, frecuencia y amplitud
	 * a -> amplitud = direction*distance/.0;
	 * c = from + amplitud
	 * frecuencia = (distancia/time)�/2?
	 */

	// amplitud = direction*distance/.0;
	a = direction * distance / 2.0;
	// frequenzy
	b = 2.0 * distance / period;
	//		float c = (start + a);

	float amps[] = {(start + a), a};
	float freqs[] = {0.0, b};
	float ph[] = {0.0, PI};
	functionType = COSINES;
	setPeriodicData(2, amps, freqs, ph);
	resetTimer();
}

float Functions::sawtooth(float frequency, float phase0)
{
	return sawtoothWave(wt(frequency, phase0));
}

float Functions::inverseSawtooth(float frequency, float phase0)
{
	return inverseSawtoothWave(wt(frequency, phase0));
}

float Functions::triangular(float frequency, float phase0, float k)
{
	return triangularWave(wt(frequency, phase0), k);
}

float Functions::pulse(float frequency, float phase0, float k)
{
	return pulseWave(wt(frequency, phase0), k);
}

float Functions::rectangular(float frequency, float phase0, float k)
{
	return rectangularWave(wt(frequency, phase0), k);
}

float Functions::square(float frequency, float phase0, float k)
{
	return squareWave(wt(frequency, phase0), k);
}

float Functions::rhomboid(float frequency, float phase0, float k)
{
	return rhomboidWave(wt(frequency, phase0), k);
}

float Functions::sinePulse(float frequency, float phase0, float k)
{
	return sinPulseWave(wt(frequency, phase0), k);
}

float Functions::linearMovement(float x0, float v) { return x0 + v * t(); }

void Functions::setFunctionType(uint8_t type) { functionType = type; }

float Functions::value()
{
	if (periodicDataSize == 0)
		return 0.0;

	switch (functionType)
	{
	case COSINES:
		return cosines(periodicDataSize, _amps, _freqs, _phases);
	case SINES:
		return cosines(periodicDataSize, _amps, _freqs, _phases);
	case CIRCLES:
		return circles(periodicDataSize, _amps, _freqs, _phases);
	case TRIANGULAR:
		return *_amps * triangular(*_freqs, *_phases, k_param);
	case SQUARE:
		return *_amps * square(*_freqs, *_phases, k_param);
	case PULSE:
		return *_amps * pulse(*_freqs, *_phases, k_param);
	case SAWTOOTH:
		return *_amps * sawtooth(*_freqs, *_phases);
	case INVERSE_SAWTOOTH:
		return *_amps * inverseSawtooth(*_freqs, *_phases);
	case RHOMBOIDAL:
		return *_amps * rhomboid(*_freqs, *_phases, k_param);
	case SINE_PULSE:
		return *_amps * sinePulse(*_freqs, *_phases, k_param);
	default:
		return 0.0F;
	}
}
