#include "adwnoise.h"

using namespace crlib;

void AdWNoise::Init(float sample_rate){
    gate_ = 0;
    WhiteNoise::Init();
    env_.Init(sample_rate);
    env_.SetTime(ADENV_SEG_ATTACK, .01);
    env_.SetTime(ADENV_SEG_DECAY, .2);
    env_.SetMax(0.7);
    env_.SetMin(0);
}

void AdWNoise::TriggerEnv(){
    gate_ = env_.GetEnvTime();
    env_.Trigger();
}

float AdWNoise::Callback(){
    if (gate_){ 
        gate_--;
        return ( WhiteNoise::Process() * env_.Process() );
    }
    else {return 0;}
}