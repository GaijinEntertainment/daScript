// The `[arch]` citation scanner: one implementation of LINT026's resolution semantics, shared by
// the rule that enforces them (`utils/lint/main.das`) and the tools that read them
// (`utils/mcp/tools/arch_of`, `utils/mcp/tools/arch_sites`). The failure strings live here too,
// so a citation lint reports and a citation a tool refuses cannot describe themselves
// differently.
//
// Everything here reads SOURCE rather than the AST: the annotation is an inert builtin marker, so
// citing a document costs a build nothing, and a citation must be checkable in a file the reading
// environment cannot compile.

options gen2

module arch_citations

require daslib/fio public
require strings public
require daslib/strings_boost public
require daslib/regex
require daslib/regex_boost


// The citation as written in a .das source. A match with `//` earlier on its line is prose or a
// commented-out annotation and is skipped; one inside a string literal or a block comment is not,
// so a fixture that plants citation text assembles it rather than spelling it out.
var private ARCH_CITE <- %regex~arch\s*\(\s*at\s*=\s*"([^"]*)"%%


// One citation, split at the first '#'. `doc` is normalized against the CITING file's folder, so
// it compares equal to the same document reached from anywhere else in the tree; `anchor` is
// empty when the citation carries no '#' at all.
struct public ArchCitation {
    line : int
    doc : string
    anchor : string
    raw : string
}


// One `{#anchor}` a heading declares: the name, the line that declares it, and the heading level
// that bounds its section.
struct public MdAnchor {
    name : string
    line : int
    level : int
}


// The anchors of every document a scan has resolved so far. `counts` is keyed "<doc>#<anchor>" so
// it answers the duplicate question in one lookup; `known` records existence separately, because
// a citation naming a missing file must not read as a missing anchor.
struct public ArchDocs {
    counts : table<string; int>
    known : table<string; bool>
    anchors : table<string; array<MdAnchor>>
}


// The one spelling a path is compared under: lexical, so it works on a document that is not
// there, and '/'-folded, so a Windows walk and a citation agree.
def public arch_path(path : string) : string {
    return normalize(path) |> replace("\\", "/")
}


// Where a match sits: its 1-based line, and the offset that line starts at - which is what lets
// the scan tell an annotation from the same text inside a `//` comment.
struct private SrcPos {
    line : int
    line_start : int
}


// Byte offsets (ascending) to source positions in one pass over the bytes. Counting newlines with
// find() per citation would re-walk the source from the top each time.
def private positions_of_offsets(text : string; offsets : array<int>) : array<SrcPos> {
    var out : array<SrcPos>
    out |> reserve(length(offsets))
    peek_data(text) $(d) {
        var line = 1
        var start = 0
        var at = 0
        for (off in offsets) {
            while (at < off) {
                if (int(d[at]) == '\n') {
                    line ++
                    start = at + 1
                }
                at ++
            }
            out |> push(SrcPos(line = line, line_start = start))
        }
    }
    return <- out
}


// Every citation `text` makes, in file order. `file` is only read for the folder a relative
// citation resolves against, so it may name a file that is no longer on disk.
def public collect_arch_citations(file, text : string) : array<ArchCitation> {
    var out : array<ArchCitation>
    var raws : array<string>
    var offsets : array<int>
    regex_foreach(ARCH_CITE, text) $(at) {
        offsets |> push(at.x)
        raws |> push(regex_group(ARCH_CITE, 1, text))
        return true
    }
    return <- out if (empty(raws))
    let positions <- positions_of_offsets(text, offsets)
    let folder = dir_name(file)
    for (raw, off, pos in raws, offsets, positions) {
        let slashes = find(text, "//", pos.line_start)
        continue if (slashes >= 0 && slashes < off)
        let hash = find(raw, "#")
        if (hash <= 0 || hash == length(raw) - 1) {
            // shape, not resolution: the compiler rejects this on a function, but a structure
            // takes the marker unvalidated (one annotation per name), so the scan owns it there
            out |> push(ArchCitation(line = pos.line, raw = raw))
            continue
        }
        out |> push(ArchCitation(line = pos.line, raw = raw, anchor = slice(raw, hash + 1),
            doc = arch_path(path_join(folder, slice(raw, 0, hash)))))
    }
    return <- out
}


