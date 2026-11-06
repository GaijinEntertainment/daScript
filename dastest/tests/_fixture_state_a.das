options gen2
require dastest/testing_boost
require _fixture_state_shared

[test]
def test_state_a(t : T?) {
    t |> equal(g_hits, 0, "a starts in a fresh context")
    g_hits = 1
}
