# Software Front-end for MOLLER PMT gain measurements using a CAEN digitizer DAQ (QDC) with a CAEN fiber bridge setup  

## This was successfully compiled with

* Ubuntu 22.04.3 LTS
* 6.2.0-34-generic
* ROOT 6.28/06 with all required and recommended support programs
* The interface program EMMonitor uses the ROOT graphical user interface components (TG*) 

## Project Layout

* /lib - Previously written data display utility library
* /include
* /source

## Setup

* 'mkdir <some directory>'
* 'cd <some directory>'
* clone the repository
* 'cd lib'
* 'mkdir build'
* 'cd build'
* 'cmake ..'
* 'make'
* 'cd ../..'
* 'mkdir build'
* 'cd build'
* 'cmake ..'
* 'make'
* cp EMMonitor ..
* cd ..


## Running EMMonitor

* './EMMonitor -c SomeName.cnf' (an example config file is provided in the repository - refer to the CAEN manual for your digitizer for the needed settings)
* A config file can also be loaded from the config menu. No additional configuration step is necessary if the configuration file is specified on the command line.
* The program reads the configuration file and connects to the CAEN module
* In the menu, you can select start/stop to start and pause (stop) the data collection. All digitizer channels will be displayed (in two tabs)
* Events are accumulated continuously in the histograms 
* In the menu, you can clear the plots/histograms
* In the file menu, you can choose to have the collected histogram data for all channels written out (currently only in text file format) for further processing. 


## Hardware

This has been tested with the CAEN digitizers V792 and V965


