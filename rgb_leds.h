#ifndef RGB_LEDS_H_
#define RGB_LEDS_H_

// #if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
// #else
// #include "WProgram.h"
// #endif

void initPins(const uint8_t *pins, uint8_t size, uint8_t mode)
{
	for (uint8_t n = 0; n < size; n++)
		pinMode(pins[n], mode);
}
void initOutputs(const uint8_t *pins, uint8_t size)
{
	initPins(pins, size, OUTPUT);
}
void initInputs(const uint8_t *pins, uint8_t size)
{
	initPins(pins, size, INPUT);
}

class RGBLed
{

protected:
	uint8_t leds[3];
	bool isCC;
	virtual uint8_t modeValue(uint8_t value) { return (isCC ? value : 255 - value); }

public:
	RGBLed() {}
	RGBLed(uint8_t pin_r, uint8_t pin_g, uint8_t pin_b, bool isCommonCathode = true)
	{
		initOutput(pin_r, pin_g, pin_b, isCommonCathode);
	}

	RGBLed(uint8_t *pins, bool isCommonCathode = true)
	{
		initOutput(pins, isCommonCathode);
	}
	virtual ~RGBLed() {}

	void initOutput(const uint8_t *pins, bool isCommonCathode = true)
	{
		isCC = isCommonCathode;
		initOutputs(pins, 3);
		memcpy(leds, pins, 3);
		off();
	}

	void initOutput(uint8_t pin_r, uint8_t pin_g, uint8_t pin_b, bool isCommonCathode = true)
	{
		const uint8_t pins[] = {pin_r, pin_g, pin_b};
		initOutput(pins, isCommonCathode);
	}

	virtual void write(uint8_t r, uint8_t g, uint8_t b)
	{
		analogWrite(leds[0], modeValue(r));
		analogWrite(leds[1], modeValue(g));
		analogWrite(leds[2], modeValue(b));
	}

	void write(const uint8_t *c) { write(c[0], c[1], c[2]); }

	void write(uint8_t *c) { write(c[0], c[1], c[2]); }

	void write(long c)
	{

		long r = 0xff & c;
		long g = 0xff & (c >> 8);
		long b = 0xff & (c >> 16);

		write(r, g, b);
	}

	void off() { write(0, 0, 0); }

	void blink(uint8_t r, uint8_t g, uint8_t b, uint16_t times, uint16_t ms_on, uint16_t ms_off = 0)
	{
		uint16_t o = ms_off == 0 ? ms_on : ms_off;
		for (uint8_t n = 0; n < times; n++)
		{
			write(r,g,b);
			delay(ms_on);
			off();
			delay(o);
		}
	}
	void blink(const uint8_t *c, uint16_t times, uint16_t ms_on,uint16_t ms_off = 0)
	{
		blink(c[0],c[1],c[2],times,ms_on,ms_off);
		
	}

	void blink(uint8_t *c, uint16_t times, uint16_t ms_on,uint16_t ms_off = 0){
		blink((const uint8_t*)c,times,ms_on,ms_off);
	}
};

class DigitalRGBLed : public virtual RGBLed
{

protected:
	uint8_t leds[3];
	bool isCC;
	virtual uint8_t modeValue(uint8_t value) { return (isCC ? value : !value); }

public:
	DigitalRGBLed() : RGBLed() {}
	DigitalRGBLed(uint8_t pin_r, uint8_t pin_g, uint8_t pin_b, bool isCommonCathode = true)
		: RGBLed(pin_r, pin_g, pin_b, isCommonCathode) {}

	DigitalRGBLed(uint8_t *pins, bool isCommonCathode = true)
		: RGBLed(pins, isCommonCathode)
	{
	}
	virtual ~DigitalRGBLed() {}

	virtual void write(uint8_t r, uint8_t g, uint8_t b)
	{
		digitalWrite(leds[0], modeValue(r));
		digitalWrite(leds[1], modeValue(g));
		digitalWrite(leds[2], modeValue(b));
	}

	void write(uint8_t digitalColor)
	{
		for (uint8_t n = 0; n < 3; n++)
			digitalWrite(leds[n], modeValue(bitRead(digitalColor, n)));
	}
};

#endif /* RGB_LEDS_H_ */
