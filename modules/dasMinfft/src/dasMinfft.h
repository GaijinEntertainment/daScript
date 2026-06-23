#pragma once

#include "daScript/misc/platform.h"
#include "minfft.h"


namespace das
{
  DAS_MOD_API void fft_real_forward(const TArray<float> & real_signal, TArray<float2> & complex_frequencies, Context * context, LineInfoArg * at);
  DAS_MOD_API void fft_calculate_magnitudes(const TArray<float2> & complex_frequencies, TArray<float> & magnitudes, Context * context, LineInfoArg * at);
  DAS_MOD_API void fft_calculate_normalized_magnitudes(const TArray<float2> & complex_frequencies, TArray<float> & magnitudes, Context * context, LineInfoArg * at);
  DAS_MOD_API void fft_calculate_log_magnitudes(const TArray<float2> & complex_frequencies, TArray<float> & magnitudes, Context * context, LineInfoArg * at);
  DAS_MOD_API void fft_real_inverse(const TArray<float2> & complex_frequencies, TArray<float> & real_signal, Context * context, LineInfoArg * at);

  // Reusable plan (twiddle tables) for the DCT-II / DCT-III transform pair.
  // The same plan drives both dct (forward, DCT-II) and idct (inverse, DCT-III).
  // NOT thread-safe: minfft_aux holds a scratch buffer mutated during a transform,
  // so a single plan must not be used concurrently from multiple threads. Build one
  // plan per thread (or serialize access) if transforming in parallel.
  struct DctPlan {
    minfft_aux * aux = nullptr;
    int          n   = 0;   // total element count the plan was built for (n for 1d, rows*cols for 2d)
    DctPlan() = default;
    DctPlan(const DctPlan &) = delete;
    DctPlan & operator = (const DctPlan &) = delete;
    ~DctPlan();
  };

  DAS_MOD_API DctPlan * make_dct_plan_1d(int n);
  DAS_MOD_API DctPlan * make_dct_plan_2d(int rows, int cols);
  DAS_MOD_API void dct(const TArray<float> & input, TArray<float> & output, DctPlan * plan, Context * context, LineInfoArg * at);
  DAS_MOD_API void idct(const TArray<float> & input, TArray<float> & output, DctPlan * plan, Context * context, LineInfoArg * at);
}
