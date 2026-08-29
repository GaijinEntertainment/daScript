options gen2

module arch_of

require common public
require arch_common public
require daslib/fio
require strings
require daslib/strings_boost

//! `arch_of` - the `[arch]` citations one file makes, each with the document section it cites.
//!
//! The read direction of the annotation: given code, show the architecture text it answers for,
//! so the section can be read without opening the document and counting headings. Resolution is
//! LINT026's, failures included - a citation this tool cannot follow is one lint will report.

// Indent a section so it reads as quoted text rather than as part of the report.
def private write_section(var w : StringBuilderWriter; body : string) {
    for (line in split(body, "\n")) {
        w |> write(empty(line) ? "\n" : "    {line}\n")
    }
}

def private write_citation(var w : StringBuilderWriter; var docs : ArchDocs;
                           file, rel : string; c : ArchCitation; symbol : string) {
    w |> write("{rel}:{c.line}: arch(at=\"{c.raw}\")\n")
    if (!empty(symbol)) {
        w |> write("  symbol: {symbol}\n")
    }
    let reason = citation_failure_reason(docs, file, c)
    if (!empty(reason)) {
        w |> write("  does not resolve - {reason}\n")
        if (!empty(c.doc)) {
            w |> write("  doc: {c.doc}\n")
        }
        return
    }
    w |> write("  doc: {c.doc}\n")
    let body = section_text(c.doc, c.anchor)
    w |> write("  section:\n")
    write_section(w, body)
}

def do_arch_of(file : string; symbol : string = "") : string {
    return make_tool_result("'file' argument is required", true) if (empty(file))
    let resolved = resolve_path(file)
    let st = stat(resolved)
    return make_tool_result("File not found: {file}", true) if (!(st.is_valid && st.is_reg))
    let text = fread(resolved)
    return make_tool_result("File is empty or unreadable: {file}", true) if (empty(text))
    let cites <- collect_arch_citations(arch_path(resolved), text)
    let rel = make_relative_path(resolved, get_das_root())
    return make_tool_result("No [arch] citations in {rel}") if (empty(cites))
    let lines <- split(text, "\n")
    var docs : ArchDocs
    var shown = 0
    let body = build_string() $(var w) {
        for (c in cites) {
            let owner = annotated_symbol(lines, c.line)
            continue if (!empty(symbol) && owner != symbol)
            shown ++
            write_citation(w, docs, arch_path(resolved), rel, c, owner)
        }
    }
    if (shown == 0) {
        return make_tool_result("No [arch] citation on '{symbol}' in {rel} ({length(cites)} citation(s) in the file)")
    }
    return make_tool_result("{body}{shown} citation(s) shown.")
}
