#include "daisy_field.h"
#include "daisysp.h" // Uncomment this if you want to use the DSP library.
#include <string>


using namespace daisysp;
using namespace daisy;


// Declare a local daisy_field for hardware access
DaisyField        hw;
Comb              flt;
MoogLadder        moog;
Oscillator        osc_1;
// Oscillator        osc_2;




float   kvals[8];
float   buf[9600];
uint8_t depth;


// This runs at a fixed rate, to prepare audio samples
static void callback(float *in, float *out, size_t size)
{    
    hw.ProcessAnalogControls();
    hw.UpdateDigitalControls();
    

    if(hw.KeyboardState(0))
    {
        osc_1.SetWaveform(Oscillator::WAVE_SIN);        
    }
    if(hw.KeyboardState(1))
    {
        osc_1.SetWaveform(Oscillator::WAVE_TRI);        
    }
    if(hw.KeyboardState(2))
    {
        osc_1.SetWaveform(Oscillator::WAVE_SAW);
    }
    if(hw.KeyboardState(3))
    {
        osc_1.SetWaveform(Oscillator::WAVE_RAMP);
    }
    if(hw.KeyboardState(4))
    {
        osc_1.SetWaveform(Oscillator::WAVE_SQUARE);
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
        sig2 = moog.Process(in[1]);
        sig3 = osc_1.Process();
        
        
        
        flt.SetFreq(hw.GetKnobValue(0) * 4000);
        flt.SetPeriod(hw.GetKnobValue(1) + 0.001);
        flt.SetRevTime(hw.GetKnobValue(2) * 4);

        moog.SetFreq(hw.GetKnobValue(3) * 1000);
        moog.SetRes(hw.GetKnobValue(4));
        // moog.SetFreq(hw.GetCvValue(0) * 4000);
        // moog.SetRes(hw.GetCvValue(1));


        osc_1.SetFreq(hw.GetKnobValue(5) * 2000);
        osc_1.SetAmp(hw.GetKnobValue(6));
        // osc_1.SetFreq(hw.GetCvValue(2) * 2000);
        // osc_1.SetAmp(hw.GetCvValue(3));
        

        hw.SetCvOut1(hw.GetKnobValue(7) * 4095);
        

        float output = sig2;
        
        hw.SetCvOut2(hw.GetKnobValue(7) * (rand() % 4095));

        // left out
        out[i] = sig1;

        // right out
        out[i + 1] = output;
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
    flt.SetPeriod(0.1f);
    flt.SetRevTime(0.385f);

    // initialize Moog Ladder
    moog.Init(sample_rate);
    moog.SetRes(0.7f);

    osc_1.Init(sample_rate);
    
    osc_1.SetFreq(220);
    osc_1.SetWaveform(Oscillator::WAVE_SAW);
    osc_1.SetAmp(1);    


    hw.StartAudio(callback);
    hw.StartAdc();

    
    while(1)
    {
    
    }
}
