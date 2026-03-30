/*
 * CIPIC HRTF Library - Subject 021 (KEMAR large pinnae)
 * Public Domain (UC Davis CIPIC database)
 *
 * Drop-in replacement for mit_hrtf_lib with the same API.
 * Internally uses CIPIC interaural-polar coordinates;
 * the API accepts MIT-style head-centered coordinates and converts.
 *
 * MIT head-centered:
 *   azimuth: -180..+180 (negative=left, positive=right, 0=front)
 *   elevation: -40..+90 (0=horizontal, 90=above)
 *
 * CIPIC interaural-polar:
 *   azimuth: -80..+80 (lateral angle, negative=left, positive=right)
 *   elevation: -45..+230.625 (0=front, 90=above, 180=behind)
 */

#include "../include/cipic_hrtf_lib.h"
#include <math.h>
#include <string.h>

#ifndef HRTF_SAMPLE_RATE
#define HRTF_SAMPLE_RATE 48000
#endif

#if HRTF_SAMPLE_RATE == 44100
#include "cipic_hrtf_44100.h"
#define CIPIC_HRIR_L cipic_hrir_l_44100
#define CIPIC_HRIR_R cipic_hrir_r_44100
#define CIPIC_HRTF_TAPS CIPIC_HRTF_44100_TAPS
#elif HRTF_SAMPLE_RATE == 48000
#include "cipic_hrtf_48000.h"
#define CIPIC_HRIR_L cipic_hrir_l_48000
#define CIPIC_HRIR_R cipic_hrir_r_48000
#define CIPIC_HRTF_TAPS CIPIC_HRTF_48000_TAPS
#elif HRTF_SAMPLE_RATE == 88200
#include "cipic_hrtf_88200.h"
#define CIPIC_HRIR_L cipic_hrir_l_88200
#define CIPIC_HRIR_R cipic_hrir_r_88200
#define CIPIC_HRTF_TAPS CIPIC_HRTF_88200_TAPS
#elif HRTF_SAMPLE_RATE == 96000
#include "cipic_hrtf_96000.h"
#define CIPIC_HRIR_L cipic_hrir_l_96000
#define CIPIC_HRIR_R cipic_hrir_r_96000
#define CIPIC_HRTF_TAPS CIPIC_HRTF_96000_TAPS
#else
#error "Unsupported HRTF_SAMPLE_RATE (must be 44100, 48000, 88200, or 96000)"
#endif

/* CIPIC azimuth grid (25 positions) */
static const float s_azimuths[CIPIC_HRTF_N_AZI] = {
    -80, -65, -55, -45, -40, -35, -30, -25, -20, -15, -10, -5, 0,
      5,  10,  15,  20,  25,  30,  35,  40,  45,  55,  65, 80
};

/* CIPIC elevation grid (50 positions, 5.625 deg spacing from -45) */
static const float s_elevations[CIPIC_HRTF_N_ELE] = {
    -45.0000f, -39.3750f, -33.7500f, -28.1250f, -22.5000f, -16.8750f, -11.2500f, -5.6250f, 0.0000f, 5.6250f,
    11.2500f, 16.8750f, 22.5000f, 28.1250f, 33.7500f, 39.3750f, 45.0000f, 50.6250f, 56.2500f, 61.8750f,
    67.5000f, 73.1250f, 78.7500f, 84.3750f, 90.0000f, 95.6250f, 101.2500f, 106.8750f, 112.5000f, 118.1250f,
    123.7500f, 129.3750f, 135.0000f, 140.6250f, 146.2500f, 151.8750f, 157.5000f, 163.1250f, 168.7500f, 174.3750f,
    180.0000f, 185.6250f, 191.2500f, 196.8750f, 202.5000f, 208.1250f, 213.7500f, 219.3750f, 225.0000f, 230.6250f
};

