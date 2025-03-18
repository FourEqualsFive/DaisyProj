#include "Drum_Machine.h"

void AudioCallback(AudioHandle::InterleavingInputBuffer  in,
                   AudioHandle::InterleavingOutputBuffer out,
                   size_t                                size)
{
    float kickOsc_out[size], snareOsc_out[size];
    float triOsc_out[size], popOsc_out[size];
    float bellOsc_out[size], hiHat_out[size];
    float tissN_out[size], clapN_out[size], sig[size];

    //meter.OnBlockStart(); //for debug

    //Fill each individual voice buffer with a block of samples
    clapNoise.CallBack(clapN_out, size);
    kickEnv.Process(kickOsc_out, size);
    snareEnv.Process(snareOsc_out, size);
    triOsc.CallBack(triOsc_out, size, tri_freq);
    //hiHatEnv.Process(hiHat_out,size, hatGate);
    popOsc.CallBack(popOsc_out, size, base_f);
    bellEnv.Process(bellOsc_out, size);
    tissNoise.CallBack(tissN_out, size);

    //Sum individual voice samples in one buffer
    for(size_t i = 0; i < size; i += 2)
    {
        hiHat_out[i] = hiHatEnv.Process(hatGate);
        sig[i] = tissN_out[i] + bellOsc_out[i] + triOsc_out[i] + hiHat_out[i]
                + popOsc_out[i] + kickOsc_out[i]+ snareOsc_out[i] + clapN_out[i];
    }

    //Soft limit the summed samples to prevent clipping
    limit.ProcessBlock(sig,size,1);

    //Send a block of samples to the output
    for(size_t i = 0; i < size; i += 2) {
        out[i]     = sig[i];
        out[i + 1] = sig[i];
    }

    //meter.OnBlockEnd(); //for debug

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

    //Initialize CPU Load Meter (for debug)
    //meter.Init(samplerate, blocksize);

    //Initialize limiter
    limit.Init();

    //Initialize metronome (for debug)
    tick.Init(1, samplerate);

    //Initialize drums
    triOsc.Init(samplerate,base_f);
    popOsc.Init(samplerate,base_f);
    popOsc.SetWaveform(Oscillator::WAVE_SIN);
    hiHatEnv.Init(samplerate);
    bellEnv.Init(samplerate);
    clapNoise.Init(samplerate);
    tissNoise.Init(samplerate);
    kickEnv.Init(samplerate);
    snareEnv.Init(samplerate);
    
    //Initialize buttons on pins 24 - 28
    //The callback rate is samplerate / blocksize (48)
    clap.Init(hardware.GetPin(23), samplerate / blocksize);
    bell.Init(hardware.GetPin(24), samplerate / blocksize);
    snare.Init(hardware.GetPin(25), samplerate / blocksize);
    kick.Init(hardware.GetPin(26), samplerate / blocksize);
    hihat.Init(hardware.GetPin(27), samplerate / blocksize);
    tri.Init(hardware.GetPin(28), samplerate / blocksize);
    tiss.Init(hardware.GetPin(29), samplerate / blocksize);
    pop.Init(hardware.GetPin(30), samplerate / blocksize);

    //Start calling the callback function
    hardware.StartAudio(AudioCallback);

    // Loop forever
    for(;;) {
        //Measure average and max CPU load (for debug)
        //float maxCpu = 100 * meter.GetMaxCpuLoad();
        //float avgCpu = 100 * meter.GetAvgCpuLoad();

        //Get rid of any bouncing
        clap.Debounce();
        tri.Debounce();
        pop.Debounce();
        tiss.Debounce();
        kick.Debounce();
        snare.Debounce();
        hihat.Debounce();
        bell.Debounce();
    
        //Trigger envelopes on button press
        if (clap.RisingEdge()) { ClapTrig(); }
        if (tri.RisingEdge()) { TriTrig(); }
        if (pop.RisingEdge()) { PopTrig(); }
        if (tiss.RisingEdge()) { TissTrig(); }
        if (bell.RisingEdge()) { belGate = true ; bellEnv.Trig(); }
        if (kick.RisingEdge()) { kckGate = true ; kickEnv.Trig(); }
        if (hihat.RisingEdge()) { hatGate = true ; }
        if (snare.RisingEdge()) { snrGate = true ; snareEnv.Trig(); }
    /*
        //Trigger envelopes on metronome (for debug)
        if (tick.Process()) { meter.Reset(); TriTrig(); }
        if (tick.Process()) { meter.Reset(); PopTrig(); }
        if (tick.Process()) { meter.Reset(); SqTrig(); }
        if (tick.Process()) { meter.Reset(); TissTrig(); }
        if (tick.Process() || bloop.FallingEdge()) { meter.Reset(); BloopTrig(); }
        if (tick.Process()) { meter.Reset(); kckGate = true ; kickEnv.Trig(); }
        if (tick.Process()) { meter.Reset(); snrGate = true ; snareEnv.Trig(); }
    */
        //Read the next ADC samples
        tri_freq = base_f * (1 + hardware.adc.GetFloat(0));
    }
    
} // end main