The REGEX module implements regular expression matching and searching.
It provides ``regex_compile`` for building patterns, ``regex_match`` for
full-string matching, and ``regex_foreach`` for finding all matches within text.

See also :doc:`regex_boost` for compile-time regex construction via the ``%regex~`` reader macro.

All functions and symbols are in "regex" module, use require to get access to it. ::

    require daslib/regex

Example: ::

    require daslib/regex
        require strings

        [export]
        def main() {
            var re <- regex_compile("[0-9]+")
            let m = regex_match(re, "123abc")
            print("match length = {m}\n")
            let text = "age 25, height 180"
            regex_foreach(re, text) <| $(r) {
                print("found: {slice(text, r.x, r.y)}\n")
                return true
            }
        }
        // output:
        // match length = 3
        // found: 25
        // found: 180
