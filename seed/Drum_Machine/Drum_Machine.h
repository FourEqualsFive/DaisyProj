#include "daisy_seed.h"
#include "daisysp.h"
#include "../CrLib/Source/crlib.h"

using namespace crlib;
using namespace daisysp;
using namespace daisy;

// Declare a DaisySeed object called hardware
DaisySeed hardware;

AdBass clickOsc;
AdWNoise tissNoise;
AdsrOsc bloopOsc;

AnalogBassDrum bassEnv;
AnalogSnareDrum snareEnv;

AdcChannelConfig adcConfig;

Switch bass, snare;
Switch click, tiss, bloop;

bool bloopVolGate, bloopFreqGate;
bool led_state = true;
const int base_f = 600;

void ClickTrig(){
    clickOsc.TriggerEnv(AD_AMPLITUDE);
    clickOsc.TriggerEnv(AD_FREQUENCY);
}
void TissTrig(){
    tissNoise.TriggerEnv();
}
void BloopTrig(){
    bloopVolGate = !bloopVolGate;
    bloopFreqGate = !bloopFreqGate;
}