options gen2
options persistent_heap

require dastest/testing_boost

let N = 10000
let OUTER = 10
let TOTAL = N * OUTER
let STATES = 64

var g_lut : int[256]
var g_remap : int[256]
var g_bytes : uint8[256]
var g_colormap : uint8?
var g_translation : uint8?
var g_source : uint8?

def sum_argument_arrays(lut_array, remap_array : array<int>) {
    var sum = 0
    for (_j in range(OUTER)) {
        for (i in range(N)) {
            sum += lut_array[remap_array[i & 255]]
        }
    }
    return sum
}

def run_automaton(classes : uint8 const?; transitions : int const?; input : uint8 const?) {
    var state = 0
    for (_j in range(OUTER)) {
        for (i in range(N)) {
            unsafe {
                let c = int(classes[int(input[i])])
                state = transitions[state * STATES + c]
            }
        }
    }
    return state
}

[benchmark]
def double_index_tables(b : B?) {
    var lut, remap : int[256]
    var bytes : uint8[256]
    for (i in range(256)) {
        lut[i] = i
        remap[i] = 255 - i
        bytes[i] = uint8(255 - i)
        g_lut[i] = i
        g_remap[i] = 255 - i
        g_bytes[i] = uint8(255 - i)
    }
    let lut_array <- [for (i in range(256)); i]
    let remap_array <- [for (i in range(256)); 255 - i]
    var expected = 0
    for (i in range(N)) {
        expected += 255 - (i & 255)
    }
    expected *= OUTER
    b |> run("local_fixed_int_of_int/{TOTAL}", TOTAL) {
        var sum = 0
        for (_j in range(OUTER)) {
            for (i in range(N)) {
                sum += lut[remap[i & 255]]
            }
        }
        b |> equal(sum, expected)
    }
    b |> run("local_fixed_int_of_uint8/{TOTAL}", TOTAL) {
        var sum = 0
        for (_j in range(OUTER)) {
            for (i in range(N)) {
                sum += lut[int(bytes[i & 255])]
            }
        }
        b |> equal(sum, expected)
    }
    b |> run("global_fixed_int_of_int/{TOTAL}", TOTAL) {
        var sum = 0
        for (_j in range(OUTER)) {
            for (i in range(N)) {
                sum += g_lut[g_remap[i & 255]]
            }
        }
        b |> equal(sum, expected)
    }
    b |> run("global_fixed_int_of_uint8/{TOTAL}", TOTAL) {
        var sum = 0
        for (_j in range(OUTER)) {
            for (i in range(N)) {
                sum += g_lut[int(g_bytes[i & 255])]
            }
        }
        b |> equal(sum, expected)
    }
    b |> run("argument_array_int_of_int/{TOTAL}", TOTAL) {
        b |> equal(sum_argument_arrays(lut_array, remap_array), expected)
    }
}

[benchmark]
def double_index_pointers(b : B?) {
    var colormap <- [for (i in range(256)); uint8(255 - i)]
    var translation <- [for (i in range(256)); uint8(i ^ 1)]
    var source <- [for (i in range(128)); uint8(i * 2)]
    var screen : array<uint8>
    screen |> resize(N)
    g_colormap = unsafe(addr(colormap[0]))
    g_translation = unsafe(addr(translation[0]))
    g_source = unsafe(addr(source[0]))
    let dest = unsafe(addr(screen[0]))
    b |> run("doom_column/{TOTAL}", TOTAL) {
        for (_j in range(OUTER)) {
            var frac = 0
            var dest_addr : uint64 = unsafe(reinterpret<uint64>(dest))
            for (_i in range(N)) {
                unsafe {
                    *reinterpret<uint8?>(dest_addr) = g_colormap[int(g_source[frac >> 16 & 127])]
                }
                dest_addr++
                frac += 40000
            }
        }
        b |> equal(int(screen[0]), 255)
    }
    b |> run("doom_translated_column/{TOTAL}", TOTAL) {
        for (_j in range(OUTER)) {
            var frac = 0
            var dest_addr : uint64 = unsafe(reinterpret<uint64>(dest))
            for (_i in range(N)) {
                unsafe {
                    *reinterpret<uint8?>(dest_addr) = g_colormap[int(g_translation[
                        int(g_source[frac >> 16 & 127])])]
                }
                dest_addr++
                frac += 40000
            }
        }
        b |> equal(int(screen[0]), 254)
    }
    g_colormap = null
    g_translation = null
    g_source = null
}

[benchmark]
def double_index_automaton(b : B?) {
    var classes <- [for (i in range(256)); uint8(i & (STATES - 1))]
    var transitions <- [for (i in range(STATES * STATES)); (i * 7 + 3) & (STATES - 1)]
    var input <- [for (i in range(N)); uint8((i * 31) & 255)]
    let pclasses = unsafe(addr(classes[0]))
    let ptransitions = unsafe(addr(transitions[0]))
    let pinput = unsafe(addr(input[0]))
    let expected = run_automaton(pclasses, ptransitions, pinput)
    b |> run("class_then_transition/{TOTAL}", TOTAL) {
        b |> equal(run_automaton(pclasses, ptransitions, pinput), expected)
    }
}
