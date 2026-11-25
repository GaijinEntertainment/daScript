options gen2

require dastest/testing_boost public

//! a fixture for uncategorized.das: passes only when its compile ran
//! under force_inscope_pod

[test]
def test_compiled_under_force_inscope_pod(t : T?) {
    t |> success(typeinfo needs_container_finalize(type<array<int>>),
                 "force_inscope_pod is on for this compile")
}
