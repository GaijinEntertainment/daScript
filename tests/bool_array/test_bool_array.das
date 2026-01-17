options gen2
require dastest/testing_boost
require daslib/faker
require daslib/fuzzer
require daslib/strings_boost

require daslib/bool_array

[test]
def test_fopen(t : T?) {
    t |> run("resie") <| @@(t : T?) {
        var a : BoolArray
        // lets test all possible tails
        for (size in 0 .. 64) {
            a.clear()
            for (i in 0 .. size) {
                a.resize(i)
                // set all bits to true
                for (j in 0 .. i) {
                    a[j] = true
                }
                // now check
                for (j in 0 .. i) {
                    t |> equal(a[j], true)
                }
                // resize to zero and back
                a.resize(0)
                a.resize(i)
                // check all bits are false
                for (j in 0 .. i) {
                    t |> equal(a[j], false)
                }
            }
        }
    }
    t |> run("erase") <| @@(t : T?) {
        var a : BoolArray
        // test all possible erases
        for (erase_i in 0 .. 100) {
            a.clear()
            for (i in 0 .. 100) {
                a.push(i % 2 == 0)
            }
            a.erase(erase_i)
            // now check
            for (i in 0 .. 99) {
                if (i < erase_i) {
                    t |> equal(a[i], i % 2 == 0)
                } else {
                    t |> equal(a[i], (i + 1) % 2 == 0)
                }
            }
        }
    }
    t |> run("insert") <| @@(t : T?) {
        var a : BoolArray
        // test all possible inserts
        for (insert_i in 0 .. 100) {
            a.clear()
            for (i in 0 .. 100) {
                a.push(i % 2 == 0)
            }
            a.insert(insert_i, true)
            // now check
            for (i in 0 .. 100) {
                if (i < insert_i) {
                    t |> equal(a[i], i % 2 == 0)
                } elif (i == insert_i) {
                    t |> equal(a[i], true)
                } else {
                    t |> equal(a[i], (i - 1) % 2 == 0)
                }
            }
        }
    }
    t |> run("iterator") <| @@(t : T?) {
        var a : BoolArray
        for (i in 0 .. 10) {
            a.push(i % 2 == 0)
        }
        var idx = 0
        for (b in a) {
            t |> equal(b, idx % 2 == 0)
            idx += 1
        }
        t |> equal(idx, 10)
    }
}

