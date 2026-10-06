options gen2
options indenting = 4

require math


def celsius_to_fahrenheit(c : float) : float => c * 1.8 + 32.0

def clamp_int(v, lo, hi : int) : int => clamp(v, lo, hi)

def private rounding_bias() : float => 0.5
