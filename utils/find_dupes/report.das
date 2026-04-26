options gen2

require strings
require daslib/strings_boost
require daslib/fio
require cluster


struct public Report {
    files_scanned : int
    functions_analyzed : int
    threshold : float
    min_tokens : int
    exact_clusters : array<Cluster>
    fuzzy_pairs : array<FuzzyPair>
}


// All canonical text is printable ASCII by construction. File paths
// and function names may contain backslashes (on Windows) or quotes
// in pathological cases. This handles the JSON-mandatory escapes and
// passes the rest through unchanged.
def json_escape(s : string) : string {
    return build_string() $(var w) {
        peek_data(s) $(arr) {
            for (b in arr) {
                let c = int(b)
                if (c == int(0x22)) {
                    write(w, "\\\"")
                } elif (c == int(0x5C)) {
                    write(w, "\\\\")
                } elif (c == int(0x0A)) {
                    write(w, "\\n")
                } elif (c == int(0x0D)) {
                    write(w, "\\r")
                } elif (c == int(0x09)) {
                    write(w, "\\t")
                } else {
                    write(w, "{to_char(c)}")
                }
            }
        }
    }
}


def write_member(var w : StringBuilderWriter; m : MemberRef) {
    write(w, "\{\"file\":\"{json_escape(m.file)}\",\"name\":\"{json_escape(m.name)}\",\"line\":{m.line},\"is_lambda\":{m.is_lambda}\}")
}


def public write_json_report(report : Report; path : string) : bool {
    var ok = false
    fopen(path, "wb") $(fw) {
        if (fw == null) {
            return
        }
        ok = true
        let body = build_string() $(var w) {
            write(w, "\{\n")
            write(w, "  \"summary\": \{\n")
            write(w, "    \"files_scanned\": {report.files_scanned},\n")
            write(w, "    \"functions_analyzed\": {report.functions_analyzed},\n")
            write(w, "    \"exact_clusters\": {length(report.exact_clusters)},\n")
            write(w, "    \"fuzzy_pairs\": {length(report.fuzzy_pairs)},\n")
            write(w, "    \"threshold\": {report.threshold},\n")
            write(w, "    \"min_tokens\": {report.min_tokens}\n")
            write(w, "  \},\n")
            write(w, "  \"exact_clusters\": [\n")
            for (i in range(length(report.exact_clusters))) {
                write(w, "    \{\n")
                write(w, "      \"size\": {length(report.exact_clusters[i].members)},\n")
                write(w, "      \"token_count\": {report.exact_clusters[i].token_count},\n")
                write(w, "      \"canonical_sample\": \"{json_escape(report.exact_clusters[i].canonical_sample)}\",\n")
                write(w, "      \"members\": [\n")
                let mn = length(report.exact_clusters[i].members)
                for (j in range(mn)) {
                    write(w, "        ")
                    write_member(w, report.exact_clusters[i].members[j])
                    if (j + 1 < mn) {
                        write(w, ",")
                    }
                    write(w, "\n")
                }
                write(w, "      ]\n")
                write(w, "    \}")
                if (i + 1 < length(report.exact_clusters)) {
                    write(w, ",")
                }
                write(w, "\n")
            }
            write(w, "  ],\n")
            write(w, "  \"fuzzy_pairs\": [\n")
            for (i in range(length(report.fuzzy_pairs))) {
                write(w, "    \{\n")
                write(w, "      \"similarity\": {report.fuzzy_pairs[i].similarity},\n")
                write(w, "      \"a\": ")
                write_member(w, report.fuzzy_pairs[i].a)
                write(w, ",\n      \"b\": ")
                write_member(w, report.fuzzy_pairs[i].b)
                write(w, ",\n      \"canonical_a\": \"{json_escape(report.fuzzy_pairs[i].canonical_a)}\",\n")
                write(w, "      \"canonical_b\": \"{json_escape(report.fuzzy_pairs[i].canonical_b)}\"\n")
                write(w, "    \}")
                if (i + 1 < length(report.fuzzy_pairs)) {
                    write(w, ",")
                }
                write(w, "\n")
            }
            write(w, "  ]\n")
            write(w, "\}\n")
        }
        fw |> fwrite(body)
    }
    return ok
}


def member_label(m : MemberRef) : string {
    let base = m.is_lambda ? "<lambda>" : m.name
    return "{m.file}:{m.line}  {base}"
}


def public print_summary(report : Report; top : int) {
    print("\n=== find_dupes summary ===\n")
    print("files scanned     : {report.files_scanned}\n")
    print("functions analyzed: {report.functions_analyzed}\n")
    print("exact clusters    : {length(report.exact_clusters)}\n")
    print("fuzzy pairs       : {length(report.fuzzy_pairs)}  (threshold {report.threshold})\n")
    print("min tokens        : {report.min_tokens}\n")
    let n_ex = top < length(report.exact_clusters) ? top : length(report.exact_clusters)
    if (n_ex > 0) {
        print("\n--- top {n_ex} exact-clone clusters ---\n")
        for (i in range(n_ex)) {
            let size = length(report.exact_clusters[i].members)
            let tcount = report.exact_clusters[i].token_count
            print("\n[{i + 1}] size={size} tokens={tcount}\n")
            print("    {report.exact_clusters[i].canonical_sample}\n")
            for (m in report.exact_clusters[i].members) {
                print("    - {member_label(m)}\n")
            }
        }
    }
    let n_fz = top < length(report.fuzzy_pairs) ? top : length(report.fuzzy_pairs)
    if (n_fz > 0) {
        print("\n--- top {n_fz} fuzzy pairs ---\n")
        for (i in range(n_fz)) {
            print("\n[{i + 1}] similarity={report.fuzzy_pairs[i].similarity}\n")
            print("    a: {member_label(report.fuzzy_pairs[i].a)}\n")
            print("    b: {member_label(report.fuzzy_pairs[i].b)}\n")
            print("    a canonical: {report.fuzzy_pairs[i].canonical_a}\n")
            print("    b canonical: {report.fuzzy_pairs[i].canonical_b}\n")
        }
    }
}
