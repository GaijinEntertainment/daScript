options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require live/live_api

[test]
def test_parse_port_string(t : T?) {
    t |> run("valid port") @(t : T?) {
        t |> equal(parse_port_string("19090"), 19090)
        t |> equal(parse_port_string("1"), 1)
        t |> equal(parse_port_string("65535"), 65535)
    }
    t |> run("out-of-range rejected as 0") @(t : T?) {
        t |> equal(parse_port_string("0"), 0)
        t |> equal(parse_port_string("65536"), 0)
        t |> equal(parse_port_string("-1"), 0)
        t |> equal(parse_port_string("999999"), 0)
    }
    t |> run("non-numeric rejected as 0") @(t : T?) {
        t |> equal(parse_port_string(""), 0)
        t |> equal(parse_port_string("abc"), 0)
        t |> equal(parse_port_string("9090 "), 0)
        t |> equal(parse_port_string(" 9090"), 9090)
    }
}

[test]
def test_parse_argv_port(t : T?) {
    t |> run("--live-port N") @(t : T?) {
        t |> equal(parse_argv_port(["--live-port", "19090"]), 19090)
    }
    t |> run("--live-port=N") @(t : T?) {
        t |> equal(parse_argv_port(["--live-port=19091"]), 19091)
    }
    t |> run("missing flag") @(t : T?) {
        t |> equal(parse_argv_port(["foo", "bar"]), 0)
        t |> equal(parse_argv_port([]), 0)
    }
    t |> run("invalid value") @(t : T?) {
        t |> equal(parse_argv_port(["--live-port", "abc"]), 0)
        t |> equal(parse_argv_port(["--live-port", "0"]), 0)
        t |> equal(parse_argv_port(["--live-port", "65536"]), 0)
    }
    t |> run("last occurrence wins (clargs convention)") @(t : T?) {
        t |> equal(parse_argv_port(["--live-port", "9090", "--live-port", "19090"]), 19090)
    }
}

[test]
def test_parse_config_json_port(t : T?) {
    t |> run("valid port key") @(t : T?) {
        t |> equal(parse_config_json_port("\{\"port\": 19090\}"), 19090)
    }
    t |> run("empty / missing key / out-of-range / wrong type → 0") @(t : T?) {
        t |> equal(parse_config_json_port(""), 0)
        t |> equal(parse_config_json_port("\{\}"), 0)
        t |> equal(parse_config_json_port("\{\"other\": 1\}"), 0)
        t |> equal(parse_config_json_port("\{\"port\": 0\}"), 0)
        t |> equal(parse_config_json_port("\{\"port\": 99999\}"), 0)
        t |> equal(parse_config_json_port("\{\"port\": \"19090\"\}"), 0)
    }
    t |> run("malformed JSON → 0") @(t : T?) {
        t |> equal(parse_config_json_port("not json"), 0)
        t |> equal(parse_config_json_port("\{port: 19090\}"), 0)
    }
}

[test]
def test_resolve_precedence(t : T?) {
    t |> run("argv beats env beats config") @(t : T?) {
        let no_argv : array<string>
        t |> equal(resolve_port_override_from(["--live-port", "11111"], "22222", "\{\"port\": 33333\}"), 11111)
        t |> equal(resolve_port_override_from(no_argv, "22222", "\{\"port\": 33333\}"), 22222)
        t |> equal(resolve_port_override_from(no_argv, "", "\{\"port\": 33333\}"), 33333)
    }
    t |> run("invalid at higher level falls through to next") @(t : T?) {
        let no_argv : array<string>
        t |> equal(resolve_port_override_from(["--live-port", "99999"], "22222", ""), 22222)
        t |> equal(resolve_port_override_from(["--live-port", "abc"], "", "\{\"port\": 33333\}"), 33333)
        t |> equal(resolve_port_override_from(no_argv, "garbage", "\{\"port\": 33333\}"), 33333)
    }
    t |> run("all empty → 0 (caller keeps default)") @(t : T?) {
        let no_argv : array<string>
        t |> equal(resolve_port_override_from(no_argv, "", ""), 0)
    }
}

[test]
def test_set_port_still_overrides(t : T?) {
    t |> run("live_api_set_port wins after init") @(t : T?) {
        // [init] already ran with the test process's real env (likely no override
        // → port stayed 9090, or env-driven if Boris's shell has DASLANG_LIVE_PORT).
        // Either way, an explicit setter call should land on the requested value.
        let original = live_api_port
        live_api_set_port(54321)
        t |> equal(live_api_port, 54321)
        live_api_set_port(original)
    }
}
