cmake_minimum_required(VERSION 3.14)

project(CRLIB VERSION 0.0.1)

add_library(CrLib STATIC 
Source/Utility/metro.cpp
Source/Control/adenv.cpp
Source/Control/adsr.cpp
Source/Dynamics/limiter.cpp
Source/Filters/svf.cpp
Source/Synthesis/oscillator.cpp
Source/Drums/adbass.cpp
Source/Drums/adsrosc.cpp
Source/Drums/adwnoise.cpp
Source/Drums/analogbassdrum.cpp
Source/Drums/analogsnaredrum.cpp
)


set_target_properties(CrLib PROPERTIES PUBLIC
  CXX_STANDARD 14 
  CXX_STANDARD_REQUIRED
  )


target_include_directories(CrLib PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/Source
  PRIVATE
  "Source"
  "Source/Utility"
  "Source/Control"
  "Source/Dynamics"
  "Source/Filters"
  "Source/Noise"
  "Source/Synthesis"
  "Source/Drums"
  )
