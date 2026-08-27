options gen2

require strings
require daslib/strings_boost
require daslib/fio
require dastest/review_gate

// The mechanical half of examples/games/REVIEW.md (contract: REVIEW_COMMON.md at the repo root).
// Run from the repo root: bin/daslang examples/games/REVIEW.das - exit 0 clean, 1 with findings.

let PAGES_YML = ".github/workflows/pages.yml"
let EXAMPLES_JS = "site/files/examples.js"
let SAMPLES_JSON = "web/examples/ui/samples/data.json"
let INTERP_HTML = "site/examples/_interp.html"
let PORTS_DIR = "web/examples/ui/samples/examples"
let GAMES_DIR = "examples/games"

// What the interpreted fallback mounts for a game its GAME_FILES map does not name.
let DEFAULT_BUNDLE = "live_stub.das main.das"

def private dir_exists(path : string) : bool {
    let st = stat(path)
    return st.is_valid && st.is_dir
}

// The text one line puts between two markers: `id: 'pacman'` -> "pacman".
def private between(line, head, tail : string) : string {
    let from = find(line, head)
    return "" if (from < 0)
    let start = from + length(head)
    let stop = find(line, tail, start)
    return stop < 0 ? "" : slice(line, start, stop)
}

// Every id a file spells between the same two markers, one line at a time.
def private ids_between(path, head, tail : string) : array<string> {
    var out : array<string>
    for (raw in split(fread(path), "\n")) {
        let id = between(strip(raw), head, tail)
        out |> push(id) if (!empty(id))
    }
    return <- out
}

// The deploy walks its games in `for g in <ids>; do` loops - one builds the cards, one
// stages them into the site - so it carries the game list more than once.
def private workflow_card_lists(text : string) : array<string> {
    var out : array<string>
    for (raw in split(text, "\n")) {
        let line = strip(raw)
        continue if (!starts_with(line, "for g in ") || !ends_with(line, "; do"))
        out |> push(strip(between(line, "for g in ", "; do")))
    }
    return <- out
}

// Cards the artifact-verify step demands a built wasm for. It also lists the showcase
// cards, which are not games, so this is a superset of the game list.
def private verified_card_ids(text : string) : array<string> {
    var out : array<string>
    let head = "web/output64/examples/"
    for (raw in split(text, "\n")) {
        var line = strip(raw)
        continue if (!starts_with(line, head))
        line = slice(line, length(head), length(line))
        let sep = find(line, "/")
        continue if (sep < 0)
        out |> push(slice(line, 0, sep))
    }
    return <- out
}

// The file names one bracketed list holds, as base names, so a sample's
// "examples/river_run/main.das" and the fallback's "main.das" compare directly.
def private bundle_in_list(line : string; quote : int) : array<string> {
    var out : array<string>
    let open = find(line, "[")
    return <- out if (open < 0)
    var start = -1
    peek_data(line) $(d) {
        for (i in range(open, length(d))) {
            let b = int(d[i])
            break if (b == ']')
            continue if (b != quote)
            if (start < 0) {
                start = i + 1
            } else {
                out |> push(base_name(slice(d, start, i)))
                start = -1
            }
        }
    }
    out |> sort
    return <- out
}

// Each sample's file bundle, by slug. The slug line precedes its files line.
def private sample_bundles() : table<string; array<string>> {
    var out : table<string; array<string>>
    var slug = ""
    for (raw in split(fread(SAMPLES_JSON), "\n")) {
        let line = strip(raw)
        let s = between(line, "\"slug\" : \"", "\"")
        if (!empty(s)) {
            slug = s
            continue
        }
        continue if (empty(slug) || !starts_with(line, "\"files\""))
        out[slug] <- bundle_in_list(line, '"')
        slug = ""
    }
    return <- out
}

// The interpreted fallback's GAME_FILES map. Read only between its braces - single
// quotes and brackets are ordinary JavaScript everywhere else in the page.
def private interp_bundles() : table<string; array<string>> {
    var out : table<string; array<string>>
    var inside = false
    for (raw in split(fread(INTERP_HTML), "\n")) {
        let line = strip(raw)
        if (!inside) {
            inside = find(line, "GAME_FILES = \{") >= 0
            continue
        }
        break if (starts_with(line, "\}"))
        let id = between(line, "'", "'")
        continue if (empty(id))
        out[id] <- bundle_in_list(line, '\'')
    }
    return <- out
}

