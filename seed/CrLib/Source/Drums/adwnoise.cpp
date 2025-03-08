#include "adwnoise.h"

using namespace crlib;

void AdWNoise::Init(float sample_rate){
    WhiteNoise::Init();
    env_.Init(sample_rate);
    env_.SetTime(ADENV_SEG_ATTACK, .01);
    env_.SetTime(ADENV_SEG_DECAY, .2);
    env_.SetMax(0.7);
    env_.SetMin(0);
}

void AdWNoise::TriggerEnv(){
    env_.Trigger();
}

float AdWNoise::Callback(){
    return ( WhiteNoise::Process() * env_.Process() );
}