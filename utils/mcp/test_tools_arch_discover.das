options gen2
options rtti

require dastest/testing_boost public
require strings
require daslib/fio
require _tools_common

require tools/arch_of public
require tools/arch_sites public
require tools/discover public

// Every citation these fixtures plant is assembled from ARCH_ANN rather than spelled out: the
// LINT026 forward pass reads source and skips only `//` comments, so a citation spelled out in
// one of the string literals below would be a citation made by THIS file, against a document
// utils/mcp does not carry.
let ARCH_ANN = "arch"

// A temp folder holding a document and the code citing it. Both sides have to live under one
// root because arch_sites walks the subtree the DOCUMENT owns. `code.das` cites twice: once
// into a section the document declares, once into an anchor it does not.
def private plant_arch_fixture(name : string; var err : string&) : string {
    let dir = create_temp_directory(name, err)
    return "" if (empty(dir))
    fwrite(path_join(dir, "ARCHITECTURE.md"), build_string() $(var w) {
        w |> write("# Doc\n\n## Tokenizer \{#tokenizer\}\n\nThe tokenizer section body.\n\n")
        w |> write("### Details \{#details\}\n\nA subsection stays inside its parent.\n\n")
        w |> write("## Buffers \{#buffers\}\n\nThe buffers section body.\n")
    })
    fwrite(path_join(dir, "code.das"), build_string() $(var w) {
        w |> write("options gen2\n\n")
        w |> write("[{ARCH_ANN}(at=\"ARCHITECTURE.md#tokenizer\")]\ndef scan_tokens \{\n\}\n\n")
        w |> write("[{ARCH_ANN}(at=\"ARCHITECTURE.md#gone\")]\ndef dangling \{\n\}\n")
    })
    return dir
}

def private discover_lines(query, limit, module_filter : string; var is_error : bool&) : array<string> {
    //! The tool's text split into lines: line 0 is the header, the rows follow in rank order.
    var text : string
    parse_result(do_discover(query, limit, module_filter), text, is_error)
    return <- split(text, "\n")
}

def private top_symbol(lines : array<string>) : string {
    //! The `module::name` of the top row - the text before its ` - description` or `  [group]` tail.
    return "" if (length(lines) < 2)
    var cut = length(lines[1])
    for (tail in [" - ", "  ["]) {
        let at = find(lines[1], tail)
        if (at >= 0 && at < cut) {
            cut = at
        }
    }
    return trim(slice(lines[1], 0, cut))
}

[test(description = "arch_of locates each citation of a file with its line, symbol and section range, reports dangling ones with lint's reason, filters by symbol and rejects missing files.")]
def test_arch_of(t : T?) {
    t |> run("a resolving citation carries the cited section text") <| @(t : T?) {
        var terr : string
        let dir = plant_arch_fixture("mcp_arch_of_test", terr)
        if (empty(dir)) {
            t |> failure("temp dir: {terr}")
            return
        }
        var text : string
        var is_error = false
        let ok = parse_result(do_arch_of(path_join(dir, "code.das")), text, is_error)
        t |> success(ok, "parse result JSON")
        t |> success(!is_error, "a readable file is not a tool failure -- text={text}")
        t |> success(find(text, "code.das:3  scan_tokens  ->  ") >= 0,
            "the citation is a locator: its own line, the annotated function, then where it lands -- text={text}")
        t |> success(find(text, "ARCHITECTURE.md:3-9  #tokenizer") >= 0,
            "the section is a line range from its heading through its last line - the deeper heading at 7 does not end it, the next same-level one at 11 does -- text={text}")
        t |> success(find(text, "The tokenizer section body.") < 0,
            "the text itself is the document's to show; the tool gives coordinates -- text={text}")
        t |> success(find(text, "#buffers") < 0, "a section nothing cites is not in the answer -- text={text}")
        rmdir_rec(dir)
    }
    t |> run("a dangling citation reports LINT026's reason") <| @(t : T?) {
        var terr : string
        let dir = plant_arch_fixture("mcp_arch_dangling_test", terr)
        if (empty(dir)) {
            t |> failure("temp dir: {terr}")
            return
        }
        var text : string
        var is_error = false
        parse_result(do_arch_of(path_join(dir, "code.das")), text, is_error)
        t |> success(find(text, "does not resolve - no such anchor") >= 0,
            "lint's own wording, so one tool's answer reads as the other's -- text={text}")
        t |> success(find(text, "code.das:7  dangling  ->  does not resolve") >= 0,
            "the dangling citation still names its line and symbol -- text={text}")
        rmdir_rec(dir)
    }
    t |> run("the symbol filter keeps one citation") <| @(t : T?) {
        var terr : string
        let dir = plant_arch_fixture("mcp_arch_symbol_test", terr)
        if (empty(dir)) {
            t |> failure("temp dir: {terr}")
            return
        }
        var text : string
        var is_error = false
        parse_result(do_arch_of(path_join(dir, "code.das"), "scan_tokens"), text, is_error)
        t |> success(find(text, "scan_tokens  ->  ") >= 0, "the named symbol's citation is kept -- text={text}")
        t |> success(find(text, "dangling") < 0, "the other symbol's citation is dropped -- text={text}")
        t |> success(find(text, "1 citation(s) shown.") >= 0, "and the tally says one -- text={text}")
        rmdir_rec(dir)
    }
    t |> run("a missing file is a tool failure") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_arch_of("nonexistent_arch_file_12345.das"), text, is_error)
        t |> success(is_error, "should be error -- text={text}")
        t |> success(find(text, "not found") >= 0, "should mention file not found -- text={text}")
    }
    t |> run("a file citing nothing says so") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_arch_of(fixture_path("_fixture_valid.das")), text, is_error)
        t |> success(!is_error, "no citations is not a failure -- text={text}")
        t |> success(find(text, "No [arch] citations") >= 0, "should report an empty scan -- text={text}")
    }
}

