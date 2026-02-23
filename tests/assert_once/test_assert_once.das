options gen2
// note: AOT is disabled for this sample
//  this is due to the fact, that in AOT mode assert does not cause panic, and does C++ assert instead
options no_aot = true

require daslib/assert_once
require dastest/testing_boost public

[test]
def test_assert_once(t : T?) {
    var ii = 0
    var count = 0
    label 0:
    try {
        assert_once(ii == 0)
    } recover {
        count++
    }
    ii++
    if (ii != 5) {
        goto 0
    }
    t |> equal(count, 1)
}
