options gen2
require dastest/testing_boost public

[inline]
def counted_positive(value : int; var calls : int&) : bool {
    calls++
    return value > 0
}

def conditional_stream(enabled : bool; mode : int) : iterator<int> {
    return <- generator<int> {
        var calls = 0
        for (value in range(3)) {
            if (mode == 0) {
                if (enabled && counted_positive(value, calls)) { yield value }
            } elif (mode == 1) {
                if (enabled || counted_positive(value, calls)) { yield value }
            } else {
                if (enabled ? counted_positive(value, calls) : false) { yield value }
            }
        }
        yield -calls
        return false
    }
}

def stream_matches(enabled : bool; mode : int; expected : array<int>) : bool {
    var stream <- conditional_stream(enabled, mode)
    var values : array<int> // nolint:STYLE027 - explicit consumption keeps the generator regression and call-count observation separate
    for (value in stream) { values |> push(value) }
    var equal = length(values) == length(expected)
    if (equal) { for (a, b in values, expected) { equal &&= a == b } }
    delete values
    return equal
}

def generator_conditions_work() : bool {
    return (stream_matches(true, 0, [1, 2, -3])
        && stream_matches(false, 0, [0])
        && stream_matches(true, 1, [0, 1, 2, 0])
        && stream_matches(false, 1, [1, 2, -3])
        && stream_matches(true, 2, [1, 2, -3])
        && stream_matches(false, 2, [0]))
}

[test]
def inline_generator_preserves_conditional_evaluation(t : T?) {
    t |> success(generator_conditions_work())
}

[export]
def main() {
    verify(generator_conditions_work())
}
