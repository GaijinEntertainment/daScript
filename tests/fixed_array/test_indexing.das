// Stage 0 characterization for the tFixedArray rework (FIXED_ARRAY_REWORK.md).
// Pins indexing: read/write at every depth, index expressions, const sources,
// range slicing via subarray, indexing through struct fields.
options gen2
require dastest/testing_boost public

struct Holder {
    grid : int[3][3]
}

let g_const : int[4] = fixed_array(10, 20, 30, 40)

[test(description = "fixed arrays of one, two and three dimensions read and write by constant and computed indices and a partial index of a 2D array yields a row")]
def test_index_rw(t : T?) {
    t |> run("1D read/write") @(t : T?) {
        var a : int[4]
        for (i in range(4)) {
            a[i] = i * i
        }
        t |> equal(a[0], 0)
        t |> equal(a[3], 9)
    }
    t |> run("2D and 3D read/write") @(t : T?) {
        var m : int[2][3]
        for (y in range(2)) {
            for (x in range(3)) {
                m[y][x] = y * 10 + x
            }
        }
        t |> equal(m[1][2], 12)
        var c : int[2][2][2]
        c[1][0][1] = 42
        t |> equal(c[1][0][1], 42)
        t |> equal(c[0][0][0], 0)
    }
    t |> run("index with expressions") @(t : T?) {
        var a : int[8]
        for (i in range(8)) {
            a[i] = i
        }
        let k = 3
        t |> equal(a[k + 1], 4)
        t |> equal(a[k * 2], 6)
    }
    t |> run("partial index of 2D yields row") @(t : T?) {
        var m : float[2][3]
        m[1][0] = 7.0
        t |> equal(typeinfo typename(m[1]), "float[3]")
        let row = m[1]      // copies the row
        t |> equal(row[0], 7.0)
        m[1][0] = 8.0
        t |> equal(row[0], 7.0)
    }
}

[test(description = "a const global fixed array reads correctly and indexing works through a struct field")]
def test_const_and_fields(t : T?) {
    t |> run("const global FA reads") @(t : T?) {
        t |> equal(g_const[0], 10)
        t |> equal(g_const[3], 40)
    }
    t |> run("indexing through struct field") @(t : T?) {
        var h : Holder
        h.grid[2][1] = 21
        t |> equal(h.grid[2][1], 21)
        t |> equal(h.grid[0][0], 0)
    }
}

[test(description = "subarray with a range or range64 and range-index sugar give views of the right elements")]
def test_subarray(t : T?) {
    t |> run("subarray range view") @(t : T?) {
        var s : int[4]
        for (i in range(4)) {
            s[i] = i * 10
        }
        var got <- [for (v in subarray(s, range(1, 3))); v]
        t |> equal(length(got), 2)
        t |> equal(got[0], 10)
        t |> equal(got[1], 20)
        delete got
    }
    t |> run("subarray range64 view") @(t : T?) {
        var s : int[4]
        for (i in range(4)) {
            s[i] = i * 10
        }
        var got <- [for (v in subarray(s, range64(1l, 3l))); v]
        t |> equal(length(got), 2)
        t |> equal(got[0], 10)
        t |> equal(got[1], 20)
        delete got
    }
    t |> run("range-index sugar a[r]") @(t : T?) {
        var s : int[4]
        for (i in range(4)) {
            s[i] = i + 1
        }
        var sum = 0
        for (v in s[range(0, 2)]) {
            sum += v
        }
        t |> equal(sum, 3)
    }
}

var g_small_int_table = fixed_array(3u8, 2u8, 1u8, 0u8)
var g_double_index_lut = fixed_array(10, 20, 30, 40)

def small_int_fixed_sum_argument(bytes : uint8[4]) {
    var sum = 0
    for (i in range(4)) {
        sum += int(bytes[i])
    }
    return sum
}

[test(description = "a fixed array of small ints read by index widens uint8 and uint16 with zeros and int8 and int16 with the sign, for a local, an argument and a global array, and an index out of range raises")]
def test_small_int_fixed_index_read(t : T?) {
    let bytes = fixed_array(200u8, 7u8, 255u8, 1u8)
    let chars = fixed_array(int8(-5), int8(100), int8(-128), int8(1))
    let words = fixed_array(uint16(60000), uint16(9), uint16(65535), uint16(1))
    let shorts = fixed_array(int16(-300), int16(300), int16(-32768), int16(1))
    var int_sum = 0
    var uint_sum = 0u
    for (i in range(4)) {
        int_sum += int(bytes[i]) + int(chars[i]) + int(words[i]) + int(shorts[i])
        uint_sum += uint(bytes[3 - i]) + uint(words[3 - i])
    }
    t |> equal(int_sum, 93209)
    t |> equal(uint_sum, 126008u)
    var index = 0
    t |> equal(uint(chars[index]), 0xfffffffb)
    t |> equal(uint(shorts[index]), 0xfffffed4)
    t |> equal(bytes[index], 200u8)
    t |> equal(int(bytes[1]), 7)
    t |> equal(small_int_fixed_sum_argument(bytes), 463)
    int_sum = 0
    for (i in range(4)) {
        int_sum += int(g_small_int_table[i]) + int(g_small_int_table[3 - i])
    }
    t |> equal(int_sum, 12)
    index = 4
    var raised = false
    try {
        int_sum = int(bytes[index])
    } recover {
        raised = true
    }
    t |> success(raised, "an index past the end of a uint8 fixed array raises")
    index = -1
    raised = false
    try {
        int_sum = int(chars[index])
    } recover {
        raised = true
    }
    t |> success(raised, "a negative index into an int8 fixed array raises")
}

[test(description = "a table indexed by an element of another table gives the same element whether the inner table holds ints or small ints, for local and global tables")]
def test_double_index_fixed(t : T?) {
    let lut = fixed_array(10, 20, 30, 40)
    let remap = fixed_array(3, 2, 1, 0)
    var bytes = fixed_array(3u8, 2u8, 1u8, 0u8)
    var through_ints = 0
    var through_bytes = 0
    var through_globals = 0
    for (i in range(4)) {
        through_ints += lut[remap[i]] * (i + 1)
        through_bytes += lut[int(bytes[i])] * (i + 1)
        through_globals += g_double_index_lut[int(g_small_int_table[i])] * (i + 1)
    }
    t |> equal(through_ints, 200)
    t |> equal(through_bytes, 200)
    t |> equal(through_globals, 200)
    var folded = 0
    for (c in bytes) {
        folded += lut[remap[int(c)]]
    }
    t |> equal(folded, 100)
}

[test(description = "a fixed array of small ints read through an expression index keeps the sign and still raises out of range")]
def test_small_int_fixed_expression_index(t : T?) {
    let chars = fixed_array(int8(-5), int8(100), int8(-128))
    let shorts = fixed_array(int16(-300), int16(300), int16(-32768))
    var int_sum = 0
    var uint_sum = 0u
    for (i in range(3)) {
        int_sum += int(chars[2 - i]) * (i + 1) + int(shorts[2 - i])
        uint_sum += uint(chars[2 - i])
    }
    t |> equal(int_sum, -32711)
    t |> equal(uint_sum, 0xffffffdf)
    var raised = false
    for (i in range(3, 4)) {
        try {
            int_sum = int(chars[i * 2 - 3])
        } recover {
            raised = true
        }
    }
    t |> success(raised, "an expression index past the end of an int8 fixed array raises")
}
