options gen2

require dastest/testing_boost public
require daslib/match

def private bucket(n : int) : string {
    return match (n) {
        (0..10) => "low"
        10 => "ten"
        (11..20) | (30..40) => "teen or thirties"
        (20..30) => "twenties"
        _ => "other"
    }
}

[test]
def test_ranges_and_constants_that_touch_but_do_not_overlap(t : T?) {
    t |> equal(bucket(0), "low")
    t |> equal(bucket(9), "low")
    t |> equal(bucket(10), "ten")
    t |> equal(bucket(11), "teen or thirties")
    t |> equal(bucket(35), "teen or thirties")
    t |> equal(bucket(25), "twenties")
    t |> equal(bucket(40), "other")
    t |> equal(bucket(-1), "other")
}
