Partitioned FFT convolution reverb with synthetic impulse response. Uses overlap-save convolution with a frequency-domain delay line for efficient real-time processing.
decayTime: Reverb decay time in seconds (time to reach -60dB).
lpFreqStart: Lowpass filter starting frequency in Hz.
lpFreqEnd: Lowpass filter ending frequency at -60dB in Hz.
fadeIn: Impulse response fade-in time in seconds.
sampleRate: Audio sample rate in Hz.
fft_size: FFT size used for partitioned convolution (2 x block size).
ir_length: Impulse response length in samples.
num_partitions: Number of IR partitions (ir_length / block_size, rounded up).
