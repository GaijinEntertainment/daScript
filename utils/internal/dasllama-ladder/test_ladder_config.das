options gen2

require dastest/testing_boost public
require ladder_config
require daslib/json_boost

// Config-merge coverage: defaults < toml < explicit CLI, authoritative flip,
// wrong-typed keys refused. Pure data — no server, no filesystem discovery.

[test]
def test_defaults(t : T?) {
    t |> run("defaults stand with no config") @(t : T?) {
        let cfg = LadderArgs()
        t |> equal(cfg.port, 8201)
        t |> equal(cfg.db, "ladder.db")
        t |> equal(cfg.max_submissions_per_ip_hour, 20)
        t |> equal(cfg.submit_open, false, "public submissions closed by default")
        t |> equal(cfg.official_dir, "")
    }
}

[test]
def test_toml_merge(t : T?) {
    t |> run("toml overrides defaults and records provenance") @(t : T?) {
        var cfg = LadderArgs()
        var sources : table<string; string>
        var err = ""
        let no_args : array<string>
        let body = "port = 9301\ndb = \"other.db\"\nmax_sidecar_bytes = 2048\n"
        t |> success(apply_config_text(cfg, body, no_args, sources, err), "merge ok ({err})")
        t |> equal(cfg.port, 9301)
        t |> equal(cfg.db, "other.db")
        t |> equal(cfg.max_sidecar_bytes, 2048)
        t |> equal(cfg.max_records_bytes, 4194304, "untouched key keeps its default")
        t |> equal(sources?["port"] ?? "", "toml")
    }
    t |> run("explicit CLI beats a non-authoritative toml") @(t : T?) {
        var cfg = LadderArgs()
        cfg.port = 9999   // as if parsed from the command line
        var sources : table<string; string>
        var err = ""
        let args <- ["--port", "9999"]
        mark_cli_sources(args, sources)
        t |> success(apply_config_text(cfg, "port = 9301\n", args, sources, err), "merge ok")
        t |> equal(cfg.port, 9999, "cli wins")
        t |> equal(sources?["port"] ?? "", "cli")
    }
    t |> run("authoritative toml beats CLI") @(t : T?) {
        var cfg = LadderArgs()
        cfg.port = 9999
        var sources : table<string; string>
        var err = ""
        let args <- ["--port", "9999"]
        mark_cli_sources(args, sources)
        t |> success(apply_config_text(cfg, "authoritative = true\nport = 9301\n", args, sources, err), "merge ok")
        t |> equal(cfg.port, 9301, "authoritative toml wins")
        t |> equal(sources?["port"] ?? "", "toml")
    }
    t |> run("wrong-typed keys are refused, never coerced") @(t : T?) {
        var cfg = LadderArgs()
        var sources : table<string; string>
        var err = ""
        let no_args : array<string>
        t |> success(!apply_config_text(cfg, "port = \"eightytwo\"\n", no_args, sources, err), "string port refused")
        t |> success(find(err, "port") >= 0, "error names the key")
        err = ""
        t |> success(!apply_config_text(cfg, "db = 42\n", no_args, sources, err), "numeric db refused")
    }
    t |> run("the banner carries values with provenance") @(t : T?) {
        var cfg = LadderArgs()
        var sources : table<string; string>
        var err = ""
        let no_args : array<string>
        t |> success(apply_config_text(cfg, "port = 9301\n", no_args, sources, err), "merge ok")
        let banner = write_json(config_banner(cfg, sources))
        t |> success(find(banner, "9301") >= 0, "value present")
        t |> success(find(banner, "toml") >= 0, "provenance present")
    }
}

[test]
def test_submit_open_config(t : T?) {
    t |> run("submit_open flips via toml, with provenance") @(t : T?) {
        var cfg = LadderArgs()
        var sources : table<string; string>
        var err = ""
        let no_args : array<string>
        t |> equal(cfg.submit_open, false, "closed by default")
        t |> success(apply_config_text(cfg, "submit_open = true\n", no_args, sources, err), "merge ok ({err})")
        t |> success(cfg.submit_open, "toml opened it")
        t |> equal(sources?["submit_open"] ?? "", "toml")
    }
    t |> run("explicit --submit-open beats a non-authoritative toml") @(t : T?) {
        var cfg = LadderArgs()
        cfg.submit_open = true   // as if clargs parsed --submit-open
        var sources : table<string; string>
        var err = ""
        let args <- ["--submit-open"]
        mark_cli_sources(args, sources)
        t |> success(apply_config_text(cfg, "submit_open = false\n", args, sources, err), "merge ok")
        t |> success(cfg.submit_open, "cli open wins over a non-authoritative toml false")
        t |> equal(sources?["submit_open"] ?? "", "cli")
    }
    t |> run("a wrong-typed submit_open is refused, never coerced") @(t : T?) {
        var cfg = LadderArgs()
        var sources : table<string; string>
        var err = ""
        let no_args : array<string>
        t |> success(!apply_config_text(cfg, "submit_open = \"yes\"\n", no_args, sources, err), "string submit_open refused")
        t |> success(find(err, "submit_open") >= 0, "error names the key")
    }
}
