#include "daisy_field.h"
#include "daisysp.h" // Uncomment this if you want to use the DSP library.

using namespace daisysp;
using namespace daisy;


// Declare a local daisy_field for hardware access
DaisyField        hw;
static Oscillator osc;

float kvals[8];
// This runs at a fixed rate, to prepare audio samples
static void callback(float *in, float *out, size_t size)
{
    hw.ProcessAnalogControls();
    hw.UpdateDigitalControls();

    for(int i = 0; i < 8; i++)
    {
        kvals[i] = hw.GetKnobValue(i);        
    }

    float output;
    for(size_t i = 0; i < size; i += 2)
    {
        output = osc.Process();
        osc.SetFreq(hw.GetKnobValue(2) * 1000);

        // left out
        out[i] = output;

        // right out
        out[i + 1] = output;
    }
}

void UpdateLeds(float *knob_vals)
{
    // knob_vals is exactly 8 members
    size_t knob_leds[] = {
        DaisyField::LED_KNOB_1,
        DaisyField::LED_KNOB_2,
        DaisyField::LED_KNOB_3,
        DaisyField::LED_KNOB_4,
        DaisyField::LED_KNOB_5,
        DaisyField::LED_KNOB_6,
        DaisyField::LED_KNOB_7,
        DaisyField::LED_KNOB_8,
    };
    size_t keyboard_leds[] = {
        DaisyField::LED_KEY_A1,
        DaisyField::LED_KEY_A2,
        DaisyField::LED_KEY_A3,
        DaisyField::LED_KEY_A4,
        DaisyField::LED_KEY_A5,
        DaisyField::LED_KEY_A6,
        DaisyField::LED_KEY_A7,
        DaisyField::LED_KEY_A8,
        DaisyField::LED_KEY_B2,
        DaisyField::LED_KEY_B3,
        DaisyField::LED_KEY_B5,
        DaisyField::LED_KEY_B6,
        DaisyField::LED_KEY_B7,
    };
    for(size_t i = 0; i < 8; i++)
    {
        hw.led_driver_.SetLed(knob_leds[i], knob_vals[i]);
    }
    for(size_t i = 0; i < 13; i++)
    {
        hw.led_driver_.SetLed(keyboard_leds[i], 1.f);
    }
    hw.led_driver_.SwapBuffersAndTransmit();
}

int main(void)
{
    float sample_rate;    
    
     
    hw.Init();
    sample_rate = hw.SampleRate();

    osc.Init(sample_rate);

    // Set parameters for oscillator
    osc.SetWaveform(osc.WAVE_SIN);
    osc.SetFreq(100);        
    osc.SetAmp(0.5);


    hw.StartAudio(callback);
    hw.StartAdc();


    while(1)
    {
        UpdateLeds(kvals);
        dsy_system_delay(1);        
        dsy_dac_write(DSY_DAC_CHN1, hw.GetKnobValue(0) * 4095);
        dsy_dac_write(DSY_DAC_CHN2, hw.GetKnobValue(1) * 4095);
    }
}
