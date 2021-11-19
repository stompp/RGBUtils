/*
 * functions.h
 *
 *  Created on: 17/08/2013
 *      Author: Juan
 */

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

// #if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
// #else
// #include "WProgram.h"
// #endif

#define DEBUG 0


typedef struct{
	float center;
	float amp;
	float freq;

}CircleAroundData;


float sign(float v){return v < 0.0 ? -1.0 : 1.0;}
bool isPositive(float f) {return f >= 0.0;}
void swap(float &value1,float &value2){
	float aux;
	aux = value1;
	value1 = value2;
	value2 = aux;
}


 


class Functions{

protected:

	unsigned long startTime;


	// periodic data
	uint8_t periodicDataSize;
	bool isConst;
	float* _amps;
	float* _freqs;
	float* _phases;

public:

	typedef enum{
		NONE,
		COSINES,
		CIRCLES,
	}FunctionTypes;

	uint8_t functionType;
	Functions(){}
	virtual ~Functions(){

		resetPeriodicData();
	}

	/** Sets startTime to now mills */
	void resetTimer(){ startTime = millis();}
	/* Gets internal function time in milliseconds	(ms)*/
	float mt(){return ((float)(millis() - startTime));}
	/* Gets internal function time in seconds (s)	 */
	float t(){return ((float)(millis() - startTime))/1000.0f;}
	/* Gets angular phase (TWO_PI*f*t) for
	 * @param freq Frequency in Hertz (1/s)
	 * @param t Time in seconds (s)
	 */
	float wt(float freq,float t){return  TWO_PI*freq*t;}
	/* Gets angular phase (TWO_PI*f*t) for internal time (s)
	 * @param in Hertz (1/s)
	 */
	float wt(float freq){return  TWO_PI*freq*t();}


	void setPeriodicData(uint8_t dataSize,float* amps,float* freqs,float* phases){

		resetPeriodicData(dataSize);

		uint8_t n = 0;
		while(n++ < dataSize){
			(*_amps++) = (*amps++);
			(*_freqs++) = (*freqs++);
			(*_phases++) = (*phases++);
		}

	}

	void setPeriodicData(uint8_t dataSize,const float* amps,const float* freqs,const float* phases){

		resetPeriodicData(dataSize);
		periodicDataSize = dataSize;
		_amps = (float*)amps;
		_freqs = (float*)freqs;
		_phases = (float*)phases;
		isConst = true;

	}

	void resetPeriodicData(uint8_t newSize = 0){

		if(newSize == periodicDataSize) return;
		periodicDataSize = newSize;

		if(!isConst){
			if(_amps != NULL){free(_amps);}
			if(_freqs != NULL){free(_freqs);}
			if(_phases != NULL){free(_phases);}
		}



		if(periodicDataSize != 0){

			_amps = (float*)malloc(periodicDataSize*sizeof(float));
			_freqs = (float*)malloc(periodicDataSize*sizeof(float));
			_phases = (float*)malloc(periodicDataSize*sizeof(float));
			isConst = false;
		}
	}




	// FUNCTIONS

	float circleAround(CircleAroundData data){

#if DEBUG
		float tt = t();
		float out = getInCircle(data.center - data.amp*cos(wt(data.freq,tt)));

		Serial.print("Circle Around -> ");
		Serial.print(tt);
		Serial.print(",");
		Serial.println(out);
		return out;


#else
		return getInCircle(data.center - data.amp*cos(wt(data.freq)));
#endif
	}


//	float circleAround(float offset, float amp, float freq,float phase = 0.0f){
//		return getInCircle(offset + amp*cos(wt(freq) + phase));
//	}

	/** Cosines addition
	* @param size Number of cosines
	* @param amplitudes Amplitudes of each cosine wave.
	* @param freqs Frequencies of each cosine wave. A 0 value will cause a offset level.
	* @param phases Initial phase for each cosine wave. A -HALF_PI value Will convert the cosine in a sine wave.
	*/
	float cosines(uint8_t size,float* amplitudes,float* freqs,float* phases){


		if(size == 0 ) return 0.0;

		float out = 0.0;
		float k;
#if DEBUG
		float tt = t();
		k = wt(1.0,tt);
#else
		k = wt(1.0);
#endif
		uint8_t n = 0;
		while(n++ < size){
			out += (*amplitudes++)*cos(k*(*freqs++) + (*phases++));

		}

		out = getInCircle(out);
#if DEBUG
		Serial.println("cosines");
		Serial.print(tt);
		Serial.print(",");
		Serial.println(out);
#endif

			return out;
		}

	float circle(float f, float ph = 0.0){


#if DEBUG

		float tt = t();
		float out = f*tt + ph/TWO_PI;
		out -= ((int)out);

		Serial.print(tt);
		Serial.print(", ");
		Serial.println(out);
		return out;
#else
		return getInCircle(f*t() + ph/TWO_PI);
#endif


	}


