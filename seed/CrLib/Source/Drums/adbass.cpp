#include "adbass.h"

using namespace crlib;

void AdBass::Init(float sample_rate, int base_f = 600){
    base_f_ = base_f;

    Oscillator::Init(sample_rate);
    amp_env_.Init(sample_rate);
    freq_env_.Init(sample_rate);

    SetWaveform(Oscillator::WAVE_TRI);
    SetAmp(1);

    freq_env_.SetTime(ADENV_SEG_ATTACK, .01);
    freq_env_.SetTime(ADENV_SEG_DECAY, .05);
    freq_env_.SetMax(base_f_);
    freq_env_.SetMin(50);

    amp_env_.SetTime(ADENV_SEG_ATTACK, .01);
    amp_env_.SetTime(ADENV_SEG_DECAY, 1);
    amp_env_.SetMax(1);
    amp_env_.SetMin(0);
}// end Init()

void AdBass::TriggerEnv(int env){
    if (env == AMPLITUDE){  amp_env_.Trigger();  }
    else { freq_env_.Trigger(); }
} // end TiggerEnv()

float AdBass::CallBack(float freq_set){

    freq_env_.SetMax(freq_set);
    freq_env_.SetMin((freq_set) - 550);

    SetAmp(amp_env_.Process());
    SetFreq(freq_env_.Process());

    return Process();

} // end CallBack()