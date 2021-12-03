#include <color_animation.h>
#include <millis_utils.h>
const unsigned long baudRate = 115200;

MillisTimer sampleTimer;
AnimationFunctions af;

// struct PeriodicData
// {
//     uint8_t size;
//     float* amps;
//     float* freqs;
//     float* phases;
// };
// #define TEST_DATA                              \
//     {                                          \
//         2, {0.5, 0.5}, {5.0, 10}, { 0.0, 0.0 } \
//     }

const PeriodicDataT<2> data = {Functions::SINE_PULSE, 2, {0.5, 0.5}, {5.0, 10.0}, {0.0, 0.0}};
// // const PeriodicData data;
// const PeriodicData data {TEST_DATA];

void serialInput(Stream &s = Serial)
{

    if (s.available())
    {

        switch (s.read())
        {
        case 't':
            af.setProgram(s.parseInt());
            // debugValue("functionType",af.functionType);
            break;
        case 's':
            af.speed = s.parseFloat();
            break;
        case 'k':
            af.k_param = s.parseFloat();
            break;
        case -1:
            // println("Es -1", s);
            break;
        default:
            break;
        }
    }
}

void setup()
{

    Serial.begin(baudRate);
    af.amp = 1;

    sampleTimer.setFrequency(100, true, true);

    // af.setCircleFromTo(10,20,2,true);

    Serial.println("Data");
    // data.debug();
    // af.setPeriodicData(data.size, (float *)data.amps, (float *)data.freqs, (float *)data.phases);
    af.setPeriodicData(CIRCLE_RAINBOW_PD.size,(float *)CIRCLE_RAINBOW_PD.amps,(float *)CIRCLE_RAINBOW_PD.freqs,(float *)CIRCLE_RAINBOW_PD.phases);
    //   af.setPeriodicData(data.size,data.amps,data.freqs,data.phases);
    af.setFunctionType(data.functionType);

    af.resetTimer();

    //    af.debugData();
}

void loop()
{
    serialInput();
    if (sampleTimer)
    {
        // float values[3];
        // values[0] = af.sines(data.size, (float *)data.amps, (float *)data.freqs, (float *)data.phases);
        // values[2] = af.cosines(data.size, (float *)data.amps, (float *)data.freqs, (float *)data.phases);
        // values[1] = af.circles(data.size, (float *)data.amps, (float *)data.freqs, (float *)data.phases);
        // printArrayln(values, 3);

        print(af.circles(data.size, (float *)data.amps, (float *)data.freqs, (float *)data.phases));
        print(',');
        println(af.output());
    }
}
