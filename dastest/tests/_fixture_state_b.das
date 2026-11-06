options gen2
require dastest/testing_boost
require _fixture_state_shared

[test]
def test_state_b(t : T?) {
    t |> equal(g_hits, 0, "b starts in a fresh context, not in a's")
    g_hits = 1
}
