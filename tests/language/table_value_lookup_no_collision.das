options gen2
options disable_inline = true
options disable_auto_inline = true

require dastest/testing_boost public

def sum_lookups(var tab : table<string; int>; a, b : string) : int => tab[a] + tab[b]

[test]
def test_value_lookups_into_one_table(t : T?) {
    var tab <- { "a" => 1, "b" => 2 }
    t |> equal(sum_lookups(tab, "a", "b"), 3)
}
