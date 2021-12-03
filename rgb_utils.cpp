#include "rgb_utils.h"

uint16_t hue_in_range(int hue)
{
    int h = HUE_MAX +1;
    while (hue < 0)
    {
        hue += (h);
    }

    return (uint16_t)(hue % (h));
}

void temperature_to_rgb(uint16_t kelvin, uint8_t brightness, uint8_t &red, uint8_t &green, uint8_t &blue)
{
    float temp = (constrain((float)kelvin, 0.0, 65500.0)) / 100.0;

    float r = 255.0;
    float g;
    float b = 255.0;

    if (temp > 66.0)
    {
        r = 329.698727466 * pow(temp - 60.0, -0.1332047592);
        g = 288.1221695283 * pow(temp - 60.0, -0.0755148492);
    }
    else
    {
        g = (99.4708025861 * log(temp)) - 161.1195681661;

        // blue
        if (temp <= 19.0)
        {
            b = 0;
        }
        else
        {
            b = (138.5177312231 * log(temp - 10.0)) - 305.0447927307;
        }
    }
    red = map(brightness, 0, 255, 0, (uint8_t)constrain(r, 0, 255));
    green = map(brightness, 0, 255, 0, (uint8_t)constrain(g, 0, 255));
    blue = map(brightness, 0, 255, 0, (uint8_t)constrain(b, 0, 255));
}

/** From Wikipedia **/
void hsv_to_rgb(uint16_t hue, uint8_t saturation, uint8_t value, uint8_t &red, uint8_t &green, uint8_t &blue)
{
    float s = ((float)saturation) / 255.0;
    float v = ((float)value) / 255.0;
    float c = v * s;
    float h = ((float)(hue % 360)) / 60.0;
    float x = c * (1 - abs(floatMod(h, 2) - 1));
    float m = v - c;

    float r, g, b;

    if (h < 1.0)
    {
        r = c;
        g = x;
        b = 0;
    }
    else if (h < 2.0)
    {
        r = x;
        g = c;
        b = 0;
    }
    else if (h < 3.0)
    {
        r = 0;
        g = c;
        b = x;
    }
    else if (h < 4.0)
    {
        r = 0;
        g = x;
        b = c;
    }
    else if (h < 5.0)
    {
        r = x;
        g = 0;
        b = c;
    }
    else if (h < 6.0)
    {
        r = c;
        g = 0;
        b = x;
    }

    r += m;
    g += m;
    b += m;

    red = (uint8_t)(255.0 * r);
    green = (uint8_t)(255.0 * g);
    blue = (uint8_t)(255.0 * b);
}

/** From Wikipedia **/
void rgb_to_hsv(uint8_t red, uint8_t green, uint8_t blue, uint16_t &hue, uint8_t &saturation, uint8_t &value)
{

    uint8_t maxRGB = max(red, max(green, blue));
    uint8_t minRGB = min(red, min(green, blue));

    hue = 0;
    saturation = 0;
    value = maxRGB;

    if (maxRGB != minRGB)
    {
        float r = ((float)red) / 255.0;
        float g = ((float)green) / 255.0;
        float b = ((float)blue) / 255.0;

        float mx = ((float)maxRGB / 255.0);
        float mn = ((float)minRGB / 255.0);
        float diff = mx - mn;
        // float h = 60.0 / inc;
        float h = 60.0 / (diff);

        if (maxRGB == red)
        {

            h *= (g - b);
            if (green < blue)
            {
                h += 360.0;
            }
        }
        else if (maxRGB == green)
        {
            h *= (b - r);
            h += 120;
        }
        else if (maxRGB == blue)
        {
            h *= (r - g);
            h += 240;
        }

        saturation = (uint8_t)(255 * (1 - mn / mx));
        hue = h;
        hue %= 360;
    }
}

HSVOutput::HSVOutput()
{
}
HSVOutput::HSVOutput(uint16_t h, uint8_t s, uint8_t v)
{
    hue = h;
    saturation = s;
    value = v;
}

void HSVOutput::debugConversion(uint8_t red, uint8_t green, uint8_t blue)
{

    rgb_to_hsv(red, green, blue, hue, saturation, value);

    char s[12];
    snprintf_P(s, 12, PSTR("%03d,%03d,%03d"), red, green, blue);
    debugValue("Origin RGB", s);
    snprintf_P(s, 12, PSTR("%03d,%03d,%03d"), hue, saturation, value);
    debugValue("Converted HSV", s);
}

RGBOutput HSVOutput::toRGB()
{
    RGBOutput o = RGBOutput::FROM_HSV(hue, saturation, value);
    return o;
}

