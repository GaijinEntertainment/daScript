options gen2
require dastest/testing_boost
require daslib/temp_strings
require daslib/lpipe

options intern_strings

var payload = "long str"

[test(description = "with intern_strings on, temp_string and lpipe results stay interned on the string heap because the number of references is unknown")]
def test_temps_strings(t : T?) {

    t |> run("keep intern strings") @@(t) {

        temp_string("long str {payload}\n") $(s) {
            t |> equal(s, "long str long str\n")
        }

        // keep intern strings, because amount of references is unknown
        t |> equal(string_heap_bytes_allocated(), 0x14ul)

        build_temp_string() $(sb) {
            sb |> write("long builder str ")
            sb |> write(payload)
            sb |> write("\n")
        }
        lpipe() $(s) {
            t |> equal(s, "long builder str long str\n")
        }

        // keep this intern string too
        t |> equal(string_heap_bytes_allocated(), 0x30ul)
    }
}
