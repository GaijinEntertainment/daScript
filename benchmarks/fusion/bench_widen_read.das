options gen2
options persistent_heap

require dastest/testing_boost

let N = 10000
let OUTER = 10
let TOTAL = N * OUTER

struct Texel {
    pad : int
    color : uint8
    height : uint16
}

def sum_color_argument(t : Texel) {
    var sum = 0
    for (_j in range(OUTER)) {
        for (_i in range(N)) {
            sum += int(t.color)
        }
    }
    return sum
}

[benchmark]
def widen_read(b : B?) {
    var bytes <- [for (i in range(N)); uint8(i & 255)]
    var texels <- [for (i in range(N)); Texel(color = uint8(i & 255), height = uint16(i))]
    var lut : int[256]
    for (i in range(256)) {
        lut[i] = i
    }
    var expected = 0
    for (v in bytes) {
        expected += int(v)
    }
    expected *= OUTER
    b |> run("loop_variable_to_int/{TOTAL}", TOTAL) {
        var sum = 0
        for (_j in range(OUTER)) {
            for (c in bytes) {
                sum += int(c)
            }
        }
        b |> equal(sum, expected)
    }
    b |> run("loop_variable_as_index/{TOTAL}", TOTAL) {
        var sum = 0
        for (_j in range(OUTER)) {
            for (c in bytes) {
                sum += lut[int(c)]
            }
        }
        b |> equal(sum, expected)
    }
    b |> run("loop_struct_field_to_int/{TOTAL}", TOTAL) {
        var sum = 0
        for (_j in range(OUTER)) {
            for (t in texels) {
                sum += int(t.color)
            }
        }
        b |> equal(sum, expected)
    }
    b |> run("loop_struct_field_to_uint/{TOTAL}", TOTAL) {
        var sum = 0u
        for (_j in range(OUTER)) {
            for (t in texels) {
                sum += uint(t.height)
            }
        }
        b |> equal(sum > 0u, true)
    }
    let texel = texels[3]
    b |> run("local_struct_field_to_int/{TOTAL}", TOTAL) {
        var sum = 0
        for (_j in range(OUTER)) {
            for (_i in range(N)) {
                sum += int(texel.color)
            }
        }
        b |> equal(sum, 3 * TOTAL)
    }
    b |> run("argument_struct_field_to_int/{TOTAL}", TOTAL) {
        b |> equal(sum_color_argument(texel), 3 * TOTAL)
    }
}
