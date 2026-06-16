Quality / cost tier for the convolution reverb, chosen at conv_reverb_init.
High: two decorrelated impulse responses, one full partitioned convolution per channel — widest stereo image, most expensive.
Medium: a single mono impulse response convolved once, split into stereo by two Schroeder allpass filters — about half the per-block cost, narrower stereo image.
Low: a Freeverb-style algorithmic reverb (8 damped comb + 4 allpass filters per channel) — no FFT, by far the cheapest tier, with a less natural / more metallic tail.
