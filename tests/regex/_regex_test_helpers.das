options gen2
module _regex_test_helpers shared public

require daslib/regex

//! the end of a match from the start of `input`, -1 when none
def regex_test_match(pattern, input : string; offset : int = 0) : int {
    var re <- regex_compile(pattern)
    return regex_match(re, input, offset)
}
