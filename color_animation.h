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
#define BEAT_FREQ 0.5
// #define BLINK_TIMES 4
// #define CIRCLE_FREQ 0.25


static const NPeriodicData<2> RAINBOW1_PD = {Functions::COSINES, 2, {0.5, -0.5}, {0, 0.05}, {0.0, PI}};
static const NPeriodicData<3> RAINBOW2_PD = {Functions::COSINES,3, {0.25, 0.5, 0.25}, {0, 0.05, 0.4}, {0.0, 0.0, 0.0}};
static const NPeriodicData<2> FAST_RAINBOW_PD = {Functions::COSINES,2, {0.5, 0.5}, {0, 0.4}, {0.0, PI}};
// static const PeriodicDataT<2> CIRCLE_RAINBOW_PD = {Functions::CIRCLES,2, {0.9, 0.1}, {0.1, 1}, {0.0, 0}};
static const NPeriodicData<2> CIRCLE_RAINBOW_PD = {Functions::CIRCLES,1, {1}, {0.1}, {0}};
static const NPeriodicData<2> BEAT_PD = {Functions::COSINES,2, {0.5, 0.5}, {0, BEAT_FREQ}, {0.0, PI}};

class AnimationFunctions : public Functions
{

	// NON-BLOCKING METHODS
public:

	static const byte MIN_ANIMATION_ID = 128;
	typedef enum
	{
		BEATING = MIN_ANIMATION_ID,
		RAINBOW1,
		RAINBOW2,
		FAST_RAINBOW,
		CIRCLE_RAINBOW
	} FunctionTypes;

	float amp;
	bool positive;
	byte program;

	AnimationFunctions() : Functions()
	{
		positive = false;
		program = NO_FUNCTION;
	}
	virtual ~AnimationFunctions(){

	}

	float beat()
	{	
		return amp * cosines(BEAT_PD.size, (float *)BEAT_PD.amps, (float *)BEAT_PD.freqs, (float *)BEAT_PD.phases);
	}

	float rainbow1()
	{
		return amp * cosines(RAINBOW1_PD.size, (float *)RAINBOW1_PD.amps, (float *)RAINBOW1_PD.freqs, (float *)RAINBOW1_PD.phases);
	}

	float rainbow2()
	{
		return amp * cosines(RAINBOW2_PD.size, (float *)RAINBOW2_PD.amps, (float *)RAINBOW2_PD.freqs, (float *)RAINBOW2_PD.phases);
	}

	float fastRainbow()
	{
		return amp * cosines(FAST_RAINBOW_PD.size, (float *)FAST_RAINBOW_PD.amps, (float *)FAST_RAINBOW_PD.freqs, (float *)FAST_RAINBOW_PD.phases);
	}

	float circleRainbow()
	{
		return amp * cosines(CIRCLE_RAINBOW_PD.size, (float *)CIRCLE_RAINBOW_PD.amps, (float *)CIRCLE_RAINBOW_PD.freqs, (float *)CIRCLE_RAINBOW_PD.phases);
	}

	void setProgram(unsigned int p)

	{
		if( p == program ) return;

		program = p;

		if (program < MIN_ANIMATION_ID)
		{
			setFunctionType(program);
		}
		else
		{

			switch (program)
			{
			case BEATING:
				setFunctionType(BEAT_PD.functionType);
				setPeriodicData(BEAT_PD.size, (float *)BEAT_PD.amps, (float *)BEAT_PD.freqs, (float *)BEAT_PD.phases);

				break;
			case RAINBOW1:
				setFunctionType(RAINBOW1_PD.functionType);
				setPeriodicData(RAINBOW1_PD.size, (float *)RAINBOW1_PD.amps, (float *)RAINBOW1_PD.freqs, (float *)RAINBOW1_PD.phases);

				break;
			case RAINBOW2:
				setFunctionType(RAINBOW2_PD.functionType);
				setPeriodicData(RAINBOW2_PD.size, (float *)RAINBOW2_PD.amps, (float *)RAINBOW2_PD.freqs, (float *)RAINBOW2_PD.phases);

				break;
			case FAST_RAINBOW:
				setFunctionType(FAST_RAINBOW_PD.functionType);
				setPeriodicData(FAST_RAINBOW_PD.size, (float *)FAST_RAINBOW_PD.amps, (float *)FAST_RAINBOW_PD.freqs, (float *)FAST_RAINBOW_PD.phases);

				break;
			case CIRCLE_RAINBOW:
				setFunctionType(CIRCLE_RAINBOW_PD.functionType);
				setPeriodicData(CIRCLE_RAINBOW_PD.size, (float *)CIRCLE_RAINBOW_PD.amps, (float *)CIRCLE_RAINBOW_PD.freqs, (float *)CIRCLE_RAINBOW_PD.phases);

				break;

			default:
				break;
			}
		}
	}

