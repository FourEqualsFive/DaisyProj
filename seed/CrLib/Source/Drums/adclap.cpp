#include "adclap.h"

using namespace crlib;

void AdClap::Init(float sample_rate){
    gate_ = 0;
    WhiteNoise::Init();
    lp1_.Init();
    lp1_.SetFrequency(.3f);
    lp2_.Init();
    lp2_.SetFrequency(.3f);
    env_.Init(sample_rate);
    env_.SetTime(ADENV_SEG_ATTACK, .01);
    env_.SetTime(ADENV_SEG_DECAY, .05);
    env_.SetMax(1.0);
    env_.SetMin(0);
}

void AdClap::TriggerEnv(){
    gate_ = env_.GetEnvTime();
    env_.Trigger();
}

void AdClap::CallBack(float *sig, size_t size){
    for (size_t i = 0; i < size; i += 2){        
        if (gate_--)
        { 
            *(sig + i) = env_.Process() * lp2_.Process(lp1_.Process(WhiteNoise::Process()));
            //*(sig + i) = ( WhiteNoise::Process() * env_.Process() );
        }
        else
        {
            *(sig + i) = 0;
        }
    }
}