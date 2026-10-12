options gen2

require dastest/testing_boost public
require daslib/match

def private kind_of(t) : string {
    static_match (t) {
        true => $ { return "true" }
        false => $ { return "false" }
        _ => $ { return "other" }
    }
    return ""
}

[test]
def test_static_match_keeps_arms_other_instances_need(t : T?) {
    t |> run("an arm unreachable for one instance is not an error") @(t : T?) {
        t |> equal(kind_of(true), "true")
        t |> equal(kind_of(false), "false")
    }
    t |> run("the same arm matches in another instance") @(t : T?) {
        t |> equal(kind_of(5), "other")
    }
}
