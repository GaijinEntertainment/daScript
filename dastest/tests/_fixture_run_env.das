options gen2

require dastest/testing_boost public
require daslib/fio

//! a fixture for test_caches.das: under dastest's --run-env the probe
//! variable is set while the test runs, and when the spawning test
//! passes the value it expects through the child's own environment,
//! the probe holds those bytes

[test]
def test_probe_is_set_for_the_run(t : T?) {
    let probe = get_env_variable("DASTEST_RUN_ENV_PROBE")
    t |> success(!empty(probe), "the run's environment carries the probe")
    if (has_env_variable("DASTEST_RUN_ENV_EXPECT")) {
        let expected = get_env_variable("DASTEST_RUN_ENV_EXPECT")
        t |> equal(probe, expected, "the probe arrives byte for byte")
    }
}
