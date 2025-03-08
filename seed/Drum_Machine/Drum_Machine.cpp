#include "Drum_Machine.h"

void AudioCallback(AudioHandle::InterleavingInputBuffer  in,
                   AudioHandle::InterleavingOutputBuffer out,
                   size_t                                size)
{
    int nOsc = 0;
    int deadClick = 0, deadBloop = 0, deadTiss = 0;
    int deadSnare = 0, deadBass = 0;
    float freq_set, freqMod;
    float bassOsc_out, snareOsc_out = 0;
    float clickOsc_out, bloopOsc_out;
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
    if (bass.RisingEdge()) { bssGate = true ; bassEnv.Trig(); }
    if (snare.RisingEdge()) { snrGate = true ; snareEnv.Trig(); }

    //Prepare the audio block
    for(size_t i = 0; i < size; i += 2)
    {
        /*******************************************************************
         * \todo Only call .Process() when an envelope is triggered
         * \todo Process in larger blocks, test latency tradeoff
        *******************************************************************/

        freqMod = (1 + hardware.adc.GetFloat(0));
        freq_set = base_f * freqMod;

        //Process the next samples
        //Increment oscillator count for active processes
        //Use individual gate and deadcounts to skip processing when no output
        if (snrGate) {
            snareOsc_out = snareEnv.Process();
            (snareOsc_out != 0) ? nOsc++ : 1 ;
            (snareOsc_out > -0.1) && (snareOsc_out < 0.1) ? deadSnare++ : deadSnare = 0;
            (deadSnare > 4) ? snrGate = false : snrGate = true;
        }
        else { snareOsc_out = 0; }

        if (bssGate) {
            bassOsc_out = bassEnv.Process();
            (bassOsc_out != 0) ? nOsc++ : 1 ;
            (bassOsc_out > -0.1) && (bassOsc_out < 0.1) ? deadBass++ : deadBass = 0;
            (deadBass > 4) ? bssGate = false : bssGate = true;
        }
        else { bassOsc_out = 0; }

        if (blpGate) {
            bloopOsc_out = bloopOsc.CallBack(freq_set, bloopFreqGate, bloopVolGate);
            (bloopOsc_out != 0) ? nOsc++ : 1 ;
            (bloopOsc_out > -0.1) && (bloopOsc_out < 0.1) ? deadBloop++ : deadBloop = 0;
            (deadBloop > 4) ? blpGate = false : blpGate = true;
        }
        else { bloopOsc_out = 0; }

        if (clkGate) {
            clickOsc_out = clickOsc.CallBack(freq_set);
            (clickOsc_out != 0) ? nOsc++ : 1 ;
            (clickOsc_out > -0.1) && (clickOsc_out < 0.1) ? deadClick++ : deadClick = 0;
            (deadClick > 4) ? clkGate = false : clkGate = true;
        }
        else { clickOsc_out = 0; }


        if (tssGate) {
            noise_out = tissNoise.Callback();
            (noise_out > -0.1) && (noise_out < 0.1) ? deadTiss++ : deadTiss = 0;
            (deadTiss > 4) ? tssGate = false : tssGate = true;
        }
        else { noise_out = 0; }

        //Dynamically compress the oscillator samples to prevent clipping
        /*********************************************************
         *\todo add proper compression instead of this nonsense
        *********************************************************/
        if (nOsc == 0){ osc_out = 0; }
        else { osc_out = (clickOsc_out + bassOsc_out + snareOsc_out + bloopOsc_out) / nOsc; }
        nOsc = 0;

        //Mix the signals
        sig = .3 * noise_out + .7 * osc_out;

        //Set the left and right outputs to the mixed signals
        out[i]     = sig;
        out[i + 1] = sig;

    } // end for loop
} // end AudioCallback()

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