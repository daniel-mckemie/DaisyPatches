#include "daisy_field.h"
#include "daisysp.h" // Uncomment this if you want to use the DSP library.


using namespace daisysp;
using namespace daisy;




// Declare a local daisy_field for hardware access
DaisyField        hw;
static Oscillator osc;
static MoogFilter filt;

float kvals[8];


// This runs at a fixed rate, to prepare audio samples
static void callback(float *in, float *out, size_t size)
{
    hw.ProcessAnalogControls();

    for(int i = 0; i < 8; i++)
    {
        kvals[i] = hw.GetKnobValue(i);        
    }

    float osc_sig;
    float output;
    
    for(size_t i = 0; i < size; i += 2)
    {
        
        osc_sig = osc.Process();
        osc.SetFreq(hw.GetKnobValue(0) * 1000);

        filt.SetFreq(hw.GetKnobValue(1) * 1000);
        filt.SetRes(hw.GetKnobValue(2));

        output = filt.Process(osc_sig);

        
        
        // left out
        out[i] = output;

        // right out
        out[i + 1] = output;
    }
}





int main(void)
{
    float sample_rate;    
    
     
    hw.Init();
    sample_rate = hw.SampleRate();

    osc.Init(sample_rate);
    
    // Set parameters for oscillator
    osc.SetWaveform(osc.WAVE_SAW);
    osc.SetFreq(100);        
    osc.SetAmp(0.5);

    filt.Init(sample_rate);
    // filt.SetFreq(1000.0f);
    // filt.SetRes(0.4f);

    hw.StartAudio(callback);
    hw.StartAdc();


    while(1)
    {

    }
}
