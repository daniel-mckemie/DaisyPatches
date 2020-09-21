#include "daisy_field.h"
#include "daisysp.h" // Uncomment this if you want to use the DSP library.
#include <string>


using namespace daisysp;
using namespace daisy;


// Declare a local daisy_field for hardware access
DaisyField        hw;
Comb              flt;
Biquad            flt_2;
static WhiteNoise noise;
Oscillator        osc;
// static Bitcrush   bitcrush;
static Metro      metro;


float kvals[8];
float buf[2400];
uint8_t depth;


// This runs at a fixed rate, to prepare audio samples
static void callback(float *in, float *out, size_t size)
{
    hw.ProcessAnalogControls();
    hw.UpdateDigitalControls();

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
    float filter_sig;
    for(size_t i = 0; i < size; i += 2)
    {
        sig1 = in[0];
        filter_sig = osc.Process();
        flt.SetFreq(hw.GetKnobValue(0) * 100);
        flt.SetPeriod(hw.GetKnobValue(1) + 0.001);
        flt.SetRevTime(hw.GetKnobValue(2) * 5);
        

        // bitcrush.SetBitDepth((hw.GetKnobValue(3) * 1.0f) * depth + 2);
        // bitcrush.SetCrushRate((hw.GetKnobValue(4) + 2) * 2000);


        osc.SetFreq(hw.GetKnobValue(3) * 2000);
        osc.SetAmp(hw.GetKnobValue(4));

        flt_2.SetCutoff(hw.GetKnobValue(5) * 2000);
        flt_2.SetRes(hw.GetKnobValue(6));


        sig1 = flt.Process(sig1);
    
        sig2 = hw.GetKnobValue(7) * flt_2.Process(filter_sig);

    

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
    // uint32_t last_led_update, led_period, now;

    // led_period = 5;
    // last_led_update = now = dsy_system_getnow();


    for(int i = 0; i < 2400; i++)
    {
        buf[i] = 0.0f;
    }

    // initialize Comb object
    flt.Init(sample_rate, buf, 2400);
    flt.SetFreq(500);
    flt.SetPeriod(0.1);
    flt.SetRevTime(0.1);

    //initialize noise
    noise.Init();
    // bitcrush.Init(sample_rate);
    osc.Init(sample_rate);
    flt_2.Init(sample_rate);    
    metro.Init(1.0f, sample_rate);

    // //set parameters for bitcrusher
    // bitcrush.SetBitDepth(6);
    // bitcrush.SetCrushRate(10000);

    osc.SetFreq(220);
    osc.SetWaveform(Oscillator::WAVE_POLYBLEP_SAW);
    osc.SetAmp(0.5);
    flt_2.SetRes(0.7);
    


    hw.StartAudio(callback);
    hw.StartAdc();

    // extern FontDef Font_16x26;

    while(1)
    {
        // now = dsy_system_getnow();

        // if(now - last_led_update > led_period)
        // {
        //     // this was ugly, but the only way I could find to avoid
        //     // getting yelled about defining note_names above

            
        //     // sprintf(strbuff, "Testing");

        //     hw.display.Fill(false);
        //     hw.display.SetCursor(0, 0);
        //     // hw.display.WriteString(strbuff, Font_16x26, true);
            
        //     // RedrawRectangles(kvals);
        //     char* knob_string = (char*)kvals;
        //     hw.display.WriteString(knob_string, Font_16x26, true);
        //     hw.display.Update();
        //     // hw.display.Update();
        // }
    }
}
