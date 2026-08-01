options gen2
options default_init_containers = false
require dastest/testing_boost public

struct Foo {
    x : int = 7
}

// The Probe fixture deliberately mirrors container_finalize.das rather than sharing a
// module: these two files are the policy-ON and policy-OFF halves of the same experiment,
// and an identical fixture compiled under each is the point.
var g_finalized = 0

struct Probe {
    xs : array<int>
}

def finalize(var p : Probe) {
    g_finalized++
    delete p.xs
}

[test]
def test_policy_off_no_finalize_banner(t : T?) {
    t |> run("erase does not finalize") @(t : T?) {
        var inscope arr : array<Probe>
        unsafe(resize(arr, 2))
        g_finalized = 0
        arr |> erase(0)
        t |> equal(g_finalized, 0)
    }
    t |> run("clear does not finalize") @(t : T?) {
        var inscope arr : array<Probe>
        unsafe(resize(arr, 2))
        g_finalized = 0
        arr |> clear()
        t |> equal(g_finalized, 0)
    }
    t |> run("table erase and clear do not finalize") @(t : T?) {
        var inscope tab : table<string; Probe>
        unsafe {
            tab["a"].xs |> push(1)
            tab["b"].xs |> push(2)
        }
        g_finalized = 0
        tab |> erase("a")
        tab |> clear()
        t |> equal(g_finalized, 0)
    }
    t |> run("the composite trait follows the policy") @(t : T?) {
        t |> success(!typeinfo needs_container_finalize(type<Probe>), "policy off disables the banner")
        t |> success(typeinfo is_safe_to_delete(type<Probe>), "the pure predicate does not follow it")
    }
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
