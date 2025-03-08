#include "Drum_Machine.h"

void AudioCallback(AudioHandle::InterleavingInputBuffer  in,
                   AudioHandle::InterleavingOutputBuffer out,
                   size_t                                size)
{
    int nOsc = 0;
    float freq_set, freqMod;
    float bassOsc_out, snareOsc_out = 0;
    float clickOsc_out;
    float bloopOsc_out;
    float osc_out, noise_out, sig;

    //Get rid of any bouncing
    click.Debounce();
    tiss.Debounce();
    bass.Debounce();
    snare.Debounce();
    bloop.Debounce();

    if (click.RisingEdge()) { ClickTrig(); }
    if (tiss.RisingEdge()) { TissTrig(); }
    if (bloop.RisingEdge() || bloop.FallingEdge()) { BloopTrig(); }
    if(bass.RisingEdge()) { bassEnv.Trig(); }
    if(snare.RisingEdge()) { snareEnv.Trig(); }

    //Prepare the audio block
    for(size_t i = 0; i < size; i += 2)
    {
        /*******************************************************************
         * \todo Make some external functions to clean up this loop [WIP]
         * \todo Only call .Process() when an envelope is triggered
         * \todo Full switch to processing by frame (instead of by sample)
        *******************************************************************/

        freqMod = (1 + hardware.adc.GetFloat(0));
        freq_set = base_f * freqMod;

        //Process the next bass sample
        bassOsc_out = bassEnv.Process();
        (bassOsc_out != 0) ? nOsc++ : 1 ;
        
        bloopOsc_out = bloopOsc.CallBack(freq_set, bloopFreqGate, bloopVolGate);
        (bloopOsc_out != 0) ? nOsc++ : 1 ;
        clickOsc_out = clickOsc.CallBack(freq_set);
        (clickOsc_out != 0) ? nOsc++ : 1 ;

        noise_out = tissNoise.Callback();

        /*********************************************************
         *\todo add proper compression instead of this nonsense
        *********************************************************/
        if (nOsc == 0){ osc_out = 0; }
        else { osc_out = (clickOsc_out + bassOsc_out + snareOsc_out + bloopOsc_out) / nOsc; }
        nOsc = 0;
        
        //Mix the two signals
        sig = .3 * noise_out + .7 * osc_out;

        //Set the left and right outputs to the mixed signals
        out[i]     = sig;
        out[i + 1] = sig;
    }
}

int main(void)
{
    // Configure and Initialize the Daisy Seed
    hardware.Configure();
    hardware.Init();
    hardware.SetAudioBlockSize(4);
    float samplerate = hardware.AudioSampleRate();
    
    //Initialize adc & start reading values
    adcConfig.InitSingle(hardware.GetPin(15));
    hardware.adc.Init(&adcConfig, 1);
    hardware.adc.Start();

    //Initialize drums
    clickOsc.Init(samplerate,base_f);
    bloopOsc.Init(samplerate,base_f);
    tissNoise.Init(samplerate);
    bassEnv.Init(samplerate);
    snareEnv.Init(samplerate);
    
    //Initialize buttons on pins 24 - 28
    //The callback rate is samplerate / blocksize (48)
    bloop.Init(hardware.GetPin(24), samplerate / 48.f);
    snare.Init(hardware.GetPin(25), samplerate / 48.f);
    bass.Init(hardware.GetPin(26), samplerate / 48.f);
    tiss.Init(hardware.GetPin(27), samplerate / 48.f);
    click.Init(hardware.GetPin(28), samplerate / 48.f);

    //Start calling the callback function
    hardware.StartAudio(AudioCallback);

    // Loop forever
    for(;;) {}
} // end main