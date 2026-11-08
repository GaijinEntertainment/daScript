//! in gen1 syntax an int64 annotation argument above the int64
//! maximum does not compile
options gen2 = false
expect 30151
[comment(value=9223372036854775808l)]
struct TooLargeSigned
    value : int
