options gen2

require dastest/testing_boost public
require daslib/fio
require strings
require daslib/strings_boost

// `dasllama-deploy.sh caddy` against scratch Caddyfiles: the script
// runs as written, with its two paths pointed at the test directory
// and `caddy` / `systemctl` stubbed. Skips where the box has no `sh`
// or `awk`.

let private LADDER = "{get_das_root()}/utils/internal/dasllama-ladder"
let private REGION_OPEN = "# >>> caddy.snippet (dasllama-deploy.sh caddy rewrites to <<<)"
let private REGION_CLOSE = "# <<< caddy.snippet"
let private VHOST_OWN = "\troot * /srv/dasllama.io/current\n\tfile_server\n"

struct private Stage {
    dir : string
    caddyfile : string
}

def private snippet_directive_lines() : string {
    let directives <- [for (line in split(fread("{LADDER}/caddy.snippet"), "\n")); "\t{line}\n";
        where !empty(strip(line)) && !starts_with(strip(line), "#")]
    return join(directives, "")
}

def private region_of(directive_lines : string) : string =>
    "\t{REGION_OPEN}\n{directive_lines}\t{REGION_CLOSE}\n"

def private caddyfile_of(vhost_body : string) : string {
    let neighbour = "\{\n\tencode gzip\n\}\n"
    return "before.io {neighbour}\ndasllama.io \{\n{vhost_body}\}\n\nafter.io {neighbour}"
}

def private stage(name, vhost_body : string) : Stage {
    let dir = replace(path_join(test_temp_dir(), name), "\\", "/")
    mkdir(dir)
    mkdir("{dir}/app")
    mkdir("{dir}/app/current")
    fwrite("{dir}/app/current/caddy.snippet", fread("{LADDER}/caddy.snippet"))
    fwrite("{dir}/Caddyfile", caddyfile_of(vhost_body))
    var script = fread("{LADDER}/dasllama-deploy.sh")
    script = replace(script, "APP=/srv/apps/dasllama-ladder\n", "APP={dir}/app\n")
    let caddy_stub = "caddy() \{ [ ! -f {dir}/caddy-rejects ]; \}\n"
    let reload_stub = "systemctl() \{ echo reloaded >> {dir}/reloads; \}\n"
    script = replace(script, "CADDYFILE=/etc/caddy/Caddyfile\n",
        "CADDYFILE={dir}/Caddyfile\n{caddy_stub}{reload_stub}")
    fwrite("{dir}/deploy.sh", script)
    return Stage(dir = dir, caddyfile = "{dir}/Caddyfile")
}

def private backups(st : Stage) : int {
    var count = 0
    dir(st.dir) $(name) {
        if (starts_with(name, "Caddyfile.bak-")) {
            count++
        }
    }
    return count
}

def private run_caddy(st : Stage; var output : string&) : int =>
    run_and_capture(["sh", "{st.dir}/deploy.sh", "caddy"], output)

def private one_line_encode(directive_lines : string) : string {
    let head = "\tencode zstd gzip \{\n"
    let from = find(directive_lines, head)
    let to = find(directive_lines, "\n\t\}\n", from) + 4
    return "{slice(directive_lines, 0, from)}\tencode zstd gzip\n{slice(directive_lines, to)}"
}

def private with_runs_cap(directive_lines, cap : string) : string =>
    replace(directive_lines, "\thandle /api/runs \{\n\t\trequest_body \{\n\t\t\tmax_size 64KB\n",
        "\thandle /api/runs \{\n\t\trequest_body \{\n\t\t\tmax_size {cap}\n")

[test(description = "the caddy snippet's encode block names its content types, application/wasm among them")]
def test_snippet_compresses_wasm(t : T?) {
    let snippet = fread("{LADDER}/caddy.snippet")
    let from = find(snippet, "\nencode zstd gzip \{\n")
    t |> success(from >= 0, "encode opens a block")
    let encode = slice(snippet, from, find(snippet, "\n\}\n", from))
    t |> success(find(encode, "\tmatch \{\n") >= 0, "encode names its content types")
    t |> success(find(encode, "header Content-Type application/wasm*") >= 0, "wasm is among them")
}

def private has_shell() : bool => executable_on_path("sh") && executable_on_path("awk")

