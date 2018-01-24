# ssp
A collection of sound and midi applications

This repository is a repository for various sound and art projects. The main projects are:

* Soundspace: Control application for sound art installations (input, output, sensor, logic, etc.)
* PressureMagi: Control application for a specific art installation with MIDI-controlled water pumps
* MidiMagi: Utility application for generating MIDI files to PressureMagi
* SoundServerAPI: API for a soundserver in an Augmented reality project

In addition there are a few support libraries (MS Wimdows based):

* sspUtility: Timers events, synchronization
* sspMidiLib: Midi handling, including Midi File read/write (MME based)
* sspAudioLib: Reading audio files using libsndfile (wrapper)

The projects are found under \source. The solution and project files are found under \source\<project name>\build\<target platform>.