	float output()
	{
		return amp * (positive ? positiveWave(value()) : value());
	}

	bool isAnimating(){
		return program > 0;
	}
};
class ColorAnimation : public Color
{

public:
	AnimationFunctions brightnessAnimation;

	AnimationFunctions hueAnimation;
	// byte animationMode;

	uint16_t _prevHue;

	ColorAnimation() : Color(), brightnessAnimation(), hueAnimation()
	{
		brightnessAnimation.amp = BRIGHTNESS_MAX;
		hueAnimation.amp = HUE_MAX;

		brightnessAnimation.speed = hueAnimation.speed = 1;
	}
	virtual ~ColorAnimation() {}

	// Color *color() { return (Color *)this; }

	void resetAllTimers()
	{
		brightnessAnimation.resetTimer();
		hueAnimation.resetTimer();
	}

	void setBrightnessAnimation(byte animation)
	{
		brightnessAnimation.setProgram(animation);
		if (animation == NO_ANIMATION)
		{

			setBrightness(BRIGHTNESS_MAX);
		}
		else
		{
			brightnessAnimation.resetTimer();
		}
	}

	void setColorAnimation(byte animation)
	{
		hueAnimation.setProgram(animation);
		if (animation == NO_ANIMATION)
		{
			setHue(_prevHue);
		}
		else
		{

			_prevHue = _hue;
			hueAnimation.resetTimer();
		}
	}

	void setAnimation(byte animation)
	{

		if (animation == NO_ANIMATION)
		{
			setColorAnimation(NO_ANIMATION);
			setBrightnessAnimation(NO_ANIMATION);
		}
		else if (animation == AnimationFunctions::BEATING)
		{

			setBrightnessAnimation(AnimationFunctions::BEATING);
		}
		else
		{
			setColorAnimation(animation);
		}
	}


	bool beatColor2Zero()
	{
		setBrightness(brightnessAnimation.beat());
		return (_brightness > 1);
	}

	//	void beatColor2Zero(){
	//		do{beatColor();
	//		}while (brightness>1);
	//	}

	// void beatColor(/*float timesWT = 6*/)
	// {

	// 	float brightness = 255 * brightnessAnimation.cosines(BEAT_N, (float *)BEAT_AMPS, (float *)BEAT_FREQS, (float *)BEAT_PHASES, 2.0);
	// 	setBrightness(brightness);
	// }

	// void circleRainbow(uint16_t center, float freq)
	// {
	// 	//		float hue = circle(CIRCLE_FREQ);
	// 	float circle = hueAnimation.circles(RC_N, (float *)RC_AMPS, (float *)RC_FREQS, (float *)RC_PHASES);
	// 	uint16_t hue = uint16_t(center + (float(HUE_MAX) * circle)) % HUE_MAX;

	// 	// setHSB(hue_in_range(hue+_prevHue),SATURATION_MAX,BRIGHTNESS_MAX);
	// 	setHue(hue + _prevHue);
	// }

	// void allRandom()
	// {
	// 	setRGB(random(0, 255), random(0, 255), random(0, 255));
	// }


	void updateAnimation()
	{

		
		if (isColorAnimating()){
			setHue(_prevHue  +hueAnimation.output() );
		}

		setBrightness(brightnessAnimation.isAnimating() ? (uint8_t)brightnessAnimation.output() : BRIGHTNESS_MAX);
/*
		// MIXED MODE
		uint16_t h = _prevHue;
		if(hueAnimation.isAnimating()){
			h+= hueAnimation.output();
		}
		uint8_t b = brightnessAnimation.isAnimating() ? (uint8_t)brightnessAnimation.output() : BRIGHTNESS_MAX;
		uint8_t s = SATURATION_MAX;

		setHSV(h, s, b);
		*/
	}

	bool isBrightnessAnimating()
	{
		return brightnessAnimation.isAnimating();
	}

	bool isHueAnimating()
	{
		return hueAnimation.isAnimating();
	}

	
	bool isColorAnimating()
	{
		return hueAnimation.isAnimating();
	}

	bool isAnimating()
	{
		return isBrightnessAnimating() || isColorAnimating();
	}
};

#endif /* COLOR_ANIMATION_H_ */