// The ATX heading level of a line, 0 when the line is not a heading. One to six '#' whose run ends
// the token - so `#!shebang` and `#include` are prose. A line carrying an anchor suffix passes
// whatever follows the run, because that is the shape an anchor is read under.
def public heading_level(line : string) : int {
    let t = line |> strip
    return 0 if (!(t |> starts_with("#")))
    var n = 0
    peek_data(t) $(d) {
        let len = length(d)
        while (n < len && int(d[n]) == '#') {
            n ++
        }
    }
    return 0 if (n > 6)
    let rest = slice(t, n)
    return n if (empty(rest) || first_character(rest) == ' ' || first_character(rest) == '\t'
        || t |> ends_with("}"))
    return 0
}


// The anchor a heading line declares, name empty when it declares none. Only a heading SUFFIX
// counts - `## <title> {#anchor}` at any level, the brace group last on the line. A `{#...}`
// elsewhere is prose about anchors, not a section identity.
def public anchor_of_heading(line : string) : MdAnchor {
    var out : MdAnchor
    let t = line |> strip
    return out if (!(t |> starts_with("#")) || !(t |> ends_with("}")))
    let open = rfind(t, "\{#")
    return out if (open < 0)
    let name = slice(t, open + 2, length(t) - 1)
    return out if (empty(name))
    out.name = name
    out.level = heading_level(t)
    return out
}


// Every anchor the document at `path` declares, in file order. A repeated name is kept twice -
// the duplicate is what makes a citation ambiguous.
def public collect_md_anchors(path : string) : array<MdAnchor> {
    var out : array<MdAnchor>
    let text = fread(path)
    if (!empty(text)) {
        var lineno = 0
        for (line in split(text, "\n")) {
            lineno ++
            var a = anchor_of_heading(line)
            continue if (empty(a.name))
            a.line = lineno
            out |> push(a)
        }
    }
    return <- out
}


// True when `doc` exists, indexing its anchors on first sight.
def private arch_doc_exists(var docs : ArchDocs; doc : string) : bool {
    if (!key_exists(docs.known, doc)) {
        let st = stat(doc)
        let ok = st.is_valid && st.is_reg
        docs.known[doc] = ok
        if (ok) {
            var found <- collect_md_anchors(doc)
            for (a in found) {
                let key = "{doc}#{a.name}"
                docs.counts[key] = (docs.counts ?[key] ?? 0) + 1
            }
            docs.anchors[doc] <- found
        }
    }
    return docs.known[doc]
}


// Every anchor `doc` declares, through the same one-parse index the forward check uses; nothing
// when the document is missing.
def public each_arch_anchor(var docs : ArchDocs; doc : string; blk : block<(a : MdAnchor) : void>) {
    return if (!arch_doc_exists(docs, doc))
    for (a in docs.anchors[doc]) {
        invoke(blk, a)
    }
}


// Why a citation does not resolve, in the words LINT026 reports; "" when it resolves. A citation
// names a document section that must be there, exactly once: an anchor declared twice is worse
// than a missing one - the citation reads as precise and points at whichever section the reader
// finds first.
def public citation_failure_reason(var docs : ArchDocs; c : ArchCitation) : string {
    return "malformed citation (expected <doc>.md#<anchor>)" if (empty(c.anchor))
    return "no such file" if (!arch_doc_exists(docs, c.doc))
    let n = docs.counts ?["{c.doc}#{c.anchor}"] ?? 0
    return "no such anchor" if (n == 0)
    return "anchor appears {n} times" if (n > 1)
    return ""
}


// True when the cited document does not sit in the citing file's own folder tree - the document's
// folder must be the file's folder or one of its ancestors. A mechanism another folder's document
// states is restated in prose in a document of the citer's own tree and cited there.
def public citation_outside_subtree(file : string; c : ArchCitation) : bool {
    let fdir = arch_path(dir_name(file))
    let ddir = dir_name(c.doc)
    return !(fdir == ddir || fdir |> starts_with("{ddir}/"))
}


// `citation_failure_reason` plus the scope check: a citation that resolves but reaches outside
// the citing file's folder tree is reported after the resolution reasons, so a broken far
// citation is named for what is broken first.
def public citation_failure_reason(var docs : ArchDocs; file : string; c : ArchCitation) : string {
    let reason = citation_failure_reason(docs, c)
    return reason if (!empty(reason))
    return "cites a document outside this file's folder tree - restate the mechanism in an architecture doc of this folder's tree and cite that" if (citation_outside_subtree(file, c))
    return ""
}