// The C++ arm of arch_of: a `// <doc>#<anchor>` pointer is read from a C++ source and located in
// the same shape a .das citation is, with no symbol - the tool does not parse C++ declarations.
[test(description = "arch_of reads pointers from a C++ source and locates them in the same shape without a symbol.")]
def test_arch_of_cpp(t : T?) {
    t |> run("a C++ source is read for its pointers, and lands in the same shape") <| @(t : T?) {
        var terr : string
        let dir = plant_arch_fixture("mcp_arch_cpp_test", terr)
        if (empty(dir)) {
            t |> failure("temp dir: {terr}")
            return
        }
        fwrite(path_join(dir, "code.cpp"), "// ARCHITECTURE.md#tokenizer - the pointer form\nvoid f() \{\}\n")
        var text : string
        var is_error = false
        parse_result(do_arch_of(path_join(dir, "code.cpp")), text, is_error)
        t |> success(!is_error, "a C++ file is a readable source -- text={text}")
        t |> success(find(text, "code.cpp:1  ->  ") >= 0,
            "the pointer is located at its line, with no symbol -- text={text}")
        t |> success(find(text, "ARCHITECTURE.md:3-9  #tokenizer") >= 0, "and lands on the same section -- text={text}")
        rmdir_rec(dir)
    }
}

[test(description = "arch_sites groups the citations planted in a document's subtree by anchor, surfaces uncited and undeclared anchors, narrows to one anchor and rejects a missing document.")]
def test_arch_sites(t : T?) {
    t |> run("the census finds the citations planted in the document's subtree") <| @(t : T?) {
        var terr : string
        let dir = plant_arch_fixture("mcp_arch_sites_test", terr)
        if (empty(dir)) {
            t |> failure("temp dir: {terr}")
            return
        }
        var text : string
        var is_error = false
        let ok = parse_result(do_arch_sites(path_join(dir, "ARCHITECTURE.md")), text, is_error)
        t |> success(ok, "parse result JSON")
        t |> success(!is_error, "an existing document is not a tool failure -- text={text}")
        t |> success(find(text, "\{#tokenizer\} (lines 3-9) - 1 citation(s)") >= 0,
            "the cited anchor is grouped with its section's line range -- text={text}")
        t |> success(find(text, "code.das:3  scan_tokens") >= 0,
            "the site names file, line and symbol -- text={text}")
        t |> success(find(text, "\{#buffers\} (lines 11-13) - no citations") >= 0,
            "an uncited anchor is part of the census -- text={text}")
        t |> success(find(text, "searched: ") >= 0 && find(text, "searched: ") < find(text, "src"),
            "the header names the document's own folder first, then the C++ homes -- text={text}")
        t |> success(find(text, "\{#gone\}") >= 0,
            "a citation naming an undeclared anchor is surfaced, not dropped -- text={text}")
        t |> success(find(text, "2 citation(s) in 1 file(s).") >= 0, "the tally counts both -- text={text}")
        rmdir_rec(dir)
    }
    t |> run("an anchor query narrows the census to that section") <| @(t : T?) {
        var terr : string
        let dir = plant_arch_fixture("mcp_arch_anchor_test", terr)
        if (empty(dir)) {
            t |> failure("temp dir: {terr}")
            return
        }
        var text : string
        var is_error = false
        let doc = path_join(dir, "ARCHITECTURE.md")
        parse_result(do_arch_sites("{doc}#tokenizer"), text, is_error)
        t |> success(find(text, "code.das:3  scan_tokens") >= 0, "the section's own site is listed -- text={text}")
        t |> success(find(text, "1 citation(s) in 1 file(s).") >= 0, "and only it -- text={text}")
        t |> success(find(text, "\{#buffers\}") < 0, "other anchors stay out -- text={text}")
        rmdir_rec(dir)
    }
    t |> run("a missing document is a tool failure") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_arch_sites("nonexistent_arch_doc_12345.md"), text, is_error)
        t |> success(is_error, "should be error -- text={text}")
        t |> success(find(text, "Document not found") >= 0, "should mention the missing document -- text={text}")
    }
}

