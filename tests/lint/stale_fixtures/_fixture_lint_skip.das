options gen2
// lint-skip-file: subject conflicts with lint policies (fixture)
// The deliberate finding below must never be reported - the directive above
// opts the whole file out (canonical user: tests/language/static_if.das).

def unused_require_bait {
    var x = 1
    x = 2
    print("{x}\n")
}
