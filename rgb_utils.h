#ifndef RGB_UTILS_H_
#define RGB_UTILS_H_

#include <Arduino.h>
#include <math_utils.h>
#include <arduino_utils.h>

#include "colors_defines.h"
#define HUE_MAX 359
#define SATURATION_MAX 255
#define BRIGHTNESS_MAX 255



struct ColorTone

{
    static const uint16_t red = 0;
    static const uint16_t orange = 30;
    static const uint16_t yellow = 60;
    static const uint16_t yellow_green = 90;
    static const uint16_t green = 120;
    static const uint16_t green_blue = 150;
    static const uint16_t cyan = 180;
    static const uint16_t azure = 210;
    static const uint16_t blue = 240;
    static const uint16_t violet = 270;
    static const uint16_t magenta = 300;
    static const uint16_t pink = 330;
    static const uint16_t white = 0xFFFF;
    static const uint16_t low_limit = 1000;
    static const uint16_t match = 1200;
    static const uint16_t candle = 1800;
    static const uint16_t sunset = 2000;
    static const uint16_t warm = 2400;
    static const uint16_t tungsten60 = 2750;
    static const uint16_t tungsten120 = 3000;
    static const uint16_t neutral = 3500;
    static const uint16_t sun_rised = 3501;
    static const uint16_t fluorescent = 4000;
    static const uint16_t flash = 5000;
    static const uint16_t daylight = 5600;
    static const uint16_t cloudy = 6000;
    static const uint16_t cold = 6500;
    static const uint16_t shade = 7000;
    static const uint16_t blue_sky = 10000;
};

/** Based on Kelvin2RGB library code **/
void temperature_to_rgb(uint16_t kelvin, uint8_t brightness, uint8_t &red, uint8_t &green, uint8_t &blue);
/** From Wikipedia **/
void hsv_to_rgb(uint16_t hue, uint8_t saturation, uint8_t value, uint8_t &red, uint8_t &green, uint8_t &blue);

/** From Wikipedia **/
void rgb_to_hsv(uint8_t red, uint8_t green, uint8_t blue, uint16_t &hue, uint8_t &saturation, uint8_t &value);

class RGBOutput;

class HSVOutput
{

public:
    uint16_t hue;
    uint8_t saturation;
    uint8_t value;
    HSVOutput();
    HSVOutput(uint16_t h, uint8_t s, uint8_t v);

    void debugConversion(uint8_t red, uint8_t green, uint8_t blue);
    RGBOutput toRGB();
};

class RGBOutput
{
public:
    uint8_t red;
    uint8_t green;
    uint8_t blue;

    RGBOutput();

    RGBOutput(uint8_t r, uint8_t g, uint8_t b);

    RGBOutput(uint8_t r, uint8_t g, uint8_t b, uint8_t level, uint8_t maxLevel = 255);

    void set(uint8_t r, uint8_t g, uint8_t b);

    void set(uint8_t r, uint8_t g, uint8_t b, uint8_t level, uint8_t maxLevel = 255);

    void set(RGBOutput other);

    bool equals(RGBOutput other);

    bool operator==(RGBOutput other);
    bool operator!=(RGBOutput other);

    void operator=(RGBOutput other);

    void maximize(uint8_t maxValue = 255);

    uint8_t brightness();


    unsigned long toInt();

    HSVOutput toHSV();

    void setTemperature(uint16_t temperature, uint8_t brightness = 255, uint8_t maxBrightness = 255);

    RGBOutput progressTo(unsigned long progress, unsigned long progressStart, unsigned long progressEnd, RGBOutput endV);

    RGBOutput progress100To(unsigned long progress100, RGBOutput endV);

    static RGBOutput PROGRESSION(unsigned long progress, unsigned long progressStart, unsigned long progressEnd, RGBOutput startV, RGBOutput endV);

    static RGBOutput PROGRESSION100(unsigned long progress100, RGBOutput startV, RGBOutput endV);

    /** Based on Kelvin2RGB library code **/
    static RGBOutput FROM_TEMPERATURE(uint16_t temperature, uint8_t brightness = 255, uint8_t maxBrightness = 255);

    static RGBOutput FROM_HSV(uint16_t hue, uint8_t saturation, uint8_t value);
};

// class RGBTransition : public TimedInterpolationBase<RGBOutput>
// {

// public:
//     RGBOutput value(RGBOutput &startV, RGBOutput endV, unsigned long transitionTime, bool autoChangeStartV = true)
//     {

//         RGBOutput out;
//         out.set(startV);
//         unsigned long t = millis();
//         this->startCheck(startV, endV);

//         if (this->canCalculate(startV, endV))
//         {

//             unsigned long endT = startTime + transitionTime;
//             out = RGBOutput::PROGRESSION(t, startTime, endT, startV, endV);
//             // out.red = (uint16_t)mapFromStartToEnd(t, startTime, endT, (unsigned long)startV.red, (unsigned long)endV.red);
//             // out.green = (uint16_t)mapFromStartToEnd(t, startTime, endT, (unsigned long)startV.green, (unsigned long)endV.green);
//             // out.blue = (uint16_t)mapFromStartToEnd(t, startTime, endT, (unsigned long)startV.blue, (unsigned long)endV.blue);
//         }

//         this->endCheck(out, startV, endV, autoChangeStartV);

//         return out;
//     }
// };

// class Kolor
// {

// protected:
//     RGBOutput _rgb;
//     HSVOutput _hsv;