static int find_nearest_index(const float* arr, int n, float val) {
    int best = 0;
    float best_dist = fabsf(val - arr[0]);
    for (int i = 1; i < n; i++) {
        float dist = fabsf(val - arr[i]);
        if (dist < best_dist) {
            best_dist = dist;
            best = i;
        }
    }
    return best;
}

/*
 * Convert MIT head-centered (azimuth, elevation) to CIPIC interaural-polar.
 *
 * Head-centered to interaural-polar conversion:
 *   theta_h = MIT azimuth (horizontal angle, deg)
 *   phi_h   = MIT elevation (vertical angle, deg)
 *
 *   CIPIC azimuth  = asin(cos(phi_h) * sin(theta_h))   [lateral angle]
 *   CIPIC elevation = atan2(sin(phi_h), cos(phi_h) * cos(theta_h))  [polar angle from front]
 */
static void headcentered_to_cipic(int mit_azi, int mit_ele, float* cipic_azi, float* cipic_ele) {
    float theta = mit_azi * 3.14159265358979f / 180.0f;
    float phi   = mit_ele * 3.14159265358979f / 180.0f;

    float cos_phi = cosf(phi);
    float sin_phi = sinf(phi);
    float cos_theta = cosf(theta);
    float sin_theta = sinf(theta);

    /* lateral angle (CIPIC azimuth) */
    float lateral = asinf(cos_phi * sin_theta);
    *cipic_azi = lateral * 180.0f / 3.14159265358979f;

    /* polar angle from front (CIPIC elevation) */
    float polar = atan2f(sin_phi, cos_phi * cos_theta);
    *cipic_ele = polar * 180.0f / 3.14159265358979f;

    /* atan2 returns -180..180, but CIPIC elevation is -45..230.625 */
    /* Wrap negative behind-angles to positive: -180..-45 -> 180..315 */
    if (*cipic_ele < -45.0f) {
        *cipic_ele += 360.0f;
    }
}


unsigned int mit_hrtf_availability(int azimuth, int elevation, unsigned int samplerate, unsigned int diffused) {
    (void)azimuth; (void)elevation; (void)diffused;
    if (samplerate == HRTF_SAMPLE_RATE) return CIPIC_HRTF_TAPS;
    return 0;
}


unsigned int mit_hrtf_get(int* pAzimuth, int* pElevation, unsigned int samplerate, unsigned int diffused, short* pLeft, short* pRight) {
    (void)diffused;
    if (samplerate != HRTF_SAMPLE_RATE) return 0;

    /* Convert MIT head-centered to CIPIC interaural-polar */
    float cipic_azi, cipic_ele;
    headcentered_to_cipic(*pAzimuth, *pElevation, &cipic_azi, &cipic_ele);

    /* Find nearest grid indices */
    int azi_idx = find_nearest_index(s_azimuths, CIPIC_HRTF_N_AZI, cipic_azi);
    int ele_idx = find_nearest_index(s_elevations, CIPIC_HRTF_N_ELE, cipic_ele);

    /* Clamp */
    if (azi_idx < 0) azi_idx = 0;
    if (azi_idx >= CIPIC_HRTF_N_AZI) azi_idx = CIPIC_HRTF_N_AZI - 1;
    if (ele_idx < 0) ele_idx = 0;
    if (ele_idx >= CIPIC_HRTF_N_ELE) ele_idx = CIPIC_HRTF_N_ELE - 1;

    /* Copy taps to output */
    memcpy(pLeft, CIPIC_HRIR_L[azi_idx][ele_idx], CIPIC_HRTF_TAPS * sizeof(short));
    memcpy(pRight, CIPIC_HRIR_R[azi_idx][ele_idx], CIPIC_HRTF_TAPS * sizeof(short));

    /* Update caller with actual snapped coordinates */
    *pAzimuth = (int)roundf(s_azimuths[azi_idx]);
    *pElevation = (int)roundf(s_elevations[ele_idx]);

    return CIPIC_HRTF_TAPS;
}
