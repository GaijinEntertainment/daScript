options gen2

require dastest/testing_boost public
require daslib/match

variant Reading {
    value : float
    error : string
}

def private reading_or_zero(r : Reading) : float {
    let f = match (r) {
        value(v) => v
        error(e) => $ {
            feint(e)
            return 0.0
        }
    }
    return f + 1.0
}

def private sign_name(n : int) : string {
    return match (n) {
        0 => "zero"
        _ => $ {
            if (n > 0) {
                return "positive"
            }
            return "negative"
        }
    }
}

def private bucket_sum(xs : array<int>) : int {
    var total = 0
    for (x in xs) {
        let w = match (x) {
            (0..10) => 1
            _ => $ {
                let scaled = x / 10
                return scaled * scaled
            }
        }
        total += w
    }
    return total
}

[test]
def test_match_value_block_body(t : T?) {
    t |> run("a return in a block body is the arm's value") @(t : T?) {
        t |> equal(reading_or_zero(Reading(value = 2.0)), 3.0)
        t |> equal(reading_or_zero(Reading(error = "no sensor")), 1.0)
    }
    t |> run("a block body returns on every path it takes") @(t : T?) {
        t |> equal(sign_name(0), "zero")
        t |> equal(sign_name(5), "positive")
        t |> equal(sign_name(-5), "negative")
    }
    t |> run("a return in a block body leaves the arm, not the loop or the function") @(t : T?) {
        t |> equal(bucket_sum([3, 25, 7, 40]), 1 + 4 + 1 + 16)
    }
}
