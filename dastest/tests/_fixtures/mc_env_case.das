options gen2
options indenting = 4

require dastest/testing_boost
require daslib/fio
require mc_env_value

[test]
def test_environment(t : T?) {
    print("env={compiled_value()}\n")
    t |> equal(compiled_value(), get_env_variable("DAS_CACHE_KEY_PROBE"))
}
