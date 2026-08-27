# 05 — a table built by the compiler

An NTC thermistor read through a 12-bit ADC. `thermistor.csv` holds thirteen bench
measurements: raw ADC reading against temperature in hundredths of a degree. They are
unevenly spaced and in no particular order, because that is how calibration data arrives.

None of that reaches the device.

While `thermometer.das` compiles, the `bake_curve` macro in `csv_bake.das` reads the file,
sorts it, inverts the curve and resamples it onto an even 64-point grid. What links into the
program is the grid. `curve_lo` / `curve_hi` bake the calibrated ADC range the same way, so
the runtime never repeats those numbers by hand.

```das
let CURVE <- bake_curve("thermistor.csv", 64)
let ADC_LO = curve_lo("thermistor.csv")
```

and what survives into the generated C++ is only the answers: the 64 table values appear as
integer literals (the hot end is `10000`, one hundred degrees in hundredths), and `ADC_LO`
folds to a bare `267` at its comparison site. Grep the generated file for `thermistor`, `csv`
or `fread` and find nothing.

An evenly spaced table is indexed directly, so the search and the interpolation that a raw
calibration file would need never ship. What is left at run time is one divide, one index and
one lerp between neighbours — no floating point on that path.

## What it costs

Cross-compiled for a cortex-m4 (`-Os`, `-fno-exceptions -fno-rtti`, `--gc-sections`,
`--specs=nosys.specs`), linking `libDaScriptNano`:

| | |
|---|---|
| `.text` | 78,028 bytes |
| `.data` / `.bss` | 1,956 / 2,464 bytes |
| the baked table | 64 entries |
| the calibration file | 279 bytes, none of it in the image |

Nothing in the linked image can parse anything — `strtol`, `strtod`, `sscanf`, `atoi`,
`fopen` and `fread` are all absent. That is the point: the alternative is not "a slightly
bigger program", it is shipping a parser, and on this tier there is no file to parse anyway.

The resampling is lossy, and honestly so. The endpoints are exact (ADC 267 → 100.00 °C,
ADC 3740 → −20.00 °C); interior calibration rows come back within about a quarter degree
(ADC 613 reads 70.25  degC against a measured 70.00). Raising the point count trades flash for
accuracy, one literal, no code change.

## Running it

```
bin/Release/standalone_05_table
```

```
baked table: 64 points, calibrated over ADC 267..3740
adc  200 ->  100.00 C  (outside calibration, clamped)
adc  613 ->   70.25 C
adc 1825 ->   30.05 C
adc 3740 ->  -20.00 C
table[0] = 100.00 C, table[63] = -20.00 C
```

Both ends clamp on purpose. A reading outside the calibrated range is a real thing on a cold
morning, and the table has to answer anyway.

## Editing the data

`thermistor.csv` and `csv_bake.das` are build dependencies of the generated C++, so changing
either rebuilds the table. Try moving a row and watch the numbers move.

The parser tolerates blank lines, `#` comments and a header row. A file it cannot use is a
compile error naming the file - the failure happens on your machine, which is the other half
of what this tier buys.

## The shape to copy

A macro module is its own module (`module csv_bake shared private`) and is required by the
program that uses it. After expansion nothing from it is referenced, and standalone AOT drops
it - the generated source carries `// require csv_bake - compile time only, not linked`.
