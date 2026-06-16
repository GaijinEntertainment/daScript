Quality / cost tier for the convolution reverb, chosen at conv_reverb_init.
High: two decorrelated impulse responses, one full partitioned convolution per channel — widest stereo image, most expensive.
Medium: a single mono impulse response convolved once, split into stereo by two Schroeder allpass filters — about half the per-block cost, narrower stereo image.
Low: reserved for a cheaper recirculating-delay reverb; currently maps to medium.
