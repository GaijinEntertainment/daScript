/*
 * Dagor Engine 4
 * Copyright (C) 2003-2019  Gaijin Entertainment Corp.  All rights reserved
 *
 * (for conditions of distribution and use, see EULA in "prog/eula.txt")
*/

#ifndef _DAGOR_PUBLIC_MATH_RANDOM_DAG_UINT_NOISE_H_
#define _DAGOR_PUBLIC_MATH_RANDOM_DAG_UINT_NOISE_H_
#pragma once

#if defined(__has_feature)
    #if __has_feature(address_sanitizer)
        #define DAGOR_NOISE_SUPPRESS_UB  __attribute__((no_sanitize("undefined")))
    #endif
#endif

#ifndef DAGOR_NOISE_SUPPRESS_UB
#define DAGOR_NOISE_SUPPRESS_UB
#endif


// Math for Game Programmers: Noise-Based RNG
// author: Squirrel Eiserloh
__forceinline DAGOR_NOISE_SUPPRESS_UB unsigned int uint32_hash(unsigned int val)
{
  constexpr unsigned int BIT_NOISE1 = 0x68E31DA4;
  constexpr unsigned int BIT_NOISE2 = 0xB5297A4D;
  constexpr unsigned int BIT_NOISE3 = 0x1B56C4E9;
  unsigned int mangled = val;
  mangled *= BIT_NOISE1;
  mangled ^= (mangled>>8);
  mangled += BIT_NOISE2;
  mangled ^= (mangled<<8);
  mangled *= BIT_NOISE3;
  mangled ^= (mangled>>8);
  return mangled;
}

__forceinline DAGOR_NOISE_SUPPRESS_UB unsigned int uint_noise1D(int position, unsigned int seed)
{
  constexpr unsigned int BIT_NOISE1 = 0x68E31DA4;
  constexpr unsigned int BIT_NOISE2 = 0xB5297A4D;
  constexpr unsigned int BIT_NOISE3 = 0x1B56C4E9;
  unsigned int mangled = position;
  mangled *= BIT_NOISE1;
  mangled += seed;
  mangled ^= (mangled>>8);
  mangled += BIT_NOISE2;
  mangled ^= (mangled<<8);
  mangled *= BIT_NOISE3;
  mangled ^= (mangled>>8);
  return mangled;
}

__forceinline DAGOR_NOISE_SUPPRESS_UB unsigned int uint_noise2D(int posX, int posY, unsigned int seed)
{
  constexpr int PRIME_NUMBER = 198491317;
  return uint_noise1D(posX + (PRIME_NUMBER*posY), seed);
}

__forceinline DAGOR_NOISE_SUPPRESS_UB unsigned int uint_noise3D(int posX, int posY, int posZ, unsigned int seed)
{
  constexpr int PRIME_NUMBER1 = 198491317;
  constexpr int PRIME_NUMBER2 = 6542989;
  return uint_noise1D(posX + (PRIME_NUMBER1*posY) + (PRIME_NUMBER2*posZ), seed);
}

#endif
