//! an int64 annotation argument below the int64 minimum does not
//! compile
options gen2
expect 10300
[comment(value=-9223372036854775809l)]
struct TooSmallSigned {}
