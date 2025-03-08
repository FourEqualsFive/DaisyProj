# FourEqualsFive DaisySeed Public Projects

I'm using this repo for my initial experimentation with the **DaisySeed**. I'm
mostly trying to fully understand the **libDaisy** and **DaisySP** libraries,
as well as the associated linking/loading structures. My method for exploring
these complex environments piece-by-piece is to create custom libraries within
the **DaisyExamples** environment, and then create programs that can run with
these custom libraries instead of the defaults from electro-smith. This will
enable me to eventually develop audio projects on a standalone **STM32F**, and
this way I get to make some noise while I sort through the firmware stuff.

At the same time, this is my introduction to many basic audio engineering
concepts, but I bring with me a fairly strong background in hardware design and
embedded C++, as well as my formal education in DSP concepts. Topics of interest
at present:
> - Dynamic compression
> - Digital audio filter design
> - Polyphonic synthesis
> - FFT and Z-Transform processing

### Current Programs
__Drum_Machine__ (*/seed/Drum_Machine/*): 
> - Based on *DaisyExamples/seed/Drum/*
> - Expanded to include more drum sounds and customization
> - Required more efficient sample/block processing
> - Required better compression

### CrLib

__Path__: */seed/CrLib/*

This is the custom library to replace DaisySP, currently includes the following
direct copies from the DaisySP library:
> - Control/adenv.cpp
> - Control/adenv.h
> - Control/adsr.cpp
> - Control/adsr.h
> - Drums/analogbassdrum.cpp
> - Drums/analogbassdrum.h
> - Drums/analogsnaredrum.cpp
> - Drums/analogsnaredrum.h
> - Filters/svf.cpp
> - Filters/svf.h
> - Noise/whitenoise.h
> - Synthesis/oscillator.cpp
> - Synthesis/oscillator.h
> - Utility/metro.cpp
> - Utility/metro.h
> - Utility/dsp.h

And also the following custom files:
> - __AdBass__: combines an Oscillator and 2x AdEnv (for frequency and amplitude)
>   - Drums/adbass.cpp
>   - Drums/adbass.h
> - __AdsrOsc__: combines an Oscillator and 2x Adsr (for frequency and amplitude)
>   - Drums/adsrosc.cpp
>   - Drums/adsrosc.h
> - __AdWNoise__: combines a WhiteNoise and an AdEnv (for amplitude)
>   - Drums/adwnoise.cpp
>   - Drums/adwnoise.h