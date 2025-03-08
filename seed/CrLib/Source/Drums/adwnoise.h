
#pragma once
#ifndef CR_ADWNOISE_H
#define CR_ADWNOISE_H
#include <stdint.h>
#include "../Noise/whitenoise.h"
#include "../Control/adenv.h"

#ifdef __cplusplus

namespace crlib{
    
class AdWNoise : public WhiteNoise{
    public:
        AdWNoise() {}
        ~AdWNoise() {}

        void Init(float sample_rate);
        /*********************************************************************
            Parameters:
            \param  sample_rate     sample rate of the audio engine being run

            Description: Initialize the Whitenoise and AdEnv
        *********************************************************************/

        float Callback();
        /*********************************************************************
            Description: Handles processing for individual envelopes
        *********************************************************************/

        void TriggerEnv();
        /*********************************************************************
            Description:    Calls Trigger() for the AdEnv
        *********************************************************************/

    private:
        AdEnv env_;
};

} // end namespace crlib
#endif
#endif