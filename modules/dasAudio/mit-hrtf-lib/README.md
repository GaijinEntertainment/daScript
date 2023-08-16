mit-hrtf-lib - MIT Kemar HRTF C Library
============

## What is this?

A library making access to the MIT Kemar HRTF set easy, through the use of two functions.

## Tell me some more

This is a C library making access to the MIT Kemar HRTF set easy, through the use of two functions. It is based on the "compact" data sets, which are trimmed to 128 taps at 44100Hz, from the the complete set of 512 taps. While Aristotel Digenis wrote this library, all the data itself is thanks to the work done at MIT by Bill Gardner and Keith Martin. All this library aims to do is to make the use of that data simpler.

The original data available from the MIT website is stored in individual ".wav" files, at 44100Hz sample rate. For creation of this library, resampled version were made. At this point there were 2944 files, so a Python script was then written to iterate through all the files, sorting them and creating multi-dimensional C arrays out of them. Next, the mechanism for returning the appropriate array elements was written and tested.

## How do I use it?

There are only two functions for use. Have a look at the example code below, or see mit_hrtf_lib.h for more information on their use.

```c
int azimuth = 30;
int elevation = 0;

// Are there filters available for these options and how long is it?
unsigned int numOfTaps = mit_hrtf_availability(azimuth, elevation, 44100, 0);
if (numOfTaps)
{
    // Allocate buffer to copy filters onto to
    short* leftEar = (short*)malloc(numOfTaps * sizeof(short));
    short* rightEar = (short*)malloc(numOfTaps * sizeof(short));

    // Copy filters to new buffers 
    numOfTaps = mit_hrtf_get(&azimuth, &elevation, 44100, 0, leftEar, rightEar);
	
    // Do as you wish with filters...
    // Remember to free up buffers
    free(leftEar);
    free(rightEar);
}
```

## Details
Licence: MIT Licence  
Sample Rates: 44100, 48000, 88200, 96000  
Bit Depth: 16  
Data Sets: Normal, Diffused  
