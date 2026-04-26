options gen2
options rtti

require strings
require daslib/strings_boost
require daslib/json_boost
require daslib/fio
require cluster


struct public Summary {
    files_scanned : int
    functions_analyzed : int
    exact_clusters : int
    fuzzy_pairs : int
    threshold : float
    min_tokens : int
}


struct public Report {
    summary : Summary
    exact_clusters : array<Cluster>
    fuzzy_pairs : array<FuzzyPair>
}


// JSON serialization is delegated to `sprint_json` from `daslib/json_boost`,
// which walks struct fields via RTTI and escapes correctly (Unicode-safe,
// emits `\u00XX` for control bytes < 0x20). Annotations on Cluster /
// FuzzyPair / Summary fields would let us rename or omit fields if needed.
def public write_json_report(report : Report; path : string) : bool {
    let body = sprint_json(report, true)
    var ok = false
    fopen(path, "wb") $(fw) {
        if (fw == null) {
            return
        }
        ok = true
        fw |> fprint(body)
    }
    return ok
}


def member_label(m : MemberRef) : string {
    let base = m.is_lambda ? "<lambda>" : m.name
    return "{m.file}:{m.line}  {base}"
}


def public print_summary(report : Report; top : int) {
    print("\n=== find_dupes summary ===\n")
    print("files scanned     : {report.summary.files_scanned}\n")
    print("functions analyzed: {report.summary.functions_analyzed}\n")
    print("exact clusters    : {length(report.exact_clusters)}\n")
    print("fuzzy pairs       : {length(report.fuzzy_pairs)}  (threshold {report.summary.threshold})\n")
    print("min tokens        : {report.summary.min_tokens}\n")
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
