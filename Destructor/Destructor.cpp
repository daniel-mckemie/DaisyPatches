#include "daisy_field.h"
#include "daisysp.h" // Uncomment this if you want to use the DSP library.

#ifndef sample_rate
#endif

using namespace daisysp;
using namespace daisy;


// Declare a local daisy_field for hardware access
DaisyField hw;
Comb       flt;
Oscillator osc_1;
Decimator  decim;


float   kvals[8];
float   buf[9600];

// This runs at a fixed rate, to prepare audio samples
static void callback(float *in, float *out, size_t size)
{  
    hw.ProcessAnalogControls();
    hw.UpdateDigitalControls();

    if(hw.KeyboardState(0))
    {
        decim.SetBitsToCrush(1);
    }
    if(hw.KeyboardState(1))
    {
        decim.SetBitsToCrush(2);
    }
    if(hw.KeyboardState(2))
    {
        decim.SetBitsToCrush(3);
    }
    if(hw.KeyboardState(3))
    {
        decim.SetBitsToCrush(4);
    }
    if(hw.KeyboardState(4))
    {
        decim.SetBitsToCrush(5);
    }
    if(hw.KeyboardState(5))
    {
        decim.SetBitsToCrush(6);
    }
    if(hw.KeyboardState(6))
    {
        decim.SetBitsToCrush(7);
    }
    if(hw.KeyboardState(7))
    {
        decim.SetBitsToCrush(8);
    }
    if(hw.KeyboardState(8))
    {
        decim.SetBitsToCrush(9);
    }
    if(hw.KeyboardState(9))
    {
        decim.SetBitsToCrush(10);
    }
    if(hw.KeyboardState(10))
    {
        decim.SetBitsToCrush(11);
    }
    if(hw.KeyboardState(11))
    {
        decim.SetBitsToCrush(12);
    }
    if(hw.KeyboardState(12))
    {
        decim.SetBitsToCrush(13);
    }
    if(hw.KeyboardState(13))
    {
        decim.SetBitsToCrush(14);
    }
    if(hw.KeyboardState(14))
    {
        decim.SetBitsToCrush(15);
    }
    if(hw.KeyboardState(15))
    {
        decim.SetBitsToCrush(16);
    }


    for(int i = 0; i < 8; i++)
    {
        kvals[i] = hw.GetKnobValue(i);
    }


    float sig1;
    float sig2;
    float sig3;

    for(size_t i = 0; i < size; i += 2)
    {
        sig1 = flt.Process(in[0]);
        sig2 = decim.Process(in[1]);
        sig3 = osc_1.Process();


        flt.SetFreq(hw.GetKnobValue(0) * 8000);
        flt.SetPeriod(hw.GetKnobValue(1) + 0.001);
        flt.SetRevTime(hw.GetKnobValue(2) * 4);

        decim.SetDownsampleFactor(hw.GetKnobValue(3));

        
        
        
        


        osc_1.SetFreq(hw.GetKnobValue(5) * 440);
        osc_1.SetAmp(hw.GetKnobValue(6));
        // osc_1.SetFreq(hw.GetCvValue(0) * 10000);


        // hw.SetCvOut1(sig3);

        float output = sig2;

        hw.SetCvOut2(hw.GetKnobValue(7) * (rand() % 4095));

        // left out
        out[i] = sig1;

        // right out
        out[i + 1] = output + sig3;
    }
}


int main(void)
{
    float sample_rate;
    
    hw.Init();
    sample_rate = hw.SampleRate();


    for(int i = 0; i < 9600; i++)
    {
        buf[i] = 0.0f;
    }

    // initialize Comb object
    flt.Init(sample_rate, buf, 9600);
    flt.SetFreq(500);
    flt.SetPeriod(0.1f);
    flt.SetRevTime(0.385f);

    // initialize Decimator
    decim.Init();
    decim.SetDownsampleFactor(0.4f);
    decim.SetBitsToCrush(8);
    
    
    

    osc_1.Init(sample_rate);
    osc_1.SetFreq(220);
    osc_1.SetWaveform(Oscillator::WAVE_SAW);
    osc_1.SetAmp(1);


    hw.StartAudio(callback);
    hw.StartAdc();


    while(1) {}
}
