options gen2

require dastest/testing_boost public
require _invoke_cmres_global_helper

def swapped_pair() : tuple<int; int> => (g_pair._1, g_pair._0)

[test]
def test_invoke_cmres_into_global_read_by_later_module(t : T?) {
    g_pair = (1, 2)
    set_pair(@@swapped_pair)
    t |> equal(g_pair._0, 2)
    t |> equal(g_pair._1, 1)
}
