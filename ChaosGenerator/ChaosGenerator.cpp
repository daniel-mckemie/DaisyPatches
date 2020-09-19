#include "daisy_field.h"
#include "daisysp.h" // Uncomment this if you want to use the DSP library.

using namespace daisysp;
using namespace daisy;


// Declare a local daisy_field for hardware access
DaisyField        hw;
Comb              flt;
static WhiteNoise noise;
static Bitcrush   bitcrush;
static Metro      metro;


float kvals[8];
float buf[9600];
uint8_t depth;
// This runs at a fixed rate, to prepare audio samples
static void callback(float *in, float *out, size_t size)
{
    hw.ProcessAnalogControls();    

    for(int i = 0; i < 8; i++)
    {
        kvals[i] = hw.GetKnobValue(i);
    }

    if(metro.Process())
    {
        depth++;
        depth %= 6;
    }

    float sig1;
    float sig2;
    for(size_t i = 0; i < size; i += 2)
    {
        sig1 = noise.Process();
        flt.SetFreq(hw.GetKnobValue(0) * 4095);
        flt.SetPeriod(hw.GetKnobValue(1));
        flt.SetRevTime(hw.GetKnobValue(2) * 5);
        bitcrush.SetBitDepth((hw.GetKnobValue(3) * 1.0f) * depth + 2);
        bitcrush.SetCrushRate((hw.GetKnobValue(4) + 2) * 2000);
        sig2 = 0.5 * bitcrush.Process(sig2);

        sig1 = 0.5 * flt.Process(sig1);

        // left out
        out[i] = (sig1 + sig2);

        // right out
        out[i + 1] = (sig1 + sig2);
    }
}



int main(void)
{
    float sample_rate;
    depth = 1;


    hw.Init();
    sample_rate = hw.SampleRate();

    for(int i = 0; i < 9600; i++)
    {
        buf[i] = 0.0f;
    }

    // initialize Comb object
    flt.Init(sample_rate, buf, 9600);
    flt.SetFreq(500);
    flt.SetPeriod(0.1);
    flt.SetRevTime(0.1);

    //initialize noise
    noise.Init();
    bitcrush.Init(sample_rate);
    metro.Init(1.0f, sample_rate);

    //set parameters for bitcrusher
    bitcrush.SetBitDepth(6);
    bitcrush.SetCrushRate(10000);


    hw.StartAudio(callback);
    hw.StartAdc();


    while(1)
    {
              
    }
}
