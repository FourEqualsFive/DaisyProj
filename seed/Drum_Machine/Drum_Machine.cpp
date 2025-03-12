#include "Drum_Machine.h"

void AudioCallback(AudioHandle::InterleavingInputBuffer  in,
                   AudioHandle::InterleavingOutputBuffer out,
                   size_t                                size)
{
    unsigned int deadSnare = 0;
    float freq_set, freqMod;
    float bassOsc_out[size], snareOsc_out[size];
    float clickOsc_out, bloopOsc_out;
    float noise_out, sig[size];

    //Get rid of any bouncing
    click.Debounce();
    tiss.Debounce();
    bass.Debounce();
    snare.Debounce();
    bloop.Debounce();

    //Trigger envelopes on button press
    if (click.RisingEdge()) { ClickTrig(); }
    if (tiss.RisingEdge()) { TissTrig(); }
    if (bloop.RisingEdge() || bloop.FallingEdge()) { BloopTrig(); }
    if (bass.RisingEdge()) { bssGate = true ; bassEnv.Trig(); }
    if (snare.RisingEdge()) { snrGate = true ; snareEnv.Trig(); }
    
    //Read the next ADC samples
    freqMod = (1 + hardware.adc.GetFloat(0));
    freq_set = base_f * freqMod;

    //Fill bassOsc_out with a block of samples
    bassEnv.Process(bassOsc_out, size);
    snareEnv.Process(snareOsc_out, size);

    //Prepare the rest of the audio block
    for(size_t i = 0; i < size; i += 2)
    {
        /*******************************************************************************
         * \todo Further optimize block processing
         * \todo Add gates and deadcounts back?
         * \todo Look into wavetable synthesis for 808 drums
        *******************************************************************************/
/*
        //Process the next samples
        //Use individual gate and deadcounts to skip processing when no output
        if (snrGate) {
            snareOsc_out = snareEnv.Process();
            (snareOsc_out > -0.1) && (snareOsc_out < 0.1) ? deadSnare++ : deadSnare = 0;
            (deadSnare > size) ? snrGate = false : snrGate = true;
        }
        else { snareOsc_out = 0; }

        if (bssGate) {
            bassOsc_out = 5 * bassEnv.Process();
            (bassOsc_out > -0.1) && (bassOsc_out < 0.1) ? deadBass++ : deadBass = 0;
            (deadBass > size) ? bssGate = false : bssGate = true;
        }
        else { bassOsc_out = 0; }
*/
        bloopOsc_out = bloopOsc.CallBack(freq_set, bloopFreqGate, bloopVolGate);
        clickOsc_out = .5 * clickOsc.CallBack(freq_set);
        noise_out = tissNoise.Callback();
        
        //Sum all voices, save the result for block processing
        sig[i] = noise_out + clickOsc_out + bloopOsc_out;
        sig[i] += bassOsc_out[i];
        sig[i] += snareOsc_out[i];

    } // end audio processing

    //Soft limit the oscillator samples to prevent clipping
    limit.ProcessBlock(sig,size,1);

    //Send sample block to output
    for(size_t i = 0; i < size; i += 2) {
        out[i]     = sig[i];
        out[i + 1] = sig[i];
    }

} // end AudioCallback()

int main(void)
{
    // Configure and Initialize the Daisy Seed
    hardware.Configure();
    hardware.Init();
    hardware.SetAudioBlockSize(48);
    float samplerate = hardware.AudioSampleRate();
    int blocksize = hardware.AudioBlockSize();
    
    //Initialize adc & start reading values
    adcConfig.InitSingle(hardware.GetPin(15));
    hardware.adc.Init(&adcConfig, 1);
    hardware.adc.Start();

    //Initialize limiter
    limit.Init();

    //Initialize drums
    clickOsc.Init(samplerate,base_f);
    bloopOsc.Init(samplerate,base_f);
    tissNoise.Init(samplerate);
    bassEnv.Init(samplerate);
    snareEnv.Init(samplerate);
    
    //Initialize buttons on pins 24 - 28
    //The callback rate is samplerate / blocksize (48)
    bloop.Init(hardware.GetPin(24), samplerate / blocksize);
    snare.Init(hardware.GetPin(25), samplerate / blocksize);
    bass.Init(hardware.GetPin(26), samplerate / blocksize);
    tiss.Init(hardware.GetPin(27), samplerate / blocksize);
    click.Init(hardware.GetPin(28), samplerate / blocksize);

    //Start calling the callback function
    hardware.StartAudio(AudioCallback);

    // Loop forever
    for(;;) {}
} // end main