RGBOutput::RGBOutput()
{
    red = green = blue = 0;
}
RGBOutput::RGBOutput(uint8_t r, uint8_t g, uint8_t b)
{
    set(r, g, b);
}

RGBOutput::RGBOutput(uint8_t r, uint8_t g, uint8_t b, uint8_t level, uint8_t maxLevel)
{
    set(r, g, b, level, maxLevel);
}

void RGBOutput::set(uint8_t r, uint8_t g, uint8_t b)
{

    red = r;
    green = g;
    blue = b;
}

void RGBOutput::set(uint8_t r, uint8_t g, uint8_t b, uint8_t level, uint8_t maxLevel)
{

    red = ((r * level) / maxLevel);
    green = ((g * level) / maxLevel);
    blue = ((b * level) / maxLevel);
}

void RGBOutput::set(RGBOutput other)
{
    set(other.red, other.green, other.blue);
}
bool RGBOutput::equals(RGBOutput other)
{

    return ((other.red == red) && (other.green == green) && (other.blue == blue)) ? true : false;
}

bool RGBOutput::operator==(RGBOutput other)
{
    return equals(other);
}
bool RGBOutput::operator!=(RGBOutput other)
{
    return !equals(other);
}

void RGBOutput::operator=(RGBOutput other)
{
    set(other);
}

void RGBOutput::maximize(uint8_t maxValue)
{

    uint8_t maxComponent = brightness();
    // uint16_t scale = maxValue / maxComponent;

    if (maxComponent > 0)
    {
        red = ((red * maxValue) / maxComponent) % (maxValue + 1);
        green = ((green * maxValue) / maxComponent) % (maxValue + 1);
        blue = ((blue * maxValue) / maxComponent) % (maxValue + 1);
    }
}

uint8_t RGBOutput::brightness()
{
    return max(red, max(green, blue));
}

unsigned long RGBOutput::toInt()
{
    unsigned long r = 0xff & red;
    unsigned long g = 0xff & green;
    unsigned long b = 0xff & blue;
    return r | (g << 8) | (b << 16);
}
HSVOutput RGBOutput::toHSV()
{
    HSVOutput o;
    rgb_to_hsv(red, green, blue, o.hue, o.saturation, o.value);
    return o;
}

void RGBOutput::setTemperature(uint16_t temperature, uint8_t brightness, uint8_t maxBrightness)
{
    RGBOutput o = FROM_TEMPERATURE(temperature, brightness, maxBrightness);
    set(o);
}
RGBOutput RGBOutput::progressTo(unsigned long progress, unsigned long progressStart, unsigned long progressEnd, RGBOutput endV)
{

    if (progress >= progressEnd)
    {
        set(endV);
        return endV;
    }

    RGBOutput s = RGBOutput(red, green, blue);
    return PROGRESSION(progress, progressStart, progressEnd, s, endV);
}

RGBOutput RGBOutput::progress100To(unsigned long progress100, RGBOutput endV)
{
    return progressTo(progress100, 0, 100, endV);
}

RGBOutput RGBOutput::PROGRESSION(unsigned long progress, unsigned long progressStart, unsigned long progressEnd, RGBOutput startV, RGBOutput endV)
{
    RGBOutput out;

    out.red = interpolate(progress, progressStart, progressEnd, (unsigned long)startV.red, (unsigned long)endV.red);
    out.green = interpolate(progress, progressStart, progressEnd, (unsigned long)startV.green, (unsigned long)endV.green);
    out.blue = interpolate(progress, progressStart, progressEnd, (unsigned long)startV.blue, (unsigned long)endV.blue);
    return out;
}

RGBOutput RGBOutput::PROGRESSION100(unsigned long progress100, RGBOutput startV, RGBOutput endV)
{
    return PROGRESSION(progress100, 0, 100, startV, endV);
}
/** Based on Kelvin2RGB library code **/
RGBOutput RGBOutput::FROM_TEMPERATURE(uint16_t temperature, uint8_t brightness, uint8_t maxBrightness)
{
    RGBOutput o;
    temperature_to_rgb(temperature, brightness, (uint8_t &)o.red, (uint8_t &)o.green, (uint8_t &)o.blue);
    return o;
}

RGBOutput RGBOutput::FROM_HSV(uint16_t hue, uint8_t saturation, uint8_t value)
{
    RGBOutput o;
    hsv_to_rgb(hue, saturation, value, (uint8_t &)o.red, (uint8_t &)o.green, (uint8_t &)o.blue);
    return o;
}
