#include "daisy_seed.h"
#include "daisysp.h"
#include "../CrLib/Source/crlib.h"

using namespace crlib;
using namespace daisysp;
using namespace daisy;

// Declare a DaisySeed object called hardware
DaisySeed hardware;

AdBass click;
AdWNoise tiss;

daisysp::Oscillator bloopOsc;

Adsr bloopPitchEnv, bloopVolEnv;
AnalogBassDrum bassEnv;
AnalogSnareDrum snareEnv;

AdcChannelConfig adcConfig;

Switch bloop, bass, snare;
Switch clickT, tissT;

bool bloopVolGate, bloopFreqGate;
bool led_state = true;
const int base_f = 600;

void ClickTrig(){
    click.TriggerEnv(AMPLITUDE);
    click.TriggerEnv(FREQUENCY);
}
void TissTrig(){
    tiss.TriggerEnv();
}