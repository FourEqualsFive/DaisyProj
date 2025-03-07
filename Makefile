# Project Name
TARGET = Drum_Machine

# Sources
CPP_SOURCES = Drum_Machine.cpp

# Library Locations
LIBDAISY_DIR = /libDaisy/
DAISYSP_DIR = /DaisySP/
CRLIB_DIR = /seed/CrLib/

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile
