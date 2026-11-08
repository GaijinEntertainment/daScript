options gen2

require dastest/testing_boost
require daslib/temp_strings
require daslib/lpipe
require strings
require daslib/random

[test(description = "delete_string frees a heap string, leaves a constant or empty string alone and clears the argument")]
def test_delete_string(t : T?) {
    var constStr = "123"
    {
        let removed = unsafe(delete_string(constStr))
        t.equal(removed, false) // const heap string should not be removed
        t.equal(constStr, "123")
    }

    var seed = random_seed(0)
    var str = "test{random_float(seed)}"
    {
        let removed = unsafe(delete_string(str))
        t.equal(removed, true) // heap string should be removed
        t.equal(str, "")
    }

    {
        var emptyStr : string
        let removed = unsafe(delete_string(emptyStr))
        t.equal(removed, false) // corner case: empty string should not be removed
        t.equal(emptyStr, "")
    }
}

[test(description = "delete_string on a temp string frees a heap string and leaves a constant string alone")]
def test_delete_temp_string(t : T?) {
    var tempString = unsafe(reinterpret<string#>("123"))
    {
        let removed = unsafe(delete_string(tempString))
        t.equal(removed, false) // const string should not be removed
        t.equal(tempString, "123")
    }

    var seed = random_seed(0)
    var tempString2 = unsafe(reinterpret<string#>("test{random_float(seed)}"))
    {
        let removed = unsafe(delete_string(tempString2))
        t.equal(removed, true) // heap string should be removed
        t.equal(tempString2, "") // temp string should be empty after removal
    }
}

var payload = "long str"

[test(description = "temp_string and build_temp_string with lpipe allocate nothing on the string heap while the equivalent ordinary string builds allocate the expected bytes")]
def test_temps_strings(t : T?) {

    t |> run("no heap allocations") @@(t) {

        temp_string("long str {payload}\n") $(s) {
            t |> equal(s, "long str long str\n")
        }

        build_temp_string() $(sb) {
            sb |> write("long builder str ")
            sb |> write(payload)
            sb |> write("\n")
        }
        lpipe() $(s) {
            t |> equal(s, "long builder str long str\n")
        }

        t |> equal(string_heap_bytes_allocated(), 0ul)

        let arr <- [ uint8('h'), uint8('e'), uint8('l'), uint8('l'), uint8('o')]
        temp_string(arr) $(s) {
            t |> equal(s, "hello")
        }

        t |> equal(string_heap_bytes_allocated(), 0x0ul)
    }

    t |> run("heap allocations") @@(t) {

        feint("long long str {payload}\n")
        t |> equal(string_heap_bytes_allocated(), 0x18ul)

        let str = build_string() $(sb) {
            sb |> write("long builder str ")
            sb |> write(payload)
            sb |> write("\n")
        }
        feint(str)

        t |> equal(string_heap_bytes_allocated(), 0x34ul)
    }
}
