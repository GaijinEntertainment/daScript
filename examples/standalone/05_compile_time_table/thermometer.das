options gen2
options stack = 16384

require csv_bake

// Tier A + compile-time work. The board reads a thermistor through an ADC and
// wants a temperature. The calibration lives in thermistor.csv - thirteen bench
// measurements, unevenly spaced in ADC counts, in no particular order.
//
// None of that reaches the device. While THIS file compiles, csv_bake reads the
// file, sorts it, inverts the curve and resamples it onto an even grid; what
// ships is the grid. The program below has no CSV text in it, no parser, no
// search and no interpolation of the raw points - just an index and a lerp
// between two neighbours.
//
// That is the trade the language is for: the work happens once, on a developer's
// machine, instead of on every board on every reading, forever.

// the count is a literal because bake_curve resolves it while this file compiles;
// everything downstream reads the size back off the table it produced
let CURVE <- bake_curve("thermistor.csv", 64)
let TABLE_SIZE = length(CURVE)
let ADC_LO = curve_lo("thermistor.csv")
let ADC_HI = curve_hi("thermistor.csv")

// Temperature in hundredths of a degree C for a raw ADC reading.
// Evenly spaced table, so the bucket is a divide and the rest is one lerp.
[export]
def temperature_centi(adc : int) : int {
    if (adc <= ADC_LO) {
        return CURVE[0]
    }
    if (adc >= ADC_HI) {
        return CURVE[TABLE_SIZE - 1]
    }
    let span = ADC_HI - ADC_LO
    let scaled = (adc - ADC_LO) * (TABLE_SIZE - 1)
    let bucket = scaled / span
    if (bucket >= TABLE_SIZE - 1) {
        return CURVE[TABLE_SIZE - 1]
    }
    // how far between bucket and bucket+1, in 1/256ths - no float on this path
    let frac = ((scaled - bucket * span) * 256) / span
    let lo = CURVE[bucket]
    let hi = CURVE[bucket + 1]
    return lo + ((hi - lo) * frac) / 256
}

// The table itself, so a caller can dump or checksum what got baked in.
[export]
def curve_point(index : int) : int {
    return index >= 0 && index < TABLE_SIZE ? CURVE[index] : 0
}

[export]
def curve_size : int {
    return TABLE_SIZE
}

[export]
def adc_range_lo : int {
    return ADC_LO
}

[export]
def adc_range_hi : int {
    return ADC_HI
}
