#include "adbass.h"

using namespace crlib;

void AdBass::Init(float sample_rate, int base_f = 600){
    base_f_ = base_f;
    gate_ = 0;

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

void AdBass::TriggerEnv(){
    float freq_gate, amp_gate;
    freq_gate = freq_env_.GetEnvTime();
    amp_gate = amp_env_.GetEnvTime();
    (freq_gate > amp_gate) ? (gate_ = freq_gate) : (gate_ = amp_gate);
    amp_env_.Trigger(); 
    freq_env_.Trigger();
} // end TiggerEnv()

void AdBass::CallBack(float *sig, size_t size, float freq_set){

    if (gate_--){
        freq_env_.SetMax(freq_set);
        freq_env_.SetMin((freq_set) - 550);
        for (size_t i = 0; i < size; i += 2)
        {
            SetAmp(amp_env_.Process());
            SetFreq(freq_env_.Process());
            *(sig + i) = Process();
        }
    }
    else { 
        for (size_t i = 0; i < size; i += 2)
        {
            *(sig + i) = 0; 
        }
    }

} // end CallBack()