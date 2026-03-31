// CIPIC HRTF Library - Subject 021 (KEMAR large pinnae)
// Public Domain (UC Davis CIPIC database)
#ifndef CIPIC_HRTF_LIB_H
#define CIPIC_HRTF_LIB_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CIPIC_HRTF_N_AZI 25
#define CIPIC_HRTF_N_ELE 50

// CIPIC interaural-polar azimuths (degrees)
static const float cipic_azimuths[25] = {
  -80.0f, -65.0f, -55.0f, -45.0f, -40.0f, -35.0f, -30.0f, -25.0f, -20.0f, -15.0f, -10.0f, -5.0f, 0.0f, 5.0f, 10.0f, 15.0f, 20.0f, 25.0f, 30.0f, 35.0f, 40.0f, 45.0f, 55.0f, 65.0f, 80.0f
};

// CIPIC interaural-polar elevations (degrees, 0=front, 90=above, 180=behind)
static const float cipic_elevations[50] = {
  -45.0000f, -39.3750f, -33.7500f, -28.1250f, -22.5000f, -16.8750f, -11.2500f, -5.6250f, 0.0000f, 5.6250f,
  11.2500f, 16.8750f, 22.5000f, 28.1250f, 33.7500f, 39.3750f, 45.0000f, 50.6250f, 56.2500f, 61.8750f,
  67.5000f, 73.1250f, 78.7500f, 84.3750f, 90.0000f, 95.6250f, 101.2500f, 106.8750f, 112.5000f, 118.1250f,
  123.7500f, 129.3750f, 135.0000f, 140.6250f, 146.2500f, 151.8750f, 157.5000f, 163.1250f, 168.7500f, 174.3750f,
  180.0000f, 185.6250f, 191.2500f, 196.8750f, 202.5000f, 208.1250f, 213.7500f, 219.3750f, 225.0000f, 230.6250f
};

// API: same names as mit_hrtf_lib for drop-in compatibility
unsigned int mit_hrtf_availability(int azimuth, int elevation, unsigned int samplerate, unsigned int diffused);
unsigned int mit_hrtf_get(int* pAzimuth, int* pElevation, unsigned int samplerate, unsigned int diffused, short* pLeft, short* pRight);

#ifdef __cplusplus
}
#endif

#endif // CIPIC_HRTF_LIB_H
