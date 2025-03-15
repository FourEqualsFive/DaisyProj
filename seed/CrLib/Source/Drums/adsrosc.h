
#pragma once
#ifndef CR_ADSROSC_H
#define CR_ADSROSC_H
#include <stdint.h>
#include "../Synthesis/oscillator.h"
#include "../Control/adsr.h"

#ifdef __cplusplus

namespace crlib{
    
enum AdsrOscEnvelope{ ADSR_FREQUENCY, ADSR_AMPLITUDE };

class AdsrOsc : public Oscillator{
    public:
        AdsrOsc() {}
        ~AdsrOsc() {}

        void Init(float sample_rate, int base_f);
        /*********************************************************************
            Parameters:
            \param sample_rate  sample rate of the audio engine being run
            \param base_f       the max frequency to be reached during Attack

            Description:    Initializes the Oscillator and both AD Envelopes

            Defaults:
            - base_f_ = base_f = 600
            - Oscillator
                *waveform_ = WAVE_TRI
                *amp_ = 1
                *other properties follow base class Init()
            - freq_env_
                *segment_time_[ADENV_SEG_ATTACK] = 0.01
                *segment_time_[ADENV_SEG_DECAY] = 0.05
                *max = base_f
                *min = 50
                *other properties follow base class Init()
            - amp_env_
                *segment_time_[ADENV_SEG_ATTACK] = 0.01
                *segment_time_[ADENV_SEG_DECAY] = 1 
                *max = 1
                *min = 50
                *other properties follow base class Init()
            - 
        *********************************************************************/

        void CallBack(float *sig, size_t size, float freq_set, bool freq_gate, bool amp_gate);
        /*********************************************************************
            Parameters:
            \param sig          pointer to audio sample block buffer
            \param size         number of samples for one audio block
            \param freq_set     base frequency modified by pot input
            \param freq_gate    true allows the pitch ADSR to process samples
            \param amp_gate     true allows the volume ADSR to process samples

            Description:    Handles processing for two separate ADSR envelopes
        *********************************************************************/

    private:
        Adsr amp_env_, freq_env_;
        int base_f_;
        bool gate_;
        float count_;
};

} // end namespace crlib
#endif
#endif