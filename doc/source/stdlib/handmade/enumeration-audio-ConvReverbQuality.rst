Quality / cost tier for the convolution reverb, chosen at conv_reverb_init.
High: two decorrelated impulse responses, one full partitioned convolution per channel — widest stereo image, most expensive.
Medium: a single mono impulse response convolved once, split into stereo by a per-channel Schroeder allpass cascade (depth set by roomstages/decorr_stages, default 5) — about half the per-block cost.
Low: a Freeverb-style algorithmic reverb (8 damped comb + 4 allpass filters per channel) — no FFT, by far the cheapest tier, with a less natural / more metallic tail.
