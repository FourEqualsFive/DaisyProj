#include "Drum_Machine.h"

void AudioCallback(AudioHandle::InterleavingInputBuffer  in,
                   AudioHandle::InterleavingOutputBuffer out,
                   size_t                                size)
{
    int nOsc = 0;
    float freq_set, freqMod;
    float bassOsc_out, snareOsc_out = 0;
    float clickOsc_out;
    float bloopOsc_out, blp_env_out;
    float osc_out, noise_out, sig;

    //Get rid of any bouncing
    clickT.Debounce();
    tissT.Debounce();
    bass.Debounce();
    snare.Debounce();
    bloop.Debounce();

    if (clickT.RisingEdge()) { ClickTrig(); }
    if (tissT.RisingEdge()) { TissTrig(); }

    if(bloop.RisingEdge())    {
        bloopVolGate = true;
        bloopFreqGate = true;
    }
    if(bloop.FallingEdge())    {
        bloopVolGate = false;
        bloopFreqGate = false;
    }
    if(bass.RisingEdge())    {
        bassEnv.Trig();
    }
    if(snare.RisingEdge())    {
        snareEnv.Trig();
    }

    //Prepare the audio block
    for(size_t i = 0; i < size; i += 2)
    {
        /*******************************************************************
         * \todo Make some external functions to clean up this loop [WIP]
         * \todo Only call .Process() when an envelope is triggered
         * \todo Full switch to processing by frame (instead of by sample)
        *******************************************************************/

        //Get the next volume samples
        blp_env_out = bloopVolEnv.Process(bloopVolGate);

        freqMod = (1 + hardware.adc.GetFloat(0));
        freq_set = base_f * freqMod;
        
        //Set the bloop volume to the envelope's output
        bloopOsc.SetAmp(blp_env_out);
        //Apply the pitch envelope to the bloop
        bloopOsc.SetFreq(freq_set * bloopPitchEnv.Process(bloopFreqGate));
        //Process the next oscillator sample
        bloopOsc_out = bloopOsc.Process();
        (bloopOsc_out != 0) ? nOsc++ : 1 ;
        //TODO: Bloop osc envelope lasts way too long... detect when freq < audible

        //Process the next bass sample
        bassOsc_out = bassEnv.Process();
        (bassOsc_out != 0) ? nOsc++ : 1 ;
        
        clickOsc_out = click.CallBack(freq_set);
        (clickOsc_out != 0) ? nOsc++ : 1 ;

        noise_out = tiss.Callback();

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
    // These are separate to allow reconfiguration of any of the internal
    // components before initialization.
    hardware.Configure();
    hardware.Init();
    hardware.SetAudioBlockSize(4);
    float samplerate = hardware.AudioSampleRate();
    
    //Initialize the adc
    adcConfig.InitSingle(hardware.GetPin(15));
    hardware.adc.Init(&adcConfig, 1);
    //Start reading values
    hardware.adc.Start();

    //Initialize the click drum
    click.Init(samplerate,base_f);
    tiss.Init(samplerate);

    //Initialize oscillator for bloop drum
    bloopOsc.Init(samplerate);
    bloopOsc.SetWaveform(daisysp::Oscillator::WAVE_TRI);
    bloopOsc.SetFreq(base_f);
    bloopOsc.SetAmp(1);

    //Initialize the bass and snare drums
    bassEnv.Init(samplerate);
    snareEnv.Init(samplerate);

    //Initialize envelopes
    //Bloop oscillator pitch envelope
    bloopPitchEnv.Init(samplerate);
    bloopPitchEnv.SetTime(ADSR_SEG_ATTACK, .01);
    bloopPitchEnv.SetTime(ADSR_SEG_DECAY, .05);
    bloopPitchEnv.SetTime(ADSR_SEG_RELEASE, .5);
    bloopPitchEnv.SetSustainLevel(0.5);

    //Bloop oscillator volume envelope
    bloopVolEnv.Init(samplerate);
    bloopVolEnv.SetTime(ADSR_SEG_ATTACK, .01);
    bloopVolEnv.SetTime(ADSR_SEG_DECAY, 1);
    bloopVolEnv.SetTime(ADSR_SEG_RELEASE, 1);
    bloopVolEnv.SetSustainLevel(0.7);
    
    //Initialize buttons on pins 24 - 28
    //The callback rate is samplerate / blocksize (48)
    bloop.Init(hardware.GetPin(24), samplerate / 48.f);
    snare.Init(hardware.GetPin(25), samplerate / 48.f);
    bass.Init(hardware.GetPin(26), samplerate / 48.f);
    tissT.Init(hardware.GetPin(27), samplerate / 48.f);
    clickT.Init(hardware.GetPin(28), samplerate / 48.f);

    //Start calling the callback function
    hardware.StartAudio(AudioCallback);

    // Loop forever
    for(;;) {}
} // end main