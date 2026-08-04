options gen2

require dastest/testing_boost public
require playground_config
require daslib/fio
require daslib/json_boost
require strings

def private no_args : array<string> {
    return <- array<string>()
}

[test]
def test_defaults_and_toml(t : T?) {
    t |> run("defaults stand with no toml keys") @(t : T?) {
        var cfg = ServerArgs()
        var sources : table<string; string>
        var err = ""
        let args <- no_args()
        t |> success(apply_config_text(cfg, "# empty\n", args, sources, err), "empty toml parses")
        t |> equal(cfg.port, 8101)
        t |> equal(cfg.base_url, "https://daslang.io")
        t |> equal(cfg.max_source_bytes, 262144)
    }
    t |> run("toml overrides defaults and records provenance") @(t : T?) {
        var cfg = ServerArgs()
        var sources : table<string; string>
        var err = ""
        let args <- no_args()
        let body = "port = 9000\ndb = \"/srv/x/samples.db\"\nmax_source_bytes = 1024\n"
        t |> success(apply_config_text(cfg, body, args, sources, err), "toml parses")
        t |> equal(cfg.port, 9000)
        t |> equal(cfg.db, "/srv/x/samples.db")
        t |> equal(cfg.max_source_bytes, 1024)
        t |> equal(sources?["port"] ?? "", "toml")
        t |> equal(sources?["max_inserts_per_ip_hour"] ?? "absent", "absent")
    }
    t |> run("bad toml reports an error") @(t : T?) {
        var cfg = ServerArgs()
        var sources : table<string; string>
        var err = ""
        let args <- no_args()
        t |> success(!apply_config_text(cfg, "port = [not toml", args, sources, err), "parse fails")
        t |> success(!empty(err), "error text set")
    }
    t |> run("a wrong-typed value is an error, not a silent zero") @(t : T?) {
        // Regression: a string where a number belongs coerced to 0 (and a
        // number where a string belongs to ""), with provenance still logged as
        // "toml" — the service would start on a config the file does not state.
        var cfg = ServerArgs()
        var sources : table<string; string>
        var err = ""
        let args <- no_args()
        t |> success(!apply_config_text(cfg, "max_source_bytes = \"256k\"\n", args, sources, err), "string for a number fails")
        t |> success(find(err, "max_source_bytes") >= 0, "error names the key")
        t |> equal(cfg.max_source_bytes, 262144)   // default stands
    }
    t |> run("a number where a string belongs is an error") @(t : T?) {
        var cfg = ServerArgs()
        var sources : table<string; string>
        var err = ""
        let args <- no_args()
        t |> success(!apply_config_text(cfg, "db = 7\n", args, sources, err), "number for a string fails")
        t |> success(find(err, "db") >= 0, "error names the key")
    }
}

[test]
def test_load_config_and_banner(t : T?) {
    t |> run("explicit missing config is an error") @(t : T?) {
        var cfg = ServerArgs()
        cfg.config = "no/such/dasweb.toml"
        var sources : table<string; string>
        var err = ""
        t |> success(!load_config(cfg, sources, err), "missing explicit config fails")
        t |> success(!empty(err), "error text set")
    }
    t |> run("explicit config file loads and merges") @(t : T?) {
        var terr = ""
        let dir = temp_directory(terr)
        let path = path_join(dir, "pg_cfg_test.toml")
        fwrite(path, "port = 9210\n")
        var cfg = ServerArgs()
        cfg.config = path
        var sources : table<string; string>
        var err = ""
        t |> success(load_config(cfg, sources, err), "config loads")
        t |> equal(cfg.port, 9210)
        t |> equal(sources?["port"] ?? "", "toml")
        var rerr = ""
        remove(path, rerr)
    }
    t |> run("banner carries every key with provenance") @(t : T?) {
        let cfg = ServerArgs()
        var sources : table<string; string>
        sources |> insert("port", "cli")
        let body = write_json(config_banner(cfg, sources))
        for (key in fixed_array("\"port\"", "\"port_src\" : \"cli\"", "\"db_src\" : \"default\"",
                "\"base_url\"", "\"max_source_bytes_src\"", "\"max_inserts_per_ip_hour_src\"",
                "\"curated_dir_src\"")) {
            t |> success(find(body, key) >= 0, "banner contains {key}")
        }
    }
}

[test]
def test_cli_precedence(t : T?) {
    t |> run("explicit cli flag beats toml") @(t : T?) {
        var cfg = ServerArgs()
        cfg.port = 7777   // as if parsed from --port
        var sources : table<string; string>
        var err = ""
        let args <- ["--port=7777"]
        mark_cli_sources(args, sources)
        t |> success(apply_config_text(cfg, "port = 9000\n", args, sources, err), "toml parses")
        t |> equal(cfg.port, 7777)
        t |> equal(sources?["port"] ?? "", "cli")
    }
    t |> run("short -p counts as the port flag") @(t : T?) {
        var cfg = ServerArgs()
        cfg.port = 7778
        var sources : table<string; string>
        var err = ""
        let args <- ["-p", "7778"]
        mark_cli_sources(args, sources)
        t |> success(apply_config_text(cfg, "port = 9000\n", args, sources, err), "toml parses")
        t |> equal(cfg.port, 7778)
    }
    t |> run("authoritative toml beats cli") @(t : T?) {
        var cfg = ServerArgs()
        cfg.port = 7777
        var sources : table<string; string>
        var err = ""
        let args <- ["--port=7777"]
        mark_cli_sources(args, sources)
        t |> success(apply_config_text(cfg, "authoritative = true\nport = 9000\n", args, sources, err), "toml parses")
        t |> equal(cfg.port, 9000)
        t |> equal(sources?["port"] ?? "", "toml")
    }
    t |> run("hyphenated cli spelling maps to underscored key") @(t : T?) {
        var cfg = ServerArgs()
        cfg.max_source_bytes = 555
        var sources : table<string; string>
        var err = ""
        let args <- ["--max-source-bytes=555"]
        mark_cli_sources(args, sources)
        t |> success(apply_config_text(cfg, "max_source_bytes = 9999\n", args, sources, err), "toml parses")
        t |> equal(cfg.max_source_bytes, 555)
        t |> equal(sources?["max_source_bytes"] ?? "", "cli")
    }
}
