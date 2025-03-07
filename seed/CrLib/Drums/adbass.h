
#pragma once
#ifndef ADBASS_H
#define ADBASS_H
#include <stdint.h>

#include "../../../libDaisy/src/daisy.h"
#include "../../../DaisySP/Source/daisysp.h"

#ifdef __cplusplus

namespace crlib{
using namespace daisy;
using namespace daisysp;
    
class AdBass : public Oscillator{
    public:
        AdBass() {}
        ~AdBass() {}

        void Init(float sample_rate, int base_f);
        /*********************************************************************
            \param sample_rate  sample rate of the audio engine being run
            \param base_f       the max frequency to be reached during Attack

            Initializes the Oscillator and both AD Envelopes
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

        void CallBack(float sample_rate, int pin_num);

    private:
        AdEnv amp_env_, freq_env_;
        Switch gpio_;
        int base_f_;
};

} //namespace crlib
#endif
#endif