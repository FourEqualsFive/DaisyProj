#include "daisy_seed.h"
#include "../CrLib/Source/crlib.h"

using namespace crlib;
using namespace daisy;

// Declare a DaisySeed object called hardware
DaisySeed hardware;

AdBass clickOsc;
AdWNoise tissNoise;
AdsrOsc bloopOsc;

Metro tick;

cr808bass bassEnv;
cr808snare snareEnv;

Limiter limit;

AdcChannelConfig adcConfig;

CpuLoadMeter meter;

Switch bass, snare;
Switch click, tiss, bloop;

bool bloopVolGate, bloopFreqGate;
bool bssGate = false;
bool snrGate = false;
bool led_state = true;
const int base_f = 600;
float freq_set, freqMod;

void ClickTrig(){
    clickOsc.TriggerEnv();
    clickOsc.TriggerEnv();
}
void TissTrig(){
    tissNoise.TriggerEnv();
}
void BloopTrig(){
    bloopVolGate = !bloopVolGate;
    bloopFreqGate = !bloopFreqGate;
}