	float circles(uint8_t size,float* amplitudes,float* freqs,float* phases){
		float tt = t();
		float x;
		float out;
		uint8_t n = 0;

		while(n++ < size){
			x = ((*freqs++)*tt) + ((*phases++)/TWO_PI);
				
			 out += ((*amplitudes++)*(x-(int)x));
			//  n++;
			//	out += ((*amplitudes++)*cos(x));
		}
	
		return out;
	}

	/**
	 * Maps a value in the range [0,1)
	 */
	float getInCircle(float f){

		if(abs(f) > 1.0)  return f - (int)f;
		else if(f < 0) return f + 1.0;
		return f;
	}


	/** Periodic oscillation every @period time, the funtion will go from @start to @end and return from @end to start
	 * @param start Initial value
	 * @param end End value
	 * @param period Time taken to go from @start to @end and return from @end to start. This can be read also as how often a value is repeated.
	 * @param minDistance If true, min distance path between end and start will be calculated.
	 */
	float circleFromTo(float start, float end,float period,bool minDistance = false){



		// Clockwise
		float distance = getInCircle(start-end);
		// Clockwise then sustract from start.
		float direction = -1.0;
		// Counterclockwise distance
		float a = getInCircle(end-start);
		// Counterclockwise then add from start
		float b = 1.0;

		if((minDistance && (a < distance)) || ((!minDistance) && (a > distance)))
		{
			swap(distance,a);
			swap(direction,b);
		}


		/* Determinar centro, frecuencia y amplitud
		 * a -> amplitud = direction*distance/.0;
		 * c = from + amplitud
		 * freccuencia = (distancia/time)�/2?
		 */

		// amplitud = direction*distance/.0;

		a = direction*distance/2.0f;
		// frequenzy
		b = 2.0f*distance/period;
//		float c = (start + a);





#if DEBUG
		float tt = t();
		float out = getInCircle((start + a) - a*cos(wt(b,tt)));
		Serial.print("circleFromTo -> ");
		Serial.print(tt);
		Serial.print(",");
		Serial.println(out);
		return out;

#else

		return getInCircle((start + a) - a*cos(wt(b)));
#endif


	}

	CircleAroundData getCircleAroundData(float start, float end,float period,bool minDistance = false){


		// Clockwise

		float distance = getInCircle(start-end);
		// Clockwise then sustract from start.
		float direction = -1.0;
		// Counterclockwise distance
		float a = getInCircle(end-start);
		// Counterclockwise then add from start
		float b = 1.0;

		if((minDistance && (a < distance)) || ((!minDistance) && (a > distance)))
		{
			swap(distance,a);
			swap(direction,b);
		}


		/* Determinar centro, frecuencia y amplitud
		 * a -> amplitud = direction*distance/.0;
		 * c = from + amplitud
		 * freccuencia = (distancia/time)�/2?
		 */

		// amplitud = direction*distance/.0;

		CircleAroundData data;
		data.amp = distance/2.0f;
		// frequenzy
		data.freq = TWO_PI*distance/period;
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
	void setCircleFromTo(float start, float end,float period,bool minDistance = false){



		// Clockwise
		float distance = getInCircle(start-end);
		// Clockwise then sustract from start.
		float direction = -1.0;
		// Counterclockwise distance
		float a = getInCircle(end-start);
		// Counterclockwise then add from start
		float b = 1.0;

		if((minDistance && (a < distance)) || ((!minDistance) && (a > distance)))
		{
			swap(distance,a);
			swap(direction,b);
		}


		/* Determinar centro, frecuencia y amplitud
		 * a -> amplitud = direction*distance/.0;
		 * c = from + amplitud
		 * freccuencia = (distancia/time)�/2?
		 */

		// amplitud = direction*distance/.0;
		a = direction*distance/2.0;
		// frequenzy
		b = 2.0*distance/period;
//		float c = (start + a);

		float amps[] = {(start + a),a};
		float freqs[] = {0.0,b};
		float ph[] = {0.0,PI};
		functionType = COSINES;
		setPeriodicData(2,amps,freqs,ph);
		resetTimer();
	}

//	float oscillateAngle(float center, float amplitude,float freq){
//
//		return inCircle(center + (amplitude/2.0)*cos(wt(freq)));
//	}



	float linearMovement(float x0,float v){return x0 + v*t();}

	void setMode(uint8_t mode){}
	float updateFuntion(){
		switch (functionType) {
			case COSINES: return cosines(periodicDataSize,_amps,_freqs,_phases);
			case CIRCLES : return circles(periodicDataSize,_amps,_freqs,_phases);
			default: return 0.0F;
		}
	}






};



#endif /* FUNCTIONS_H_ */
