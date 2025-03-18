
#pragma once
#ifndef CR_ADCLAP_H
#define CR_ADCLAP_H
#include <stdint.h>
#include "../Noise/whitenoise.h"
#include "../Utility/dsp.h"
#include "../Control/adenv.h"
#include "../Filters/onepole.h"
#include "../Filters/soap.h"

#ifdef __cplusplus

namespace crlib{
    
class AdClap : public WhiteNoise{
    public:
        AdClap() {}
        ~AdClap() {}

        void Init(float sample_rate);
        /*********************************************************************
            Parameters:
            \param  sample_rate     sample rate of the audio engine being run

            Description: Initialize the Whitenoise and AdEnv
        *********************************************************************/

        void CallBack(float *sig, size_t size);
        /*********************************************************************
            Description: Handles processing for individual envelopes
        *********************************************************************/

        void TriggerEnv();
        /*********************************************************************
            Description:    Calls Trigger() for the AdEnv
        *********************************************************************/

    private:
        AdEnv env_;
        OnePole lp1_, lp2_;
        float gate_;
};

} // end namespace crlib
#endif
#endif