[test(description = "deploy.sh caddy writes the snippet region once into the dasllama.io vhost, gathers or drops stray copies of its directives, restores edited values, and leaves a matching vhost alone", timeout = 1200)]
def test_caddy_region(t : T?) {
    if (!has_shell()) {
        t |> skip("no sh or awk on this box")
        return
    }
    let lines = snippet_directive_lines()
    let written = "{region_of(lines)}{VHOST_OWN}"
    t |> success(with_runs_cap(lines, "1GB") != lines, "the fixture names the snippet's cap")
    t |> run("a vhost that carries the snippet as written is left alone") @(t : T?) {
        let st = stage("written", written)
        var out = ""
        t |> equal(0, run_caddy(st, out), out)
        t |> success(find(out, "nothing to do") >= 0, out)
        t |> equal(caddyfile_of(written), fread(st.caddyfile))
        t |> success(!fexist("{st.dir}/reloads"), "nothing changed, nothing reloaded")
        t |> equal(0, backups(st), "nothing changed, nothing backed up")
    }
    t |> run("a bare vhost takes the region ahead of its own directives, once") @(t : T?) {
        let st = stage("bare", VHOST_OWN)
        var out = ""
        t |> equal(0, run_caddy(st, out), out)
        t |> equal(caddyfile_of(written), fread(st.caddyfile))
        t |> equal("reloaded\n", fread("{st.dir}/reloads"), "one reload for the run")
        t |> equal(0, run_caddy(st, out), out)
        t |> success(find(out, "nothing to do") >= 0, out)
        t |> equal(1, backups(st), "a run that changes nothing leaves the earlier backup alone")
    }
    t |> run("an unmarked vhost has its copies of the snippet's directives gathered") @(t : T?) {
        let nested = "\thandle_path /blobs/* \{\n\t\tencode gzip\n\t\}\n"
        let copies = with_runs_cap(one_line_encode(lines), "1GB")
        let st = stage("unmarked", "{nested}{copies}{VHOST_OWN}")
        var out = ""
        t |> equal(0, run_caddy(st, out), out)
        t |> equal(caddyfile_of("{region_of(lines)}{nested}{VHOST_OWN}"), fread(st.caddyfile),
            "the vhost's own directives stay, a nested one of a snippet's name among them")
    }
    t |> run("a directive the snippet does not have leaves the region") @(t : T?) {
        let stale = "\thandle /api/retired \{\n\t\treverse_proxy 127.0.0.1:8201\n\t\}\n"
        let with_stale = region_of("{lines}{stale}")
        let st = stage("stale", "{with_stale}{VHOST_OWN}")
        var out = ""
        t |> equal(0, run_caddy(st, out), out)
        t |> equal(caddyfile_of(written), fread(st.caddyfile))
    }
    t |> run("a value changed inside the region is put back") @(t : T?) {
        let changed = region_of(with_runs_cap(lines, "1GB"))
        let st = stage("changed", "{changed}{VHOST_OWN}")
        var out = ""
        t |> equal(0, run_caddy(st, out), out)
        t |> equal(caddyfile_of(written), fread(st.caddyfile))
    }
    t |> run("a top-level copy of a snippet directive outside the region is dropped") @(t : T?) {
        let st = stage("copy", "{written}\tredir /index.html / 308\n")
        var out = ""
        t |> equal(0, run_caddy(st, out), out)
        t |> equal(caddyfile_of(written), fread(st.caddyfile))
    }
    t |> run("a second region is dropped") @(t : T?) {
        let again = region_of(lines)
        let st = stage("twice", "{written}{again}")
        var out = ""
        t |> equal(0, run_caddy(st, out), out)
        t |> equal(caddyfile_of(written), fread(st.caddyfile))
    }
}

[test(description = "deploy.sh caddy refuses a file with no dasllama.io vhost or an unclosed region, and rolls back a config caddy rejects without reloading", timeout = 1200)]
def test_caddy_refusals(t : T?) {
    if (!has_shell()) {
        t |> skip("no sh or awk on this box")
        return
    }
    let lines = snippet_directive_lines()
    let written = "{region_of(lines)}{VHOST_OWN}"
    t |> run("a file with no dasllama.io vhost is refused and left alone") @(t : T?) {
        let st = stage("novhost", written)
        let elsewhere = replace(fread(st.caddyfile), "dasllama.io \{", "dasllama.example \{")
        fwrite(st.caddyfile, elsewhere)
        var out = ""
        t |> success(run_caddy(st, out) != 0, out)
        t |> success(find(out, "found no dasllama.io vhost") >= 0, out)
        t |> equal(elsewhere, fread(st.caddyfile))
        t |> equal(0, backups(st))
    }
    t |> run("a region that never closes is refused") @(t : T?) {
        let unclosed = "\t{REGION_OPEN}\n{lines}{VHOST_OWN}"
        let st = stage("unclosed", unclosed)
        var out = ""
        t |> success(run_caddy(st, out) != 0, out)
        t |> success(find(out, "do not pair up") >= 0, out)
        t |> equal(caddyfile_of(unclosed), fread(st.caddyfile))
        t |> equal(0, backups(st))
        t |> success(!fexist("{st.dir}/reloads"))
    }
    t |> run("a second region that never closes is refused") @(t : T?) {
        let dangling = "{written}\t{REGION_OPEN}\n"
        let st = stage("dangling", dangling)
        var out = ""
        t |> success(run_caddy(st, out) != 0, out)
        t |> success(find(out, "do not pair up") >= 0, out)
        t |> equal(caddyfile_of(dangling), fread(st.caddyfile))
        t |> equal(0, backups(st))
    }
    t |> run("a config caddy rejects is rolled back and not reloaded") @(t : T?) {
        let st = stage("rejected", VHOST_OWN)
        fwrite("{st.dir}/caddy-rejects", "x")
        var out = ""
        t |> success(run_caddy(st, out) != 0, out)
        t |> success(find(out, "caddy validate FAILED") >= 0, out)
        t |> equal(caddyfile_of(VHOST_OWN), fread(st.caddyfile), "the file the run started from")
        t |> success(!fexist("{st.dir}/reloads"))
    }
}