// };

class Color
{
protected:
    uint16_t _hue;
    uint8_t _red, _green, _blue, _saturation, _brightness;

    void updateHSV()
    {
        rgb_to_hsv(_red, _green, _blue, _hue, _saturation, _brightness);
    }

    void updateRGB()
    {

        hsv_to_rgb(_hue, _saturation, _brightness, _red, _green, _blue);
    }

public:
    /** Default constructor **/
    Color()
    {
        setRGB(0, 0, 0);
    }
    /** RGB constructor **/
    Color(uint8_t red, uint8_t green, uint8_t blue)
    {
        setRGB(red, green, blue);
    }

     /** RGB constructor **/
    Color(const uint8_t *rgb)
    {
        setRGB(rgb);
    }

     /** RGB constructor **/
    Color(uint8_t *rgb)
    {
        setRGB(rgb);
    }

    /** SETTERS **/

    /** Set RGB components [0,255] **/
    void setRGB(uint8_t red, uint8_t green, uint8_t blue)
    {
        _red = red;
        _green = green;
        _blue = blue;

        updateHSV();
    }

    /** Set RGB components [0,255] **/
    void setRGB(const uint8_t *rgb)
    {
        setRGB(rgb[0], rgb[1], rgb[2]);
    }
    /** Set RGB components [0,255] **/
    void setRGB(uint8_t *rgb) { setRGB((const uint8_t *)rgb); }

    /** Set red component [0,255] */
    void setRed(uint8_t r)
    {
        _red = r;
        updateHSV();
    }

    /** Set green component [0,255] */
    void setGreen(uint8_t g)
    {
        _green = g;
        updateHSV();
    }

    /** Set blue component [0,255] */
    void setBlue(uint8_t b)
    {
        _blue = b;
        updateHSV();
    }

    /** Sets hue[0,359],saturation[0,255] and value[0,255] components **/
    void setHSV(uint16_t hue, uint8_t saturation, uint8_t value)
    {
        _hue = hue;
        _saturation = saturation;
        _brightness = value;
        updateRGB();
    }

    /** Sets hue[0,359],saturation[0,255] and brightness[0,255] components **/
    void setHSB(uint16_t hue, uint8_t saturation, uint8_t brightness)
    {
        setHSV(hue, saturation, brightness);
    }

    /** Sets hue component [0,359] **/
    void setHue(uint16_t hue)
    {
        _hue = hue;
        updateRGB();
    }

    /** Sets saturation component [0,255] **/
    void setSaturation(uint8_t saturation)
    {

        _saturation = saturation;
        updateRGB();
    }

    /** Sets saturation component [0,255] **/
    void setBrightness(uint8_t brightness)
    {
        _brightness = brightness;
        updateRGB();
    }

    /** Set brightness [0,255]. RGB values will be updated. */
    void setValue(uint8_t value)
    {
        _brightness = value;
        updateRGB();
    }

    /** Sets from other */
    void set(Color color)
    {
        setRGB(color.red(), color.green(), color.blue());
    }
    /** Sets from RGBOutput */
    void set(RGBOutput other)
    {
        setRGB(other.red, other.green, other.blue);
    }

    /** Sets from HSVOutput */
    void set(HSVOutput other)
    {
        setHSV(other.hue, other.saturation, other.value);
    }

    /** Copies color from int*/
    void set(unsigned long color)
    {
        setRGB(0xff & color, 0xff & (color >> 8), 0xff & (color >> 16));
    }

    /** Sets color temperature [0,65500]  and brightness **/
    void setTemperature(uint16_t kelvin, uint8_t brightness)
    {
        temperature_to_rgb(kelvin, brightness, _red, _green, _blue);
        updateHSV();
    }
    /** Sets color temperature in kelving **/
    void setTemperature(uint16_t kelvin)
    {
        setTemperature(kelvin, brightness());
    }

    /** GETTERS **/

    /** Gets red component [0,255] **/
    uint8_t red() { return _red; }

    /** Gets green component [0,255] **/
    uint8_t green() { return _green; }

    /** Gets blue component [0,255] **/
    uint8_t blue() { return _blue; }

    /** Gets hue component [0,359] **/
    uint16_t hue() { return _hue; }

    /** Gets saturation component [0,255] **/
    uint8_t saturation() { return _saturation; }

    /** Gets brightness component [0,255] **/
    uint8_t brightness() { return _brightness; }

    /** Gets value component aka brightness [0,255] **/
    uint8_t value() { return _brightness; }

    /** Returns RGB as int **/
    unsigned long toInt()
    {
        unsigned long r = 0xff & _red;
        unsigned long g = 0xff & _green;
        unsigned long b = 0xff & _blue;
        return r | (g << 8) | (b << 16);
    }

    /** Returns as RGBOutput **/
    RGBOutput toRGB()
    {
        return RGBOutput(_red, _green, _blue);
    }
    /** Copies rgb to input array **/
    void toRGB(uint8_t *rgb)
    {
        rgb[0] = _red;
        rgb[1] = _green;
        rgb[2] = _blue;
    }
    /** Returns as HSVOutput **/
    HSVOutput toHSV()
    {
        return HSVOutput(_hue, _saturation, _brightness);
    }
    /** Copies hsv to input array **/
    void toHSV(uint16_t *hsv)
    {
        hsv[0] = _hue;
        hsv[1] = _saturation;
        hsv[2] = _brightness;
    }
};

#endif