[test(description = "discover ranks names over descriptions, bridges synonyms, supports fts5 filters and module filters, defaults its limit, and reports no match and bad queries correctly.")]
def test_discover(t : T?) {
    t |> run("query word order ranks a name over its mirror, and the group tail names the manual's heading") <| @(t : T?) {
        var is_error = false
        let lines <- discover_lines("convert uri to file name", "5", "", is_error)
        t |> success(!is_error, "should not be error -- {lines}")
        t |> success(top_symbol(lines) == "uriparser::uri_to_file_name", "uri_to_file_name ranks first, its mirror file_name_to_uri does not -- {lines}")
        t |> success(find(lines[1], "[File name conversions]") >= 0, "the row carries its group heading -- {lines}")
        t |> success(find(lines[1], " - Converts a URI string") >= 0, "the row carries its description -- {lines}")
    }
    t |> run("a word found only in descriptions still ranks its symbol first") <| @(t : T?) {
        var is_error = false
        let lines <- discover_lines("percent-encodes reserved characters", "3", "", is_error)
        t |> success(top_symbol(lines) == "uriparser::escape_uri", "description words rank escape_uri first -- {lines}")
    }
    t |> run("synonyms bridge the query's word to the name") <| @(t : T?) {
        var is_error = false
        let lines <- discover_lines("url to path", "8", "", is_error)
        let text = join(lines, "\n")
        t |> success(find(text, "uri_to_file_name") >= 0 || find(text, "uri_to_unix_file_name") >= 0, "url reaches the uri family -- {lines}")
    }
    t |> run("fts5 syntax filters before ranking") <| @(t : T?) {
        var is_error = false
        let lines <- discover_lines("uri* NOT windows", "50", "uriparser", is_error)
        let text = join(lines, "\n")
        t |> success(!is_error, "should not be error -- {lines}")
        t |> success(find(text, "uri_to_file_name") >= 0, "prefix match survives -- {lines}")
        t |> success(find(text, "uri_to_windows_file_name") < 0, "NOT excludes the windows twin -- {lines}")
    }
    t |> run("an exact name ranks first, and the module filter keeps other modules out") <| @(t : T?) {
        var is_error = false
        let in_fio <- discover_lines("base_name", "3", "fio", is_error)
        t |> success(top_symbol(in_fio) == "fio::base_name", "exact name first in the named module -- {in_fio}")
        let elsewhere <- discover_lines("base_name", "50", "uriparser", is_error)
        t |> success(find(join(elsewhere, "\n"), "fio::") < 0, "another module's rows never appear -- {elsewhere}")
    }
    t |> run("limit defaults to 12 on an empty or unparsable value and is honored otherwise") <| @(t : T?) {
        var is_error = false
        t |> success(find(discover_lines("string", "", "", is_error)[0], "showing 12") >= 0, "empty limit shows 12")
        t |> success(find(discover_lines("string", "abc", "", is_error)[0], "showing 12") >= 0, "garbage limit shows 12")
        t |> success(find(discover_lines("string", "3", "", is_error)[0], "showing 3") >= 0, "a numeric limit is honored")
    }
    t |> run("a query no symbol matches says so without an error") <| @(t : T?) {
        var is_error = false
        let lines <- discover_lines("zzqqxxyy", "5", "", is_error)
        t |> success(!is_error, "no match is not a tool failure -- {lines}")
        t |> success(find(lines[0], "no symbol matches") >= 0, "names the outcome -- {lines}")
    }
    t |> run("an empty query and a bad fts5 query are tool failures") <| @(t : T?) {
        var text : string
        var is_error = false
        parse_result(do_discover("  ", "", ""), text, is_error)
        t |> success(is_error, "empty query should be error -- text={text}")
        parse_result(do_discover("(uri", "", ""), text, is_error)
        t |> success(is_error, "unbalanced fts5 query should be error -- text={text}")
        t |> success(find(text, "invalid FTS5 query") >= 0, "names the cause -- text={text}")
    }
}