// A ported game's bundle is written down twice - once as the sample's file list, once
// as the interpreted fallback's - and a file added to the port reaches neither on its
// own. Both copies name one bundle, and every file in it is there to serve.
def private check_port_bundles(games : array<string>) {
    let samples <- sample_bundles()
    let interp <- interp_bundles()
    for (g in games) {
        continue if (!dir_exists("{PORTS_DIR}/{g}"))
        get(samples, g) $(files) {
            var mounted = DEFAULT_BUNDLE
            get(interp, g) $(named) {
                mounted = join(named, " ")
            }
            let listed = join(files, " ")
            if (listed != mounted) {
                gate_finding(INTERP_HTML, find_line(fread(INTERP_HTML), "GAME_FILES = \{"),
                    "the interpreted fallback mounts \"{mounted}\" for {g} while its sample lists \"{listed}\" - the two name one bundle")
            }
            for (f in files) {
                continue if (fexist("{PORTS_DIR}/{g}/{f}"))
                gate_finding(SAMPLES_JSON, "{g} lists {f}, which is not in {PORTS_DIR}/{g} - the sample would fail to load it")
            }
        }
    }
}

// daspkg names the released card from package_name and emits <name>/<name>.wasm, while
// the deploy verifies and stages <dir>/<dir>.wasm. The two spellings have to be one.
def private check_package_names {
    dir(GAMES_DIR) $(name) {
        return if (name == "." || name == "..")
        let pkg = "{GAMES_DIR}/{name}/.das_package"
        return if (!fexist(pkg))
        let text = fread(pkg)
        return if (find(text, "package_name(\"{name}\")") >= 0)
        gate_finding(pkg, find_line(text, "package_name("),
            "package_name must be \"{name}\", to match the directory - daspkg names the released card from it, so any other spelling emits an artifact the deploy never looks for")
    }
}

// Each card's poster, by card id. The id line opens a card, its poster line follows.
def private card_posters() : table<string; string> {
    var out : table<string; string>
    var id = ""
    for (raw in split(fread(EXAMPLES_JS), "\n")) {
        let line = strip(raw)
        let found = between(line, "id: '", "'")
        if (!empty(found)) {
            id = found
            continue
        }
        continue if (empty(id) || !starts_with(line, "poster:"))
        out[id] = between(line, "'", "'")
        id = ""
    }
    return <- out
}

// A game the deploy builds needs its sources, an entry in every other place that names
// it, and - when a playground port exists - a sample slug, or the port ships unlisted.
def private check_card_wiring(pages : string; games : array<string>) {
    let verified <- verified_card_ids(pages)
    let carded <- ids_between(EXAMPLES_JS, "id: '", "'")
    let slugs <- ids_between(SAMPLES_JSON, "\"slug\" : \"", "\"")
    let posters <- card_posters()
    for (g in games) {
        if (!dir_exists("{GAMES_DIR}/{g}")) {
            gate_finding(PAGES_YML, find_line(pages, g),
                "the deploy builds a card for {g}, but there is no {GAMES_DIR}/{g}")
            continue
        }
        if (find_index(verified, g) < 0) {
            gate_finding(PAGES_YML, "{g} is built but the artifact-verify step never demands web/output64/examples/{g}/{g}.wasm - a card that failed to build would deploy as a placeholder without reddening the run")
        }
        if (find_index(carded, g) < 0) {
            gate_finding(EXAMPLES_JS, "{g} is deployed but the examples page has no card for it - add an entry with id: '{g}'")
        }
        get(posters, g) $(poster) {
            gate_finding(EXAMPLES_JS, "the {g} card names poster {poster}, which is not at site/{poster} - the card would show a broken image") if (!fexist("site/{poster}"))
        }
        if (dir_exists("{PORTS_DIR}/{g}") && find_index(slugs, g) < 0) {
            gate_finding(SAMPLES_JSON, "{PORTS_DIR}/{g} holds a playground port that no sample lists - add one with slug \"{g}\"")
        }
    }
}

[export]
def main() : int {
    if (!fexist(PAGES_YML)) {
        to_log(LOG_ERROR, "examples/games/REVIEW.das: run from the repo root\n")
        return 2
    }
    let pages = fread(PAGES_YML)
    let lists <- workflow_card_lists(pages)
    if (empty(lists)) {
        gate_finding(PAGES_YML, "no `for g in <ids>; do` loop - this gate reads the deployed game list from those loops and can no longer see one")
    } else {
        // A game added to one loop and not the other builds a card the deploy never
        // copies into the site, or stages a card it never built.
        for (i in range(1, length(lists))) {
            continue if (lists[i] == lists[0])
            gate_finding(PAGES_YML, find_line(pages, "for g in {lists[i]}; do"),
                "this loop walks \"{lists[i]}\" while another walks \"{lists[0]}\" - every `for g in` loop in this workflow names the same games")
        }
        let games <- [for (g in split(lists[0], " ")); g; where !empty(g)]
        check_card_wiring(pages, games)
        check_port_bundles(games)
    }
    check_package_names()
    return gate_verdict("examples/games")
}
