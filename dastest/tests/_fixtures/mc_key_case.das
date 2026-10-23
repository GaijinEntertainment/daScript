options gen2
options indenting = 4

require dastest/testing_boost
require mc_key_mod

[test]
def test_key(t : T?) {
    print("key={key_value()}\n")
    t |> success(key_value() > 0, "the module answers")
}
