options gen2
options default_init_containers = false
require dastest/testing_boost public

struct Foo {
    x : int = 7
}

[test]
def test_policy_off_old_behavior(t : T?) {
    t |> run("table slots stay zeroed") @(t : T?) {
        var inscope tab : table<string; Foo>
        tab["a"].x += 5
        t |> equal(tab["a"].x, 5)
    }
    t |> run("resize zeroes under unsafe") @(t : T?) {
        var inscope arr : array<Foo>
        unsafe(resize(arr, 2))
        t |> equal(arr[1].x, 0)
    }
    t |> run("traits reflect the off policy") @(t : T?) {
        t |> success(!typeinfo needs_container_init(type<Foo>), "composite trait follows the policy")
        t |> success(typeinfo needs_nontrivial_init(type<Foo>), "pure trait does not")
    }
}
