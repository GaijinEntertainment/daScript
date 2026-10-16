options gen2
options indenting = 4

require dastest/testing_boost public

require ../config.das

// The lanes as the children that run them: a run-only lane is a child
// of its own, and the lanes that build in the shared build dir share
// one child in the order given - two builds in one dir collide.
[test]
def test_lane_groups(t : T?) {
    let builds <- ["docs", "tests-aot", "utils-tests"]
    let lanes <- ["docs", "tests-cpp", "tests-interp", "tests-jit", "tests-aot", "utils-tests"]
    let groups <- lane_groups(lanes, builds)
    t |> equal(length(groups), 4, "three run-only lanes a child each, the building lanes one child")
    t |> equal(groups[0][0], "tests-cpp", "a run-only lane keeps its order")
    t |> equal(groups[1][0], "tests-interp", "a run-only lane keeps its order")
    t |> equal(groups[2][0], "tests-jit", "a run-only lane keeps its order")
    t |> equal(length(groups[3]), 3, "the building lanes share the last child")
    t |> equal(groups[3][0], "docs", "the building lanes keep the order given")
    t |> equal(groups[3][1], "tests-aot", "the building lanes keep the order given")
    t |> equal(groups[3][2], "utils-tests", "the building lanes keep the order given")
    let run_only <- lane_groups(["tests-interp", "tests-jit"], builds)
    t |> equal(length(run_only), 2, "no building lane, no building child")
    let builders <- lane_groups(["tests-aot", "docs"], builds)
    t |> equal(length(builders), 1, "building lanes alone make one child")
    t |> equal(length(builders[0]), 2, "building lanes alone make one child")
    let none : array<string>
    t |> equal(length(lane_groups(none, builds)), 0, "no lane, no child")
}
