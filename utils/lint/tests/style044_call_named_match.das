options gen2

require dastest/testing_boost public
require daslib/style_lint

def match_rules(x : int; blk : block<(r : int) : void>) {
    invoke(blk, x)
}

def call_named_match(x : int) : string {
    var res = ""
    match_rules(x) $(r) {
        if (r == 3) {
            res = "three"
        }
    }
    return res
}

[test]
def test_a_call_named_like_match_is_no_match_head(t : T?) {
    t |> equal(call_named_match(3), "three")
}
