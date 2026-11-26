options gen2
options persistent_heap

require dastest/testing_boost

let N = 10000
let OUTER = 10
let TOTAL = N * OUTER

def sum_bytes_argument(bytes : array<uint8>) {
    var sum = 0
    for (_j in range(OUTER)) {
        for (i in range(N)) {
            sum += int(bytes[i])
        }
    }
    return sum
}

[benchmark]
def array_at_small_int(b : B?) {
    var bytes <- [for (i in range(N)); uint8(i & 255)]
    var words <- [for (i in range(N)); uint16(i)]
    var bytes_expected = 0
    for (v in bytes) {
        bytes_expected += int(v)
    }
    bytes_expected *= OUTER
    var words_expected = 0u
    for (v in words) {
        words_expected += uint(v)
    }
    words_expected *= uint(OUTER)
    b |> run("uint8_local_array/{TOTAL}", TOTAL) {
        var sum = 0
        for (_j in range(OUTER)) {
            for (i in range(N)) {
                sum += int(bytes[i])
            }
        }
        b |> equal(sum, bytes_expected)
    }
    b |> run("uint8_argument_array/{TOTAL}", TOTAL) {
        b |> equal(sum_bytes_argument(bytes), bytes_expected)
    }
    b |> run("uint16_local_array_to_uint/{TOTAL}", TOTAL) {
        var sum = 0u
        for (_j in range(OUTER)) {
            for (i in range(N)) {
                sum += uint(words[i])
            }
        }
        b |> equal(sum, words_expected)
    }
}

[benchmark]
def fixed_at_small_int(b : B?) {
    var bytes : uint8[256]
    var shorts : int16[256]
    for (i in range(256)) {
        bytes[i] = uint8(i)
        shorts[i] = int16(i - 128)
    }
    b |> run("uint8_local_fixed/{TOTAL}", TOTAL) {
        var sum = 0
        for (_j in range(OUTER)) {
            for (i in range(N)) {
                sum += int(bytes[i & 255])
            }
        }
        b |> equal(sum > 0, true)
    }
    b |> run("int16_local_fixed/{TOTAL}", TOTAL) {
        var sum = 0
        for (_j in range(OUTER)) {
            for (i in range(N)) {
                sum += int(shorts[i & 255])
            }
        }
        b |> equal(sum < 0, true)
    }
}
