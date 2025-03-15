#include "Drum_Machine.h"

void AudioCallback(AudioHandle::InterleavingInputBuffer  in,
                   AudioHandle::InterleavingOutputBuffer out,
                   size_t                                size)
{
    float bassOsc_out[size], snareOsc_out[size];
    float clickOsc_out[size], bloopOsc_out[size];
    float noise_out[size], sig[size];

    //meter.OnBlockStart(); //for debug

    //Fill each individual voice buffer with a block of samples
    bassEnv.Process(bassOsc_out, size);
    snareEnv.Process(snareOsc_out, size);
    clickOsc.CallBack(clickOsc_out, size, freq_set);
    bloopOsc.CallBack(bloopOsc_out, size, freq_set, bloopFreqGate, bloopVolGate);
    tissNoise.CallBack(noise_out, size);

    //Sum individual voice samples in one buffer
    for(size_t i = 0; i < size; i += 2)
    {
        sig[i] = noise_out[i];
        sig[i] += bloopOsc_out[i];
        sig[i] += clickOsc_out[i];
        sig[i] += bassOsc_out[i];
        sig[i] += snareOsc_out[i];
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
    for(;;) {
        //Measure average and max CPU load (for debug)
        //float maxCpu = 100 * meter.GetMaxCpuLoad();
        //float avgCpu = 100 * meter.GetAvgCpuLoad();

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
    /*
        //Trigger envelopes on metronome (for debug)
        if (tick.Process()) { meter.Reset(); ClickTrig(); }
        if (tick.Process()) { meter.Reset(); TissTrig(); }
        if (tick.Process() || bloop.FallingEdge()) { meter.Reset(); BloopTrig(); }
        if (tick.Process()) { meter.Reset(); bssGate = true ; bassEnv.Trig(); }
        if (tick.Process()) { meter.Reset(); snrGate = true ; snareEnv.Trig(); }
    */
        //Read the next ADC samples
        freqMod = (1 + hardware.adc.GetFloat(0));
        freq_set = base_f * freqMod;
    }
    
} // end main