#include "adsrosc.h"

using namespace crlib;

void AdsrOsc::Init(float sample_rate, int base_f = 600){
    base_f_ = base_f;
    gate_ = false;
    count_ = 0;

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
    float amp_release, freq_release;
    if ((!freq_gate || !amp_gate) & gate_){ //falling edge
        amp_release = amp_env_.GetReleaseTime();
        freq_release = freq_env_.GetReleaseTime();
        (amp_release > freq_release) ? (count_ = amp_release) : (count_ = freq_release);
    }
    else
    {
        count_ = 1;
    }

    if (count_){
        SetAmp(amp_env_.Process(amp_gate));
        SetFreq(freq_set * freq_env_.Process(freq_gate));

        count_--;

        return Process();
    }
    else { return 0; }

} // end CallBack()