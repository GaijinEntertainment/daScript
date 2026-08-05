options gen2

require dastest/testing_boost public
require buildd_config
require daslib/json_boost
require strings

def private no_args : array<string> {
    return <- array<string>()
}

[test]
def test_defaults_and_toml(t : T?) {
    t |> run("defaults stand with no toml keys") @(t : T?) {
        var cfg = BuilddArgs()
        var sources : table<string; string>
        var err = ""
        let args <- no_args()
        t |> success(apply_config_text(cfg, "# empty\n", args, sources, err), "empty toml parses")
        t |> equal(cfg.port, 8111)
        t |> equal(cfg.server_url, "http://127.0.0.1:8101")
        t |> equal(cfg.poll_interval_seconds, 5)
        t |> success(cfg.announce, "announce defaults on")
    }
    t |> run("toml overrides defaults and records provenance") @(t : T?) {
        var cfg = BuilddArgs()
        var sources : table<string; string>
        var err = ""
        let args <- no_args()
        let body = "server_url = \"https://daslang.io\"\ntoken = \"s3cret\"\nbuild_timeout_seconds = 1200\nannounce = false\n"
        t |> success(apply_config_text(cfg, body, args, sources, err), "toml parses")
        t |> equal(cfg.server_url, "https://daslang.io")
        t |> equal(cfg.token, "s3cret")
        t |> equal(cfg.build_timeout_seconds, 1200)
        t |> success(!cfg.announce, "announce off")
        t |> equal(sources?["token"] ?? "", "toml")
        t |> equal(sources?["worktree"] ?? "absent", "absent")
    }
    t |> run("a wrong-typed value is an error, not a silent zero") @(t : T?) {
        var cfg = BuilddArgs()
        var sources : table<string; string>
        var err = ""
        let args <- no_args()
        t |> success(!apply_config_text(cfg, "build_timeout_seconds = \"long\"\n", args, sources, err), "string for a number fails")
        t |> success(find(err, "build_timeout_seconds") >= 0, "error names the key")
        t |> equal(cfg.build_timeout_seconds, 600)   // default stands
    }
    t |> run("a wrong-typed boolean is an error") @(t : T?) {
        var cfg = BuilddArgs()
        var sources : table<string; string>
        var err = ""
        let args <- no_args()
        t |> success(!apply_config_text(cfg, "announce = \"yes\"\n", args, sources, err), "string for a bool fails")
        t |> success(find(err, "announce") >= 0, "error names the key")
    }
}

[test]
def test_banner_hides_the_token(t : T?) {
    t |> run("the banner reports set/unset and provenance, never the value") @(t : T?) {
        var cfg = BuilddArgs()
        cfg.token = "the-actual-secret"
        var sources : table<string; string>
        sources["token"] = "toml"
        var banner = config_banner(cfg, sources)
        let text = write_json(banner)
        t |> success(find(text, "the-actual-secret") < 0, "token value absent")
        t |> success(banner?["token_set"] ?? false, "token_set true")
        t |> equal(banner?["token_src"] ?? "", "toml")
    }
}
