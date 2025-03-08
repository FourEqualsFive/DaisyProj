#include "adsrosc.h"

using namespace crlib;

void AdsrOsc::Init(float sample_rate, int base_f = 600){
    base_f_ = base_f;

    Oscillator::Init(sample_rate);
    amp_env_.Init(sample_rate);
    freq_env_.Init(sample_rate);

    SetWaveform(Oscillator::WAVE_TRI);
    SetFreq(base_f_);
    SetAmp(1);

    freq_env_.SetTime(ADSR_SEG_ATTACK, .01);
    freq_env_.SetTime(ADSR_SEG_DECAY, .05);
    freq_env_.SetTime(ADSR_SEG_RELEASE, .5);
    freq_env_.SetSustainLevel(0.5);

    amp_env_.SetTime(ADSR_SEG_ATTACK, .01);
    amp_env_.SetTime(ADSR_SEG_DECAY, 1);
    amp_env_.SetTime(ADSR_SEG_RELEASE, 1);
    amp_env_.SetSustainLevel(0.7);
}// end Init()

float AdsrOsc::CallBack(float freq_set, bool freq_gate, bool amp_gate){

    SetAmp(amp_env_.Process(amp_gate));
    SetFreq(freq_set * freq_env_.Process(freq_gate));

    return Process();

} // end CallBack()