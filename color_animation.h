/*
 * color_animation.h
 *
 *  Created on: 30/09/2013
 *      Author: J.M. Tomé
 */

#ifndef COLOR_ANIMATION_H_
#define COLOR_ANIMATION_H_

//#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
//#else
// #include "WProgram.h"
// #endif
#include "rgb_utils.h"
#include "functions.h"

// EFFECT MODES
#define NO_ANIMATION 0
#define MODE_BEATING 1
#define MODE_RAINBOW1 2
#define MODE_RAINBOW2 3
#define MODE_FAST_RAINBOW 4
#define MODE_CIRCLE_RAINBOW 5
#define MODES_NUMBER 5
#define MODE_RANDOM 6

// beating
//#define BEAT_AMP 0.08
#define BEAT_FREQ 0.5
#define BLINK_TIMES 4
#define CIRCLE_FREQ 0.25

const int R1_N = 2;
const float R1_AMPS[] = {0.5,0.5};
const float R1_FREQS[] = {0.0,0.05};
const float R1_PHASES[] = {0.0,PI};


//#define R1_N  2
//#define R1_AMPS (0.5f,-0.5f)
//#define R1_FREQS (0.0f,0.05f)
//#define R1_PHASES (0.0f,0.0f)

const int R2_N = 3;
const float R2_AMPS[] = {0.25,0.5,0.5};
const float R2_FREQS[] = {0.0,0.05,0.4};
const float R2_PHASES[] = {0.0,0.0,0.0};

const int RF_N = 2;
const float RF_AMPS[] = {0.5,0.5};
const float RF_FREQS[] = {0.0,0.4};
const float RF_PHASES[] = {0.0,PI};

const int RC_N = 1;
const float RC_AMPS[] = {1};
const float RC_FREQS[] = {0.25};
const float RC_PHASES[] = {0.0};


const int BEAT_N = 2;
const float BEAT_AMPS[] = {0.5,0.5};
const float BEAT_FREQS[] = {0,BEAT_FREQ};
const float BEAT_PHASES[] = {0.0,PI};

// these are the pins connected to
// the RGB LEDs


class ColorAnimation : public Color{


public :


	Functions f;
	byte animationMode;
	uint16_t _prevHue;
	ColorAnimation():Color(),f() {}
	virtual ~ColorAnimation(){}

	Color* color(){return (Color*)this;}
	void resetEffectsTimer(){ f.resetTimer();}

	void setAnimation(byte animation){
		f.resetTimer();
		_prevHue = _hue;
		animationMode = animation;
	}
	bool beatColor2Zero(){
		beatColor();
		return (_brightness>1);
	}

//	void beatColor2Zero(){
//		do{beatColor();
//		}while (brightness>1);
//	}

	// NON-BLOCKING METHODS

	void beatColor(/*float timesWT = 6*/){

		float brightness = 255*f.cosines(BEAT_N,(float*)BEAT_AMPS,(float*)BEAT_FREQS,(float*)BEAT_PHASES);
		setBrightness(brightness);

	}




	void rainbow1(){
		float hue = HUE_MAX*f.cosines(R1_N,(float*)R1_AMPS,(float*)R1_FREQS,(float*)R1_PHASES);
		setHSB(hue_in_range(hue+_prevHue),SATURATION_MAX,BRIGHTNESS_MAX);

	}

	void rainbow2(){
	  float hue = HUE_MAX*f.cosines(R2_N,(float*)R2_AMPS,(float*)R2_FREQS,(float*)R2_PHASES);
	  setHSB(hue_in_range(hue+_prevHue),SATURATION_MAX,BRIGHTNESS_MAX);
	}

	void fastRainbow(){
		float hue = HUE_MAX*f.cosines(RF_N,(float*)RF_AMPS,(float*)RF_FREQS,(float*)RF_PHASES);
		setHSB(hue_in_range(hue+_prevHue),SATURATION_MAX,BRIGHTNESS_MAX);
	}

	void circleRainbow(){
		float hue = HUE_MAX*f.circles(RC_N,(float*)RC_AMPS,(float*)RC_FREQS,(float*)RC_PHASES);
		// Serial.println(hue);
		setHSB(hue_in_range(hue+_prevHue),SATURATION_MAX,BRIGHTNESS_MAX);
	}

	void circleRainbow(uint16_t center, float freq){
	//		float hue = circle(CIRCLE_FREQ);
			float circle = f.circles(RC_N,(float*)RC_AMPS,(float*)RC_FREQS,(float*)RC_PHASES);
			uint16_t hue = uint16_t(center + (float(HUE_MAX)*circle))%HUE_MAX;
			
			setHSB(hue_in_range(hue+_prevHue),SATURATION_MAX,BRIGHTNESS_MAX);
	}

	void allRandom(){
		setRGB(random(0,255),random(0,255),random(0,255));
	}

	void updateAnimation(){

		 switch (animationMode) {
				case MODE_BEATING: beatColor(); break;
				case MODE_RAINBOW1: rainbow1();	break;
				case MODE_RAINBOW2: rainbow2();	break;
				case MODE_FAST_RAINBOW: fastRainbow();	break;
				case MODE_CIRCLE_RAINBOW: circleRainbow();	break;
				case MODE_RANDOM: allRandom();
				default: break;
			}
	}
};


#endif /* COLOR_ANIMATION_H_ */
