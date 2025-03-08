#include "daisy_seed.h"
#include "../CrLib/Source/crlib.h"

using namespace crlib;
using namespace daisy;

// Declare a DaisySeed object called hardware
DaisySeed hardware;

AdBass clickOsc;
AdWNoise tissNoise;
AdsrOsc bloopOsc;

AnalogBassDrum bassEnv;
AnalogSnareDrum snareEnv;

Limiter limit;

AdcChannelConfig adcConfig;

Switch bass, snare;
Switch click, tiss, bloop;

bool bloopVolGate, bloopFreqGate;
bool clkGate = false, blpGate = false;
bool tssGate = false, bssGate = false;
bool snrGate = false;
bool led_state = true;
const int base_f = 600;

void ClickTrig(){
    clkGate = true;
    clickOsc.TriggerEnv(AD_AMPLITUDE);
    clickOsc.TriggerEnv(AD_FREQUENCY);
}
void TissTrig(){
    tssGate = true;
    tissNoise.TriggerEnv();
}
void BloopTrig(){
    blpGate = true;
    bloopVolGate = !bloopVolGate;
    bloopFreqGate = !bloopFreqGate;
}