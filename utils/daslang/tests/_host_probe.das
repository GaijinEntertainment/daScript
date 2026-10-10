options gen2

require dastest/testing_boost public
require daslib/fio
require daslib/clargs

//! run by a tool copy of daslang: the host a tool spawns its children through is daslang itself
[test]
def test_host_is_daslang(t : T?) {
    let host = base_name(get_host_binary())
    t |> success(starts_with(host, "daslang"), "the host binary is daslang, not '{host}'")
}
