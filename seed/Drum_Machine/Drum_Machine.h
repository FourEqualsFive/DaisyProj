#include "daisy_seed.h"
#include "../CrLib/Source/crlib.h"

using namespace crlib;
using namespace daisy;

// Declare a DaisySeed object called hardware
DaisySeed hardware;

AdBass triOsc;
AdBass sqOsc, popOsc;
AdWNoise tissNoise;
AdClap clapNoise;
AdsrOsc bloopOsc;

Metro tick;

HiHat<> hiHatEnv;
crBell bellEnv;
cr808bass kickEnv;
cr808snare snareEnv;

Limiter limit;

AdcChannelConfig adcConfig;

CpuLoadMeter meter;

Switch kick, snare, hihat;
Switch tri, tiss, bloop;
Switch square, pop, bell, clap;

bool bloopVolGate, bloopFreqGate;
bool kckGate = false;
bool snrGate = false;
bool hatGate = false;
bool belGate = false;
bool led_state = true;
const int base_f = 600;
float tri_freq;
int deadHat = 0;

void TriTrig(){
    triOsc.TriggerEnv();
}
void PopTrig(){
    popOsc.TriggerEnv();
}
void SqTrig(){
    sqOsc.TriggerEnv();
}
void ClapTrig(){
    clapNoise.TriggerEnv();
}
void TissTrig(){
    tissNoise.TriggerEnv();
}
void BloopTrig(){
    bloopVolGate = !bloopVolGate;
    bloopFreqGate = !bloopFreqGate;
}